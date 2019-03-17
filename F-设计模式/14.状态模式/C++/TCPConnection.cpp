
#include "TCPConnection.h"
TCPConnection::TCPConnection() {
    _state = TCPClosed::Instance();
}

void TCPConnection::ActiveOpen() {
    _state->ActiveOpen(this);
}

void TCPConnection::PassiveOpen() {
    _state->PassiveOpen(this);
}

void TCPConnection::Close() {
    _state->Close(this);
}
void TCPConnection::Send() {
    _state->Send(this);
}

void TCPConnection::Acknowledge() {
    _state->Acknowledge(this);
}

void TCPConnection::Synchronize() {
    _state->Synchronize(this);
}

void TCPConnection::ChangeState(TCPState *s) {
    _state = s;
}