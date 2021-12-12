[TOC]

# 状态模式-C++示例
* 如果不熟悉C++，要注意循环引用的问题。
* 这个示例状态转移有问题...
* 状态转移过程：
```cpp
TCPClosed->TCPEstablished
(关闭)->(建立)

TCPEstablished->TCPClosed
(建立)->(关闭)
```

## 1. 文件列表
* TCPConnection.h/cpp: Context, 保存state，设置state
* TCPState.h/cpp: State，抽象类，实现缺省实现。
* TCPClosed.h/cpp: ConcreteState，具体状态，继承自TCPState。
* TCPEstablished.h/cpp: ConcreteState，具体状态，继承自TCPState。
* TCPListen.h/cpp: ConcreteState，具体状态，继承自TCPState。
* main.cpp：主函数。

## 2. 各个文件关键代码
### 2.1 Context
* TCPConnection.h
```cpp
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
```

* TCPConnection.cpp
```cpp
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
```

### 2.2 State
* TCPState.h
```cpp
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

```

* TCPState.cpp
```cpp
void TCPState::Transmit(TCPConnection *c) {}
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
```

### 2.3 ConcreteState
> 只贴TCPClosed的代码

* TCPClosed.h
```cpp
class TCPClosed : public TCPState {
public:
    static TCPState* Instance();

    virtual void ActiveOpen(TCPConnection *c);
    virtual void PassiveOpen(TCPConnection *c);
protected:
    TCPClosed() {};
private:
    static TCPClosed* _instance;
};
```

* TCPClosed.cpp
```cpp
TCPClosed* TCPClosed::_instance = 0;

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
```

### 2.4 测试
* main.cpp
```cpp
int main()
{
    TCPConnection tc;
    tc.ActiveOpen();
    tc.Close();
    return 0;
}
```

## x. 疑问

## y. 拓展/总结

## z. 参考
* 《设计模式》

