#include "atomic_hdr_test.hpp"
#include <atomic>
#include <string>
#include <cassert>

namespace atomic_hdr_test {

    void test_bool() {
        std::cout << "test std::atomic_bool " << std::endl;

        std::cout << "\ntest 1" << std::endl;
        //using atomic_bool = atomic<bool>;
        std::atomic_bool ab(false);

        //Checks whether the atomic operations on all objects of this type are lock-free
        std::cout << "ab.is_lock_free() : " << ab.is_lock_free() << std::endl;
        std::cout << "ab.is_always_lock_free : " << ab.is_always_lock_free << std::endl;

        std::cout << "value : " << (ab = true) << std::endl;
        std::cout << "value : " << ab.load() << std::endl;
        ab.store(false); // return void
        std::cout << "value : " << ab.load() << std::endl;
        std::cout << "set true, old value : " << ab.exchange(true) << std::endl;

        std::cout << "\ntest 2" << std::endl;
        ab             = false;
        bool expected  = true;
        bool new_value = true;
        std::cout << "current : " << ab << ", expected : " << expected << ", set new: " << new_value
                  << ", success : " << ab.compare_exchange_strong(expected, new_value) << ", result value: " << ab << std::endl;
        expected  = false;
        new_value = true;
        std::cout << "current : " << ab << ", expected : " << expected << ", set new: " << new_value
                  << ", success : " << ab.compare_exchange_strong(expected, new_value) << ", result value: " << ab << std::endl;

        std::cout << "\ntest 3" << std::endl;
        // compare_exchange_weak() - replace operation may fail
        // to set a new value to 100%, the operation is called in a loop
        expected  = true;
        new_value = false;
        std::cout << "current : " << ab << ", expected : " << expected << ", set new: " << new_value
                  << ", success : " << ab.compare_exchange_weak(expected, new_value) << ", result value: " << ab << std::endl;
    }

    void test_ptr() {
        std::cout << "test std::atomic<T*> " << std::endl;

        std::cout << "\ntest 1" << std::endl;
        std::atomic<int*> api0;
        std::cout << "api0 : " << api0.load() << std::endl;

        int i[] = {0, 1, 2, 3, 4, 5};
        std::atomic<int*> api(i);
        std::cout << "*api : " << *api << std::endl;

        // fetch_..() return old value
        std::cout << "*api.fetch_add(1) : " << *api.fetch_add(1) << ", *api : " << *api << std::endl;
        std::cout << "*api.fetch_sub(1) : " << *api.fetch_sub(1) << ", *api : " << *api << std::endl;
        // standard behaviour
        std::cout << "*++api : " << *++api << ", *api : " << *api << std::endl;
        std::cout << "*api++ : " << *api++ << ", *api : " << *api << std::endl;
        // += -= &= |= ^=  return new value
        std::cout << "*api+=1 : " << *(api += 1) << ", *api : " << *api << std::endl;
        std::cout << "*api-=1 : " << *(api -= 1) << ", *api : " << *api << std::endl;

        std::cout << "\ntest 2" << std::endl;
        // std::memory_order_relaxed не гарантирует тот же порядок изменений переменных в другом потоке
        // нет гарантий что значение измененной переменной сразу же будут видны в другом потоке
        std::atomic<bool> ab21{false};
        std::atomic<bool> ab22{false};
        // thread1
        {
            ab21.store(true, std::memory_order_relaxed);
            ab22.store(true, std::memory_order_relaxed);
        }
        // thread2
        {
            std::cout << ab22.load(std::memory_order_relaxed) << std::endl;
            std::cout << ab21.load(std::memory_order_relaxed) << std::endl;
        }
        // может быть вывод : 0 0, 0 1, 1 1, 1 0

        std::cout << "\ntest 3" << std::endl;
        // memory_order_seq_cst(default) гарантирует тот же порядок изменений переменных в другом потоке
        // значение измененной переменной сразу же будут видны в другом потоке
        // также гарантирует что значения измененных переменных до, даже если они не атомики тоже будут видны в другом потоке
        bool b3{false};
        std::atomic<bool> ab31{false};
        std::atomic<bool> ab32{false};
        // thread1
        {
            b3 = true;
            ab31.store(true, std::memory_order_seq_cst);
            ab32.store(true, std::memory_order_seq_cst);
        }
        // thread2
        {
            std::cout << ab32.load(std::memory_order_seq_cst) << std::endl;
            std::cout << ab31.load(std::memory_order_seq_cst) << std::endl;
            if (ab31.load(std::memory_order_seq_cst))
                assert(b3);
        }
        // может быть вывод : 0 1, 0 0 или 1 1, те если второе значение изменилось то первое 100% тоже изменилось

        std::cout << "\ntest 4" << std::endl;
        // модификация атомарной переменной c a.store(new_value, memory_order_release) сразуже будет видна в другом потоке
        // value = a.load(memory_order_acquire)
        // !!! все модификации памяти в потоке до .store() будут видны в другом потоке посде .load()
        // но если несколькими тредами записывать разные переменные то эти переменные читающими тердами могут увидится в разном порядке
        // тоесть нет синхронизации между атомиками если они записываются в разных потоках( с флагом memory_order_seq_cst есть)
        // пример как возможная реализация мьютекса
        /*
        class mutex {
        public:
            void lock() {
                bool expected = false;
                while (!_locked.compare_exchange_weak(expected, true, std::memory_order_acquire)) {
                    expected = false;
                }
            }

            void unlock() {
                _locked.store(false, std::memory_order_release);
            }

        private:
            std::atomic<bool> _locked;
        };
        */
    }
}