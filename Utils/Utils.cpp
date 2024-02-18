#include <cctype>
#include "Utils.hpp"

namespace utils {

    void dump(std::string caption, void* data_, uint32_t size, uint32_t interval, bool print_offset, bool print_base_offsest) {
        uint8_t* data = reinterpret_cast<uint8_t*>(data_);
        std::cout << caption << "(" << data_ << ") size = " << size << " : ";
        char buff[23];
        for (int i = 0; i < size; ++i) {
            sprintf(buff, "%02X", (int)data[i]);
            //std::cout << (int)data[i] << " ";
            //std::cout << buff << " ";

            auto NewLine = [print_base_offsest, i](void* offset) {
                char buff[32];
                sprintf(buff, "\n0x%016p ", offset);
                std::string ret(buff);
                std::transform(ret.begin() + 3, ret.end(), ret.begin() + 3,
                               [](unsigned char c) { return std::toupper(c); });

                if (print_base_offsest) {
                    sprintf(buff, "+ %-8d", i);
                    ret += buff;
                }

                ret += " : ";

                return ret;
            };

            if (print_offset)
                std::cout << (!(i % interval) ? NewLine(data + i) : "") << buff << " ";
            else
                std::cout << (!(i % interval) ? "\n" : "") << buff << " ";
        }
        std::cout << std::endl;
    }
}