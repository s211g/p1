#pragma once

#include <string>
#include <iostream>

namespace pattern_state {

    class TCPState;

    class TCPConnection {
    public:
        TCPConnection();
        void Open();
        void Close();
        void Send(std::string text);

    private:
        friend class TCPState;
        void ChangeState(TCPState*);

    private:
        TCPState* state;
    };

    class TCPState {
    public:
        virtual std::string Name() = 0;
        virtual void Open(TCPConnection* connection) {}
        virtual void Close(TCPConnection* connection) {}
        virtual void Send(TCPConnection* connection, std::string text) {}

    protected:
        void ChangeState(TCPConnection* connection, TCPState* state) {
            connection->ChangeState(state);
        }
    };

    class TCPEstablished : public TCPState {
    public:
        static TCPState* Instance() { return new TCPEstablished(); }
        std::string Name() { return "TCPEstablished"; }
        void Close(TCPConnection* connection) override;
        void Send(TCPConnection* connection, std::string text) override;
    };

    class TCPClosed : public TCPState {
    public:
        static TCPState* Instance() { return new TCPClosed(); }
        std::string Name() override { return "TCPClosed"; }
        void Open(TCPConnection* connection) override;
    };
}