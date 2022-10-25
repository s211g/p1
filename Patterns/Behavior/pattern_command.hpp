#pragma once

#include <iostream>

namespace pattern_command {

    class Command {
    public:
        ~Command()             = default;
        virtual void Execute() = 0;

    protected:
        Command(){};
    };

    // шаблон для простых команд без аргументов и без отмены
    template <typename Receiver>
    class SimpleCommand : public Command {
    public:
        using Action = void (Receiver::*)();
        SimpleCommand(Receiver* receiver_, Action action_) :
            receiver(receiver_), action(action_) {}
        void Execute() override {
            (receiver->*action)();
        }

    private:
        Receiver* receiver;
        Action action;
    };
}