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
        // compare_exchange_weak(expected, new_value) - replace operation may fail
        // to set a new value to 100%, the operation is called in a loop,
        // причем expected надо снова возвращать в предидущее значение которое нам надопередать в функцию
        // expected 	- 	reference to the value expected to be found in the atomic object. Gets stored with the actual value of *this if the comparison fails.
        //                  !!! при неудачном вызове expected(ссылка) заполнятется текущим значением
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
                bool expected_ = expected;
                    while (!_locked.compare_exchange_weak(expected_, true, std::memory_order_acquire)) {
                    expected_ = expected; // need reload 
                }
            }

            void unlock() {
                _locked.store(false, std::memory_order_release);
            }

        private:
            std::atomic<bool> _locked;
        };
        */

        std::atomic<bool> x{false};
        std::atomic<bool> y{false};

        // thread1
        {
            x.store(true, std::memory_order_relaxed);
            y.store(true, std::memory_order_release);
        }
        // thread2
        {
            while (!y.load(std::memory_order_acquire)) {};
            bool x_ = x.load(std::memory_order_relaxed);
            assert(x_); // x == true, тк устанавливается в другом потоке до y
            std::cout << "x = " << x_ << std::endl;
        }

        std::cout << "\ntest 5" << std::endl;
        // std::memory_order_acq_rel одновременно синхронизирует с предидущим a.store(1, memory_order_release)
        //  и следующим a.load(memory_order_acquire)

        std::atomic<int> x5{0};
        // thread1
        {
            x5.store(1, std::memory_order_release);
        }
        // thread2
        {
            int expected = 1;
            while (!x5.compare_exchange_strong(expected, 2, std::memory_order_acq_rel))
                ;
        }
        // thread3
        {
            while (x5.load(std::memory_order_acquire) < 2)
                ;
            std::cout << "x5 = " << x5.load() << std::endl;
        }

        std::cout << "\ntest 6" << std::endl;
        // модификация атомарной переменной c a.store(new_value, memory_order_release) сразуже будет видна в другом потоке
        // value = a.load(memory_order_consume)
        // !!! все модификации памяти в потоке до .store() не гарантируются что будут видны в другом потоке после .load()
        struct X6 {
            int x;
            std::string s;
        };

        std::atomic<bool> y6{false};
        std::atomic<X6*> x6;

        // thread1
        {
            X6* px6 = new X6;
            px6->s  = "aaa";
            px6->x  = 123;

            y6.store(true, std::memory_order_relaxed);
            x6.store(px6, std::memory_order_release);
        }
        // thread2
        {
            while (!x6.load(std::memory_order_consume)) {};
            // есть гарантия что команды зависящие от px6(px6->s = "aaa"; px6->x = 123;) выполнились и видны в этом треде
            std::cout << "y6 = " << y6.load(std::memory_order_relaxed) << std::endl; // нет гарантии что будет = 1, хотя команда выполнилась в предидущем потоке до x6.store(release)
        }


        std::cout << "\ntest 7" << std::endl;
        // все операции с атомиками до std::atomic_thread_fence(release) в одном потоке(!!! после ее пересечения)
        // видны после std::atomic_thread_fence(acquire) в другом потоке
        std::atomic<bool> x7{false};
        std::atomic<bool> y7{false};

        // thread1
        {
            x7.store(true, std::memory_order_relaxed);
            std::atomic_thread_fence(std::memory_order_release);
            y7.store(true, std::memory_order_relaxed);
        }
        // thread2
        {
            while (!y7.load(std::memory_order_acquire)) {}; // выход из цикла - признак того что в первом потоке пересекли стену atomic_thread_fence(release)
            std::atomic_thread_fence(std::memory_order_acquire);
            bool x7_ = x7.load(std::memory_order_relaxed);
            assert(x7_); // x7 == true, тк устанавливается в другом потоке до std::atomic_thread_fence(release)
            std::cout << "x7 = " << x7_ << std::endl;
        }
    }

}