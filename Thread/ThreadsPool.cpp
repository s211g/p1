#include <iostream>

#include "ThreadsPool.hpp"

namespace ThreadsPool {

    ThreadsPool::ThreadsPool(const std::atomic_bool& _shutdown) :
        shutdown(_shutdown) {
    }

    void ThreadsPool::start(uint32_t threadsCount) {
        while (threadsCount--)
            m_threads.push_back(std::thread(&ThreadsPool::perform_work, this, m_threads.size()));
    }

    void ThreadsPool::wait() {
        // Put a dummy entry into the worker queue to wake that thread up
        work_queue.push(std::future<void>{});
        for (auto& worker_thread : m_threads) {
            if (worker_thread.joinable()) {
                // Wait for the thread to exit
                worker_thread.join();
            }
        }
    }

    void ThreadsPool::schedule_work(work task) {
        work_queue.push(std::async(std::launch::deferred, task));
    }

    void ThreadsPool::perform_work(int thIdx) {
        std::string name = "Thread_" + std::to_string(thIdx);
        std::cout << name << " ThreadsPool::perform_work() started" << std::endl;

        while (!shutdown) {
            std::future<void> next_task;
            while (!shutdown && work_queue.pop(next_task)) {
                if (!shutdown && next_task.valid()) {
                    next_task.wait();
                }
            }
        }

        std::cout << name << " ThreadsPool::perform_work() terminated" << std::endl;
    }

}