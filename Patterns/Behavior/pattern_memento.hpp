#pragma once

namespace pattern_memento {

    class State {
    public:
        virtual ~State()             = default;
        virtual State* Clone() const = 0;
    };

    class Originator;

    class Memento {
    public:
        virtual ~Memento() = default;

    private:
        friend class Originator;
        Memento() = default;
        void SetState(State* state_) {
            if (state)
                delete state;
            state = state_->Clone();
        }
        const State* GetState() const { return state; }

    private:
        State* state{nullptr};
    };

    class Originator {
    public:
        Memento* CreateMememento() {
            Memento* memento = new Memento();
            memento->SetState(state->Clone());
            return memento;
        }

        void SetMememento(const Memento* memento) {
            if (state)
                delete state;
            state = memento->GetState()->Clone();
        }

    protected:
        State* state;
    };

    class Caretaker {
    public:
        Caretaker(Originator* originator_) :
            originator(originator_) {}
        void CreateMomento() {
            memento = originator->CreateMememento();
        }
        void SetMomento() {
            if (memento)
                originator->SetMememento(memento);
        }

    private:
        Memento* memento{nullptr};
        Originator* originator;
    };
}