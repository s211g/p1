#include "pattern_state.hpp"

namespace pattern_state {

    TCPConnection::TCPConnection() {
        state = TCPClosed::Instance();
    }

    void TCPConnection::Open() {
        state->Open(this);
    }

    void TCPConnection::Close() {
        state->Close(this);
    }

    void TCPConnection::Send(std::string text) {
        state->Send(this, text);
    }

    void TCPConnection::ChangeState(TCPState* state_) {
        std::cout << "TCPConnection::ChangeState() " << state->Name() << " -> " << state_->Name() << std::endl;
        delete state;
        state = state_;
    }

    void TCPEstablished::Close(TCPConnection* connection) {
        // close socket
        // ::close(connection->socket)
        ChangeState(connection, TCPClosed::Instance());
    }

    void TCPEstablished::Send(TCPConnection* connection, std::string text) {
        // send to socket
        // ::send(connection->socket ..
        std::cout << "TCPEstablished::Send() text: " << text << std::endl;
    }

    void TCPClosed::Open(TCPConnection* connection) {
        // open socket
        // connection->socket = new ...
        // connection->connect() ...
        ChangeState(connection, TCPEstablished::Instance());
    }
}