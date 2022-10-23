#pragma once

#include <iostream>

namespace pattern_bridge {

    enum class FlusherType {
        CONSOLE,
        FILE
    };

    class FlusherInterface;
    using FlusherFabric_t = FlusherInterface* (*)(FlusherType type);

    class FlusherInterface {
    public:
        virtual void Open(std::string command) = 0;
        virtual void Close()                   = 0;
        virtual void Write(std::string text)   = 0;
    };

    class FlushBrige {
    public:
        FlushBrige(FlusherFabric_t f, FlusherType type) { imp = (*f)(type); }
        void Open(std::string command) { GetImp()->Open(command); };
        void Close() { GetImp()->Close(); }
        void Write(std::string text) { GetImp()->Write(text); };

    protected:
        FlusherInterface* GetImp() { return imp; }

    private:
        FlusherInterface* imp;
    };
}