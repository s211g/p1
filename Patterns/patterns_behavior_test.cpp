
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

#include "patterns_behavior_test.hpp"
#include "pattern_chain_of_responsibility.hpp"
#include "pattern_command.hpp"
#include "pattern_interpreter.hpp"
#include "pattern_iterator.hpp"
#include "pattern_mediator.hpp"
#include "pattern_memento.hpp"
#include "pattern_observer.hpp"
#include "pattern_state.hpp"
#include "pattern_strategy.hpp"

namespace patterns_behavior_test {

    void test_chain_of_responsibility() {
        std::cout << "\ntest chain of responsibility" << std::endl;
        using namespace pattern_chain_of_responsibility;

        class Application : public RequestHandler {
        public:
            Application(std::string name, RequestHandler* successor_ = nullptr) :
                RequestHandler(name, successor_) {}

            void Handle(Request* req) override {
                if (req->type == Request_t::APPLICATION) {
                    std::cout << "Response: processed by application" << std::endl;
                    return;
                }
                RequestHandler::Handle(req);
            }
        };

        class Dialog : public RequestHandler {
        public:
            Dialog(std::string name, RequestHandler* successor_) :
                RequestHandler(name, successor_) {}

            void Handle(Request* req) override {
                if (req->type == Request_t::DIALOG) {
                    std::cout << "Response: processed by dialog" << std::endl;
                    return;
                }
                RequestHandler::Handle(req);
            }
        };

        class Button : public RequestHandler {
        public:
            Button(std::string name, RequestHandler* successor_) :
                RequestHandler(name, successor_) {}

            void Handle(Request* req) override {
                if (req->type == Request_t::BUTTON) {
                    std::cout << "Response: processed by button" << std::endl;
                    return;
                }
                RequestHandler::Handle(req);
            }
        };

        // chain of responsibility
        // участники:
        // client - отправляет запрос одному из обработчиков
        // handler - обработчик запроса, возможно имеет доступ к преемнику(которому и передает запрос если сам его не обрабатывает)
        // +
        // слабая связанность, клиенту надо знать всеголиш одного бработчика
        // -
        // запрос может быть не обработан
        // особенность:
        // обработчик может обработать и передавать запрос дальше, тоесть запрос может обрабатывается несколькими обработчиками

        std::cout << "\ntest 1" << std::endl;

        Application* application = new Application("Application");    // если не смог обработать то запрос остается не обработан
        Dialog* dialog           = new Dialog("Dialog", application); // передает запрос в application если не смог обработать
        Button* button           = new Button("Button", dialog);      // передает запрос в dialog если не смог обработать

        Request req{Request_t::WHERE_I_AM};
        application->Handle(&req);
        dialog->Handle(&req);
        button->Handle(&req);

        std::cout << "\ntest 2" << std::endl;
        Request req2{Request_t::BUTTON};
        button->Handle(&req2);

        std::cout << "\ntest 3" << std::endl;
        Request req3{Request_t::DIALOG};
        button->Handle(&req3);

        std::cout << "\ntest 4" << std::endl;
        Request req4{Request_t::APPLICATION};
        button->Handle(&req4);

        std::cout << "\ntest 5" << std::endl;
        Request req5{Request_t::UNKNOWN};
        button->Handle(&req5);
    }

    void test_command() {
        std::cout << "\ntest command" << std::endl;
        using namespace pattern_command;

        // command(action,transaction)
        // инкапсулирует запрос в объект
        // +
        // можно ставить запросы в очередь
        // протоколировать
        // поддерживать отмену операций
        // структура:
        // Invoker - инициатор, выполняет command_obj->Execute();
        // Commmand - инициализированный объект который по запросу инициатора выполняет некоторое действие над Receiver:
        // Command::Execute() { receiver->Action(); }
        // Receiver - получатель, распологает способами выполнения операций необходимых для удовлетворения запроса(receiver->Action())

        class WriteCommand : public Command {
        public:
            WriteCommand(std::ostream& receiver_, std::string text_) :
                receiver(receiver_), text(text_) {}
            void Execute() override {
                receiver << text;
            }

        private:
            std::ostream& receiver;
            std::string text;
        };

        std::cout << "\ntest 1" << std::endl;
        std::vector<Command*> commands;
        commands.push_back(new WriteCommand(std::cout, "1"));
        commands.push_back(new WriteCommand(std::cout, "2"));
        commands.push_back(new WriteCommand(std::cout, "3"));
        commands.push_back(new WriteCommand(std::cout, "\n"));
        std::for_each(commands.begin(), commands.end(), [](auto c) { c->Execute(); });

        std::cout << "\ntest 2" << std::endl;

        class MyCommand1 {
        public:
            void f1() { std::cout << "1"; }
            void f4() { std::cout << std::endl; }
        };
        class MyCommand2 {
        public:
            void f2() { std::cout << "2"; }
            void f3() { std::cout << "3"; }
        };

        std::vector<Command*> commands2;
        MyCommand1 mc1;
        MyCommand2 mc2;
        commands2.push_back(new SimpleCommand<MyCommand1>(&mc1, &MyCommand1::f1));
        commands2.push_back(new SimpleCommand<MyCommand2>(&mc2, &MyCommand2::f2));
        commands2.push_back(new SimpleCommand<MyCommand2>(&mc2, &MyCommand2::f3));
        commands2.push_back(new SimpleCommand<MyCommand1>(&mc1, &MyCommand1::f4));
        std::for_each(commands2.begin(), commands2.end(), [](auto c) { c->Execute(); });
    }

