#pragma once

#include <memory>
#include <functional>

namespace pattern_prototype {

    template <typename T>
    class Prototype {
    public:
        template <typename... Args>
        void Reset(Args&&... args) {
            m_instance.reset(new T(std::forward<Args>(args)...));
        }

        template <typename F, typename... Args>
        bool Initialize(F f, Args&&... args) {
            if (!m_instance)
                return false;
            return std::invoke(f, m_instance, std::forward<Args>(args)...);
        }

        std::unique_ptr<T> Clone() const {
            return std::make_unique<T>(*m_instance.get());
        }

    private:
        std::unique_ptr<T> m_instance;
    };

}