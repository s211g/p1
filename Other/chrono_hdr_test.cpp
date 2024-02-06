
#include "chrono_hdr_test.hpp"
#include <chrono>
#include <string>
#include <thread>
#include <iostream>

namespace chrono_hdr_test {

    /*
Type 	Definition
yocto 	std::ratio<1, 1000000000000000000000000>, if std::intmax_t can represent the denominator
zepto 	std::ratio<1, 1000000000000000000000>, if std::intmax_t can represent the denominator
atto 	std::ratio<1, 1000000000000000000>
femto 	std::ratio<1, 1000000000000000>
pico 	std::ratio<1, 1000000000000>
nano 	std::ratio<1, 1000000000>
micro 	std::ratio<1, 1000000>
milli 	std::ratio<1, 1000>
centi 	std::ratio<1, 100>
deci 	std::ratio<1, 10>
deca 	std::ratio<10, 1>
hecto 	std::ratio<100, 1>
kilo 	std::ratio<1000, 1>
mega 	std::ratio<1000000, 1>
giga 	std::ratio<1000000000, 1>
tera 	std::ratio<1000000000000, 1>
peta 	std::ratio<1000000000000000, 1>
exa 	std::ratio<1000000000000000000, 1>
zetta 	std::ratio<1000000000000000000000, 1>, if std::intmax_t can represent the numerator
yotta 	std::ratio<1000000000000000000000000, 1>, if std::intmax_t can represent the numerator 

    using nanoseconds  = duration<long long, nano>;
    using microseconds = duration<long long, micro>;
    using milliseconds = duration<long long, milli>;
    using seconds      = duration<long long>;
    using minutes      = duration<int, ratio<60>>;
    using hours        = duration<int, ratio<3600>>;
#if _HAS_CXX20
    using days   = duration<int, ratio_multiply<ratio<24>, hours::period>>;
    using weeks  = duration<int, ratio_multiply<ratio<7>, days::period>>;
    using years  = duration<int, ratio_multiply<ratio<146097, 400>, days::period>>;
    using months = duration<int, ratio_divide<years::period, ratio<12>>>;
#endif // _HAS_CXX20
*/

    void test_duration() {
        std::cout << "test std::chrono::duration " << std::endl;

        auto start = std::chrono::steady_clock::now();
        std::this_thread::sleep_for(std::chrono::milliseconds(1123));
        auto end                            = std::chrono::steady_clock::now();
        std::chrono::duration d             = end - start;
        std::chrono::duration<double> sec_d = d;
        std::cout << "elapsed time : " << sec_d.count() << " sec" << std::endl;

        std::chrono::seconds s = std::chrono::duration_cast<std::chrono::seconds>(d);
        std::cout << "elapsed time : " << s.count() << " sec" << std::endl;

        std::chrono::microseconds us = std::chrono::duration_cast<std::chrono::microseconds>(d);
        std::cout << "elapsed time : " << us.count() << " us" << std::endl;
    }

    void test_time_point() {
        std::cout << "test std::chrono::time_point " << std::endl;
        //system_clock – представляет время системы. Обычно этот таймер не подходит для измерения интервалов, так как во время измерения время может быть изменено пользователем или процессом синхронизации. Обычно основывается на количестве времени, прошедших с 01.01.1970, но это не специфицировано.
        //steady_clock – представляет так называемые устойчивые часы, то есть ход которых не подвержен внешним изменениям. Хорошо подходит для измерения интервалов. Обычно его реализация основывается на времени работы системы после включения.
        //high_resolution_clock – таймер с минимально возможным периодом отсчетов, доступным системе. Может являтся псевдонимом для одного из рассмотренных (почти наверняка это steady_clock).

        std::cout << "\ntest 1" << std::endl;
        std::chrono::time_point now = std::chrono::system_clock::now();
        auto dur                    = now.time_since_epoch();
        auto sec                    = std::chrono::duration_cast<std::chrono::seconds>(dur);

#ifdef VS_COMPIL
        std::cout << "now : " << now << std::endl;
        std::cout << "time_since_epoch : " << dur << std::endl;
        std::cout << "sec         : " << sec << std::endl;
#endif

        std::cout << "sec.count() : " << sec.count() << std::endl;
        std::cout << "time(NULL)  : " << time(nullptr) << std::endl;
        // to time_t
        // years sins 20
        time_t now_t = std::chrono::system_clock::to_time_t(now + std::chrono::years(1));
        // from time_t
        auto now2 = std::chrono::system_clock::from_time_t(now_t); // +1h
        //std::cout << "now2 : " << std::to_string(now2) << std::endl;
        //std::cout << "now2 : " <<  std::format("{:%Y-%m-%d %X}", now2);
    }
}