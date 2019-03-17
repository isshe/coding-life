#include "TCPListen.h"
#include <iostream>
#include "TCPEstablished.h"

using namespace std;

TCPListen* TCPListen::_instance = 0;

TCPListen* TCPListen::Instance() {
    if (_instance == 0) {
        _instance = new TCPListen();
    }

    return _instance;
}

void TCPListen::Send(TCPConnection *c) {
    cout << "TCPListen::Send()" << endl;
    ChangeState(c, TCPEstablished::Instance());
}

