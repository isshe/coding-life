#ifndef _TCP_CONNECTION_H_
#define _TCP_CONNECTION_H_

#include "TCPState.h"

class TCPConnection {
public:
    TCPConnection();

    void ActiveOpen();
    void PassiveOpen();
    void Close();
    void Send();
    void Acknowledge();
    void Synchronize();

private:
    friend class TCPState;
    void ChangeState(TCPState *);
    TCPState *_state;
};

#endif