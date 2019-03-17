#include "TCPClosed.h"
#include <iostream>
#include "TCPEstablished.h"
#include "TCPListen.h"

using namespace std;

TCPState* TCPState::_instance = 0;

TCPState* TCPClosed::Instance() {
    if (_instance == 0) {
        _instance = new TCPClosed();
    }

    return _instance;
}

void TCPClosed::ActiveOpen(TCPConnection *c) {
    cout << "TCPClosed::ActiveOpen()" << endl;
    ChangeState(c, TCPEstablished::Instance());
}

void TCPClosed::PassiveOpen(TCPConnection *c) {
    cout << "TCPClosed::PassiveOpen()" << endl;
    ChangeState(c, TCPListen::Instance());
}