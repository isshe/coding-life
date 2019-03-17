#include "TCPEstablished.h"
#include <iostream>
#include "TCPClosed.h"

using namespace std;

TCPEstablished* TCPEstablished::_instance = 0;

TCPEstablished* TCPEstablished::Instance() {
    if (_instance == 0) {
        _instance = new TCPEstablished();
    }

    return _instance;
}

void TCPEstablished::Transmit(TCPConnection *c) {
    cout << "TCPEstablished::Transmit()" << endl;
}

void TCPEstablished::Close(TCPConnection *c) {
    cout << "TCPEstablished::Close()" << endl;
    ChangeState(c, TCPClosed::Instance());
}