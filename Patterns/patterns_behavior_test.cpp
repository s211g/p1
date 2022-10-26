
#include <iostream>
#include <vector>
#include <algorithm>

#include "patterns_behavior_test.hpp"
#include "pattern_chain_of_responsibility.hpp"
#include "pattern_command.hpp"
#include "pattern_interpreter.hpp"
#include "pattern_iterator.hpp"
#include "pattern_mediator.hpp"

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
}