#pragma once

#include <iostream>
#include <atomic>
#include <queue>
#include <functional>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>

namespace thread_test {

    using work = std::function<void()>;

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
        void terminate();
        void schedule_work(work);

    private:
        void perform_work(int thIdx);
        notifyQueue<std::future<void>> work_queue;
        std::vector<std::thread> m_threads;
        const std::atomic_bool& shutdown;
    };

    inline void test() {
        std::cout << "thread tests" << std::endl;
    }
}