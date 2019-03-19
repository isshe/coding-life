// 这个类没用上

#ifndef _TCP_LISTEN_H_
#define _TCP_LISTEN_H_

#include "TCPState.h"
#include "TCPConnection.h"

class TCPListen : public TCPState {
public:
    static TCPState* Instance();

    virtual void Send(TCPConnection *);

protected:
    TCPListen() {};
private:
    static TCPListen* _instance;
};

#endif