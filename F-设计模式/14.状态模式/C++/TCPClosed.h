#ifndef _TCP_CLOSED_H_
#define _TCP_CLOSED_H_

#include "TCPState.h"
#include "TCPConnection.h"

class TCPClosed : public TCPState {
public:
    static TCPState* Instance();

    virtual void ActiveOpen(TCPConnection *);
    virtual void PassiveOpen(TCPConnection *);
protected:
    TCPClosed() {};
private:
    static TCPClosed* _instance;
};

#endif