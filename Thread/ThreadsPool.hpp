#pragma once

#include <atomic>
#include <queue>
#include <functional>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>

namespace ThreadsPool {

    using work = std::function<void()>;

    int getThreadNumber();

    struct ThreadBarier {
        std::atomic<unsigned> count;
        std::atomic<unsigned> spaces;
        std::atomic<unsigned> generation;

    public:
        ThreadBarier(unsigned count_) :
            count(count_), spaces(count_), generation() {}

        void wait() {
            unsigned const gen = generation.load();
            if (!--spaces) {
                spaces = count.load();
                ++generation;
            }
            else {
                while (generation.load() == gen)
                    std::this_thread::yield();
            }
        }

        void done_waiting() {
            --count;
            if (!--spaces) {
                spaces = count.load();
                ++generation;
            }
        }
    };

    template <class T>
    class notifyQueue {
    public:
        notifyQueue()  = default;
        ~notifyQueue() = default;

        void push(T&& item) {
            std::lock_guard<std::mutex> grd(queue_mutex);
            queue.push_front(std::move(item));
            cv.notify_all();
        }

        bool pop(T& item_out) {
            auto pop_item = [&, this] {
                if (!queue.empty()) {
                    item_out = std::move(queue.back());
                    queue.pop_back();
                    return true;
                }
                return false;
            };
            std::unique_lock<std::mutex> lk(queue_mutex);
            if (pop_item())
                return true;
            cv.wait(lk);
            if (pop_item())
                return true;
            return false;
        }

    private:
        std::deque<T> queue;
        std::mutex queue_mutex;
        std::condition_variable cv;
    };

    class ThreadsPool {
    public:
        ThreadsPool() = delete;
        ThreadsPool(const std::atomic_bool& _shutdown);
        ~ThreadsPool() = default;
        void start(uint32_t threadsCount);
        void wait();
        void schedule_work(work);

    private:
        void perform_work(size_t thIdx);
        notifyQueue<std::future<void>> work_queue;
        std::vector<std::thread> m_threads;
        const std::atomic_bool& shutdown;
    };
}