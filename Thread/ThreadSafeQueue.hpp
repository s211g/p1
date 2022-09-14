#pragma once

#include <atomic>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

namespace ThreadSafeQueue {

    template <typename T>
    class ThreadSafeQueue1 {
    private:
        mutable std::mutex m_mtx;
        std::queue<T> m_queue;
        std::condition_variable m_cv;

    public:
        ThreadSafeQueue1() {}
        void push(T value) {
            std::lock_guard lock(m_mtx);
            m_queue.push(std::move(value));
            m_cv.notify_one();
        }

        bool pop(T& value) {
            std::lock_guard lock(m_mtx);
            if (m_queue.empty())
                return false;
            value = std::move(m_queue.front());
            m_queue.pop();
            return true;
        }

        bool pop(T& value, uint32_t timeout_ms) {
            std::unique_lock lock(m_mtx);
            if (m_cv.wait_for(lock, std::chrono::milliseconds(timeout_ms), [this] { return !m_queue.empty(); })) {
                value = std::move(m_queue.front());
                m_queue.pop();
                return true;
            }
            return false;
        }

        bool empty() {
            std::lock_guard lock(m_mtx);
            return m_queue.empty();
        }
    };
}