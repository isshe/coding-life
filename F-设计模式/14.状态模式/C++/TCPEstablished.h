#ifndef _TCP_ESTABLISHED_H_
#define _TCP_ESTABLISHED_H_

#include "TCPState.h"
#include "TCPConnection.h"

class TCPEstablished : public TCPState {
public:
    static TCPState* Instance();

    virtual void Transmit(TCPConnection *);
    virtual void Close(TCPConnection *);
protected:
    TCPEstablished() {};
private:
    static TCPEstablished* _instance;
};

#endif