    void test_interpreter() {
        std::cout << "\ntest interpreter" << std::endl;
        using namespace pattern_interpreter;

        // определяет представление грамматики языка. интерпретатор предложений этого языка.
        // применяется где конструкции на этом языка можно представить в виде абстрактных синтаксических дервьев

        Context ctx;
        ctx.Assign("true", true);
        ctx.Assign("false", false);

        // (true and x) or y)
        // !!! парсинг в дерево отдельная задача, идеологии паттерна не касается
        VariableExp* x  = new VariableExp("x");
        VariableExp* y  = new VariableExp("y");
        AndExp* and_exp = new AndExp(new VariableExp("true"), x);
        OrExp* or_exp   = new OrExp(and_exp, y);

        std::cout << "\ntest 1" << std::endl;
        ctx.Assign("x", true);
        ctx.Assign("y", false);
        std::cout << "ret = " << or_exp->Evaluate(ctx) << std::endl;

        std::cout << "\ntest 2" << std::endl;
        ctx.Assign("x", false);
        ctx.Assign("y", false);
        std::cout << "ret = " << or_exp->Evaluate(ctx) << std::endl;

        std::cout << "\ntest 3" << std::endl;
        ctx.Assign("x", false);
        ctx.Assign("y", true);
        std::cout << "ret = " << or_exp->Evaluate(ctx) << std::endl;
    }

    void test_iterator() {
        std::cout << "\ntest iterator" << std::endl;
        using namespace pattern_iterator;

        // iterator(cursor)
        // предоставляет способ последовательного обращения ко всем элементам
        // составного объекта без раскрытия его внутреннего представления
    }

    void test_mediator() {
        std::cout << "\ntest mediator" << std::endl;
        using namespace pattern_mediator;

        // mediator(посредник)
        // определяет объект, инкапсулирующий способ взаимодействия множества объектов
        // +
        // обеспечивает слабую связанность объектов
        // коллективное поведение инкапсулируется в отдельном объекте-посреднике
        // структура паттерна:
        // colleague - подклассы объектов связанных определенным поведением
        //             каждый класс знает свой объект mediator
        // mediator - определяет интерфейс для обмена информацией между объектами(colleague)
        //            реализует кооперативное поведение, координируя действия объектов(colleague)
        //
        // одним из подходов к реализации посредника является применение паттерна наблюдатель:
        // объекты посылают извещения посреднику, посредник сообхает об них другим коллегам.
        // другой подход - определение специального интерфейса для уведомления медиатора

        class Colleague1 : public Colleague {
        public:
            Colleague1(Mediator* mediator) :
                Colleague(mediator) {}
            void SetText(std::string text_) {
                text = text_;
                std::cout << "Colleague1::SetText() text: " << text << std::endl;
                Changed();
            }
            void ResetText() {
                std::cout << "Colleague1::ResetText()" << std::endl;
                text = "";
            }
            std::string GetText() { return text; }

        private:
            std::string text;
        };

        class Colleague2 : public Colleague {
        public:
            Colleague2(Mediator* mediator) :
                Colleague(mediator) {}
            void f1(std::string text) {
                std::cout << "Colleague2::f1() text: " << text << std::endl;
            }
            void f2() {
                std::cout << "Colleague2::f2()" << std::endl;
                Changed();
            }
        };

        class DirectorMediator : public Mediator {
        public:
            void CreateColleagues() {
                colleague1 = new Colleague1(this);
                colleague2 = new Colleague2(this);
            }

            void Changed(Colleague* colleague) override {
                if (colleague == colleague1)
                    colleague2->f1(colleague1->GetText());
                if (colleague == colleague2)
                    colleague1->ResetText();
            }

        public:
            Colleague1* colleague1;
            Colleague2* colleague2;
        };

        DirectorMediator dm;
        dm.CreateColleagues();

        std::cout << "\ntest 1" << std::endl;
        dm.colleague1->SetText("123");

        std::cout << "\ntest 2" << std::endl;
        dm.colleague2->f2();
    }

