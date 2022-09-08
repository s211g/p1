#include "functional_hdr_test.hpp"
#include <future>
#include <string>
#include <vector>

namespace future_hdr_test {

    class A {
    public:
        int i{0};
        A() { std::cout << "A::A()" << std::endl; }
        A(A& a) {
            std::cout << "A::A(A&)" << std::endl;
            i = a.i;
        }
        A(const A& a) {
            std::cout << "A::A(const A&)" << std::endl;
            i = a.i;
        }
        A(A&& a) {
            std::cout << "A::A(A&&)" << std::endl;
            i = a.i;
        }

        A(int i_) :
            i(i_) { std::cout << "A::A(int)" << std::endl; }
        void operator()() const { std::cout << "A::operator()() i = " << i << std::endl; }
        void operator()(int i_) const { std::cout << "A::operator()(int i_ ) i_ = " << i_ << ", i = " << i << std::endl; }
        int operator()(double d) const {
            std::cout << "int A::operator()(double d) d = " << d << std::endl;
            return d * 10;
        }
        void fn() { std::cout << "A::fn()" << std::endl; }
        void fn(int i_) {
            std::cout << "A::fn(int) i = " << i_ << std::endl;
            i = i_;
        }
        void print() { std::cout << "A::print() i = " << i << std::endl; }
    };

    struct B {
        int i{0};
        void print() { std::cout << "B::print() i = " << i << std::endl; }
    };

    B fn(B&);
    B fn(B& b) {
        std::cout << "call B fn(B& b) b.i = " << b.i << std::endl;
        return b;
    }

    void test_async() {
        std::cout << "test std::async" << std::endl;

        std::cout << "\ntest 1" << std::endl;
        B b{11};
        std::future<B> f1 = std::async(fn, std::ref(b));
        B b1              = f1.get(); // call fn(B& b)
        b1.print();

        //        std::this_thread::sleep_for(std::chrono::milliseconds(50));

        std::cout << "\ntest 2" << std::endl;
        A a2(2);
        //auto f2 = std::async(a2); // not correct, need ref; call A(a2).operator()
        auto f2 = std::async(std::ref(a2));
        f2.wait(); // call a2.operator()

        std::cout << "\ntest 3" << std::endl;
        auto f3 = std::async(A(3));
        f3.wait(); // call temp_A(3)()

        std::cout << "\ntest 4" << std::endl;
        A a4(4);
        //auto f4 = std::async(&A::print, a4); f4.wait(); // call A(a4).print()
        auto f4 = std::async(&A::print, std::ref(a4));
        f4.wait(); // call a4.print()

        // std::launch::async - new thread
        // std::launch::deferred - same thread
        std::cout << "\ntest 5" << std::endl;
        auto f5 = std::async(std::launch::deferred, [] { std::cout << "Hi from thread" << std::endl; });
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        std::cout << "std::async(deferred, ...) ... " << std::endl;
        f5.wait();

        std::cout << "\ntest 6" << std::endl;
        auto f6 = std::async([] {
            std::cout << "Start thread" << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(2));
            std::cout << "End thread" << std::endl;
        });
        if (std::future_status::timeout == f6.wait_for(std::chrono::seconds(1))) {
            std::cout << "Wait timeout" << std::endl;
        }

        std::cout << "\ntest 7" << std::endl;
        // async залипает в той же области где разрушается future, если запускать в цикле то пока предидущий
        // тред не завершится следующий async не запустится
        // даже если вызвать future.wait_for(0) залипаем в области видимости пока тред текущего async не завершиться
        // тоесть удаление(детач от треда async) future от треда пока тот не завершиться невозможен
        auto fn7 = [](int thread_id, int timeout) -> int {
            std::cout << "thread " << thread_id << "start" << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(timeout));
            std::cout << "thread " << thread_id << "end" << std::endl;
            return thread_id;
        };

        int count = 3;
        while (count--) {
            auto f = std::async(fn7, count, 2);
            f.wait_for(std::chrono::microseconds(0));
        }
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }

    std::string packaged_task1(int i) {
        std::cout << "packaged_task1(" << i << ")" << std::endl;
        return std::to_string(i);
    }

    class C {
    public:
        int operator()(double d) {
            std::cout << "int C::operator()(double d) d = " << d << std::endl;
            return d * 10;
        }
    };

    void test_packaged_task() {
        std::cout << "test std::packaged_task" << std::endl;

        // async либо запускает тред !!!СРАЗУЖЕ (может быть неконтролируемое количество)
        // либо в возвращенной future (если использовать флаг deferred) не запуская потока и результат вычисляется при future.get() в томже потоке
        // а packaged_task никаких тредов не создает он создает два объекта future и task, future пользует один поток чтобы ждать результат а task() дергается в другом потоке
        // пул сформированных задач task можно впаралель выпонять определенным количеством тредов что не приведет к неконтролируемому росту паралельно выполняющихся задач
        // по сравнению c async

        std::cout << "\ntest 1" << std::endl;
        std::packaged_task<std::string(int)> task1(packaged_task1);
        task1(1);

        std::cout << "\ntest 2" << std::endl;
        A a;
        std::packaged_task<int(double)> task2(std::ref(a)); // без ref() делаются копии
        auto f2  = task2.get_future();
        double d = 2.2;
        task2(d);
        std::cout << "return value = " << f2.get() << std::endl;

        std::cout << "\ntest 3" << std::endl;
        auto fn = [](int thread_id, int timeout) -> int {
            std::cout << "thread " << thread_id << "start" << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(timeout));
            std::cout << "thread " << thread_id << "end" << std::endl;
            return thread_id;
        };

        int count = 5;
        std::vector<std::packaged_task<int(void)>> tasks;
        std::vector<std::future<int>> futures;

        while (count--) {
            std::packaged_task<int(void)> task(std::bind(fn, count, count));
            futures.push_back(std::move(task.get_future()));
            tasks.push_back(std::move(task));
        }

        std::vector<std::future<void>> afutures;
        for (auto& f : futures) {
            afutures.push_back(
                std::async([](std::future<int>&& f) { 
                int thread_id = f.get(); 
                std::cout << "detected thread id: " << thread_id << " finished" <<std::endl; }, std::move(f)));
        }
        for (auto& task : tasks) {
            afutures.push_back(std::async(std::move(task)));
        }
    }
}