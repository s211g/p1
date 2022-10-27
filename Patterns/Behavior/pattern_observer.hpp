#pragma once

#include <list>

namespace pattern_observer {

    class Subject;
    class Observer {
    public:
        virtual ~Observer()                   = default;
        virtual void Update(Subject* subject) = 0;

    protected:
        Observer() = default;
    };

    class Subject {
    public:
        virtual ~Subject() = default;
        virtual void Attach(Observer* observer) {
            observers.push_back(observer);
        }
        virtual void Detach(Observer* observer) {
            observers.remove_if([&](Observer* o) { return o == observer; });
        }
        virtual void Notify() {
            for (auto o : observers) {
                o->Update(this);
            }
        }

    protected:
        Subject() = default;

    private:
        std::list<Observer*> observers;
    };

}