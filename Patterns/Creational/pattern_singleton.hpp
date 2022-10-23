#pragma once

#include <memory>
#include <mutex>
#include <iostream>

namespace pattern_singleton {

    class Singleton1 {
    public:
        static Singleton1* Instance();
        void f() { std::cout << "Singleton1" << std::endl; }

    protected:
        Singleton1() { std::cout << "Singleton1::Singleton1()" << std::endl; }

    private:
        static Singleton1* instance;
        static std::mutex mtx;
    };

    class Singleton2 {
    public:
        Singleton2() { std::cout << "Singleton2::Singleton2()" << std::endl; }

        static Singleton2* Instance() {
            static Singleton2 instance;
            return &instance;
        }
        void f() { std::cout << "Singleton2" << std::endl; }
    };

    template <typename T>
    class Singleton_t {
    private:
        Singleton_t<T>(const Singleton_t<T>&) = delete;
        Singleton_t<T>(Singleton_t<T>&&)      = delete;
        Singleton_t<T>& operator=(const Singleton_t<T>&) = delete;
        Singleton_t<T>& operator=(Singleton_t<T>&&) = delete;

        Singleton_t<T>() = default;

        static std::unique_ptr<T> m_instance;
        static std::once_flag m_once;

    public:
        virtual ~Singleton_t<T>() = default;

        static T* getInstance() {
            std::call_once(m_once, []() {
                m_instance.reset(new T);
            });
            return m_instance.get();
        }

        template <typename... Args>
        static T* getInstanceInit(Args&&... args) {
            std::call_once(m_once, [&]() {
                m_instance.reset(new T(std::forward<Args>(args)...));
            });
            return m_instance.get();
        }
    };

    template <typename T>
    std::unique_ptr<T> Singleton_t<T>::m_instance;
    template <typename T>
    std::once_flag Singleton_t<T>::m_once;

}