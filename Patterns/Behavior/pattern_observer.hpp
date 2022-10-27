#pragma once

#include <list>
#include <map>

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

    class ManagedSubject;

    class ManagedObserver {
    public:
        virtual ~ManagedObserver()                   = default;
        virtual void Update(ManagedSubject* subject) = 0;

    protected:
        ManagedObserver() = default;
    };

    class ChangeManager {
    public:
        void Register(ManagedSubject* subject, ManagedObserver* observer) {
            mapSO[subject].push_back(observer);
        }
        void Unregister(ManagedSubject* subject, ManagedObserver* observer) {
            mapSO[subject].remove_if([&](ManagedObserver* o) { return o == observer; });
        }
        void Notify(ManagedSubject* subject) {
            auto& observers = mapSO[subject];
            for (auto o : observers) {
                o->Update(subject);
            }
        }

    private:
        std::map<ManagedSubject*, std::list<ManagedObserver*>> mapSO;
    };

    class ManagedSubject {
    public:
        virtual ~ManagedSubject() = default;
        virtual void Attach(ManagedObserver* observer) {
            manager->Register(this, observer);
        }
        virtual void Detach(ManagedObserver* observer) {
            manager->Unregister(this, observer);
        }
        virtual void Notify() {
            manager->Notify(this);
        }

    protected:
        ManagedSubject(ChangeManager* manager_) :
            manager(manager_) {}

    private:
        ChangeManager* manager;
    };

}