#pragma once

#include <string>
#include <vector>

namespace pattern_strategy {

    class StringSplitterStrategyBase {
    public:
        virtual ~StringSplitterStrategyBase() = default;
        virtual std::vector<std::string> Split(const char* text, size_t length, char delimiter) {
            std::vector<std::string> result;
            result.emplace_back(text, length);
            return result;
        }
    };

    class StringSplitterContext {
    public:
        StringSplitterContext(StringSplitterStrategyBase* strategy_, char delimiter_) :
            strategy(strategy_), delimiter(delimiter_) {}
        virtual ~StringSplitterContext() {
            delete strategy;
        }

        std::vector<std::string> Split(const char* text, size_t length) {
            return strategy->Split(text, length, delimiter);
        }

    private:
        StringSplitterStrategyBase* strategy;
        char delimiter;
    };
}