    void test_memento() {
        std::cout << "\ntest memento" << std::endl;
        using namespace pattern_memento;

        // memento(token)
        // не нарушая инкапсуляции, фиксирует и выносит за пределы объекта его
        // внутреннее состояние, чтобы позднее можно было восстановить в нем объект

        // структура:

        // Originator - хозяин, state - внутренее состояние
        // Originator::CreateMemento() - создает хранитель по пинку от Caretaker
        // Originator::SetMemento(Memento m) - восстанавливает внутреннее состояние(state)

        // Memento - хранитель,
        //           запрещает доступ другим объектам, кроме хозяина
        // Memento::SetState() - сохраняет внутренее состояние объекта Originator
        // Memento::GetState() - возвращает состояние Originator-у
        // !!! чтобы хозяин имел доступ к закрытым членам надо его объявить другом

        // Caretaker - посыльный(механизм отката)
        // Caretaker::CreateMomento() - сигнал Originator -у что надо создать Memento и дернуть memento->SetState()
        // Caretaker::SetMomento() - сигнал Originator -у что надо восстановить состояние и дернуть memento->GetState()

        class AState : public State {
        public:
            std::string text;
            State* Clone() const override {
                return new AState(*this);
            }
        };

        class A : public Originator {
        public:
            ~A() = default;
            void PrintState() { std::cout << "text: " << GetState()->text << std::endl; }
            void Execute(int i) { GetState()->text += std::to_string(i); }

        private:
            AState* GetState() { return static_cast<AState*>(state); }
        };

        A a;
        Caretaker ca(&a);

        a.PrintState();
        a.Execute(1);
        a.Execute(2);
        a.Execute(3);
        a.PrintState();
        ca.CreateMomento();
        a.Execute(4);
        a.Execute(5);
        a.Execute(6);
        a.PrintState();
        ca.SetMomento();
        a.PrintState();
    }

    void test_observer() {
        std::cout << "\ntest observer" << std::endl;
        using namespace pattern_observer;

        // observer(наблюдатель), dependens(подчиненные), publish-subscribe(издатель-подписчик)
        // определяет зависимость типа "один ко многим" между объектами таким образом, что при
        // изменении состоянии одного объекта все зависящие от него оповещаются об этом и
        // автоматически обновляются

        // структура

        // Subject - наблюдаемый объект
        // Subject::Attach(Observer) - добавление наблюдателя
        // Subject::Attach(Observer, topic) - добавление наблюдателя c определенным интересом
        // Subject::Detach(Observer) - удаление наблюдателя
        // Subject::GetState() - может вызвать наблюдатель чтоб опросить состояние
        // Subject::SetState() - может вызвать наблюдатель чтоб задать состояние
        // Об изменении состояния сразу будут оповещены все наблюдатели:
        // Subject::Notify(){        - информирование всех наблюдателей об изменении состояния
        //      for_each(){ o->Update() }
        // }

        // Observer - наблюдатель
        // Observer::Update(subjectX) { - вызывается наблюдаемым объектом
        //      state = subjectX->GetState() - может в ответ посмотреть состояние
        //      subjectY->Settate(state) - может задать состояние другому наблюдаемому объекту
        // }
        // 1) (push model)модель проталкивания - в аргументе передается вся информация
        // 2) (pull model)модель вытягивания - в аргументе передается причина, наблюдатель дополнительно запрашивает уточнение
        // Observer::Update(subject, Interest interest) {}
        //      data = subject->GetState(interest);
        // }

        // ChangeManager - менеджер изменений(он же паттерн Mediator)
        // Должен уменьшить сложность семантики обновления:
        //  - определяет стратегию обновлений, например не апдейтить наблюдателя несколько раз при связанном изменении нескольких субъектов
        //  - освободить субъектов от необходимости хранить ссылки на своих наблюдателей и наоборот
        //  - обновлять всех зависимых наблюдателей по запросу субъекта

        class Timer : public Subject {
        public:
            int GetTime() {
                return i;
            }
            void Tick() {
                ++i;
                Notify();
            }

        private:
            int i{0};
        };

        class Clock : public Observer {
        public:
            Clock(Timer* timer_) :
                subject_timer(timer_) {
                subject_timer->Attach(this);
            }
            ~Clock() {
                subject_timer->Detach(this);
            }

            void Update(Subject* subject) override {
                if (subject == subject_timer) {
                    int time = subject_timer->GetTime();
                    std::cout << "Clock::Update() time = " << time << std::endl;
                }
            }

        private:
            Timer* subject_timer;
        };

        std::cout << "\ntest 1" << std::endl;
        Timer timer;
        Clock clock(&timer);
        timer.Tick();
        timer.Tick();
        timer.Tick();

        std::cout << "\ntest 2 managed subject - observer" << std::endl;
        class ManagedTimer : public ManagedSubject {
        public:
            ManagedTimer(ChangeManager* manager) :
                ManagedSubject(manager) {}

            int GetTime() {
                return i;
            }
            void Tick() {
                ++i;
                Notify();
            }

        private:
            int i{0};
        };

        class ManagedClock : public ManagedObserver {
        public:
            ManagedClock(ManagedTimer* timer_) :
                subject_timer(timer_) {
                subject_timer->Attach(this);
            }
            ~ManagedClock() {
                subject_timer->Detach(this);
            }

            void Update(ManagedSubject* subject) override {
                if (subject == subject_timer) {
                    int time = subject_timer->GetTime();
                    std::cout << "ManagedClock::Update() time = " << time << std::endl;
                }
            }

        private:
            ManagedTimer* subject_timer;
        };

        ChangeManager manager;
        ManagedTimer mtimer(&manager);
        ManagedClock mclock(&mtimer);
        mtimer.Tick();
        mtimer.Tick();
        mtimer.Tick();
    }

