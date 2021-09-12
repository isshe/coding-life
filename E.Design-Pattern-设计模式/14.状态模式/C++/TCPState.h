#ifndef _TCP_STATE_H_
#define _TCP_STATE_H_

//#include "TCPConnection.h"
class TCPConnection;            // 这里解决了循环引用


class TCPState {
public:
    virtual void Transmit(TCPConnection *c);
    virtual void ActiveOpen(TCPConnection *c);
    virtual void PassiveOpen(TCPConnection *c);
    virtual void Close(TCPConnection *c);
    virtual void Synchronize(TCPConnection *c);
    virtual void Acknowledge(TCPConnection *c);
    virtual void Send(TCPConnection *c);
protected:
    void ChangeState(TCPConnection *c, TCPState *s);
};

#endif