
#include <iostream>

#include "patterns_behavior_test.hpp"
#include "pattern_chain_of_responsibility.hpp"

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
}