    void test_state() {
        std::cout << "\ntest state" << std::endl;
        using namespace pattern_state;

        // state
        // позволяет объекту изменять свое поведение в зависимости от внутреннего состояния.
        // +
        // поведение меняется динамически во время выполнения
        // не надо условных операторов для выбора поведения

        // структура
        // Context - определяет интерфейс для клиента, хранит экземпляр объекта State которым определяется текущее поведение
        //           делегирует ему зависящие от состояния запросы
        // State - реализует поведение, ассоциированое с некоторым состоянием контекста
        // смену состояний решает либо сам Context либо Context


        std::cout << "\ntest 1" << std::endl;

        TCPConnection connection;
        connection.Open();
        connection.Send("123");
        connection.Close();
    }

    void test_strategy() {
        std::cout << "\ntest state" << std::endl;
        using namespace pattern_strategy;

        // strategy, policy
        // определяет семейство алгоритмов, инкапсулирует каждый из них и делает их взаимозаменяемыми.
        // позволяет изменять алгоритмы независимо от клиентов, которые им пользуются.
        // +
        // избавляет от порождения множества подклассов с различным поведением
        // динамически выбирать алгоритм во время выполнения
        // выбирать алгорим независимо(скрыто) от клиента
        // скрытость работы и данных алгоритма от клиента
        // структура
        // Strategy - объект в котором реализовано поведение
        // Context - содержит объект strategy, при вызове его функций передает либо необходимые параметры ли бо ссылку на самого себя
        // !!! при отсутствии стратегии, Context может иметь реализацию по умолчанию
        // возможна реализация через параметр шаблона:
        // Context<Strategy> - статически привязывается с контекстом, что повышает эффективность программы

        enum class splitter_type {
            SPLITTER_TYPE_C,
            SPLITTER_TYPE_CPP,
            SPLITTER_TYPE_UNKNOWN
        };

        class StringSplitterStrategyC : public StringSplitterStrategyBase {
        public:
            std::vector<std::string> Split(const char* text, size_t length, char delimiter) override {
                std::vector<std::string> result;
                return result;
            }
        };

        class StringSplitterStrategyCPP : public StringSplitterStrategyBase {
        public:
            std::vector<std::string> Split(const char* text, size_t length, char delimiter) override {
                std::vector<std::string> result;
                std::string s(text, length);
                std::string::size_type prev_pos = 0, pos = 0;
                while ((pos = s.find(delimiter, pos)) != std::string::npos) {
                    std::string substring(s.substr(prev_pos, pos - prev_pos));
                    result.push_back(substring);
                    prev_pos = ++pos;
                }
                result.push_back(s.substr(prev_pos, pos - prev_pos)); // Last word
                return result;
            }
        };

        class StringSplitter {
        public:
            StringSplitter(splitter_type type, char delimiter) {
                switch (type) {
                    case splitter_type::SPLITTER_TYPE_C:
                        ctx = new StringSplitterContext(new StringSplitterStrategyC(), delimiter);
                        break;
                    case splitter_type::SPLITTER_TYPE_CPP:
                        ctx = new StringSplitterContext(new StringSplitterStrategyCPP(), delimiter);
                        break;
                    default:
                        ctx = new StringSplitterContext(nullptr, delimiter);
                        break;
                }
            }

            std::vector<std::string> Split(const char* text, size_t length) {
                return ctx->Split(text, length);
            }

        private:
            StringSplitterContext* ctx;
        };

        auto print_result = [](auto& v) {
            std::for_each(v.begin(), v.end(), [](auto& i) {
                std::cout << i << ",";
            });
            std::cout << std::endl;
        };

        std::cout
            << "\ntest 1" << std::endl;
        StringSplitter splitter(splitter_type::SPLITTER_TYPE_CPP, ' ');
        std::string text = "1 2 3 44 55";
        auto v1          = splitter.Split(text.c_str(), text.length());
        print_result(v1);
    }
}