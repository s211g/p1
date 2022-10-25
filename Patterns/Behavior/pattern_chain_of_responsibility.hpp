#pragma once

#include <iostream>
#include <string>

namespace pattern_chain_of_responsibility {

    enum class Request_t {
        WHERE_I_AM,
        APPLICATION,
        DIALOG,
        BUTTON,
        UNKNOWN
    };

    struct Request {
        Request_t type;
    };

    class RequestHandler {
    public:
        RequestHandler(std::string name_, RequestHandler* successor_) :
            name(name_), successor(successor_) {
        }

        virtual void Handle(Request* req) {
            if (req->type == Request_t::WHERE_I_AM) {
                std::cout << "Response: " << name << std::endl;
                return;
            }
            if (successor)
                successor->Handle(req);
        }

    private:
        std::string name;
        RequestHandler* successor;
    };

}
