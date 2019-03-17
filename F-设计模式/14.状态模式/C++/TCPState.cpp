
#include "TCPState.h"
#include "TCPListen.h"
#include "TCPEstablished.h"
#include "TCPClosed.h"

#include <iostream>

using namespace std;

void TCPState::Transimit(TCPConnection *c) {}
void TCPState::ActiveOpen(TCPConnection *c) {}
void TCPState::PassiveOpen(TCPConnection *c) {}
void TCPState::Close(TCPConnection *c) {}
void TCPState::Synchronize(TCPConnection *c) {}
void TCPState::Acknowledge(TCPConnection *c) {}
void TCPState::Send(TCPConnection *c) {}
void TCPState::ChangeState(TCPConnection *c, TCPState *s) {
    c->ChangeState(s);
    if (s == TCPEstablished::Instance()) {
        cout << "change state to TCPEstablished..." << endl;
    }

    if (s == TCPClosed::Instance()) {
        cout << "change state to TCPClosed..." << endl;
    }

    if (s == TCPListen::Instance()) {
        cout << "change state to TCPListen..." << endl;
    }
}