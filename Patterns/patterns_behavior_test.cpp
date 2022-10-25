
#include <iostream>
#include <vector>
#include <algorithm>

#include "patterns_behavior_test.hpp"
#include "pattern_chain_of_responsibility.hpp"
#include "pattern_command.hpp"

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
}