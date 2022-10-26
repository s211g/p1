#pragma once

namespace pattern_mediator {

    class Colleague;

    class Mediator {
    public:
        virtual ~Mediator()                        = default;
        virtual void Changed(Colleague* colleague) = 0;
    };

    class Colleague {
    public:
        Colleague(Mediator* mediator_) :
            mediator(mediator_) {}
        virtual ~Colleague() = default;
        void Changed() {
            mediator->Changed(this);
        }

    private:
        Mediator* mediator;
    };

}
