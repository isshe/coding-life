[TOC]

# 命令模式-C++ 代码示例

## 1. 文件列表
* CPP/:
    * main.cpp: 测试程序。
    * Command.h: 定义命令的接口，抽象类。
    * ConcreteCommand.h/cpp: 具体的命令，继承自 Command
    * Invoker.h/cpp: 触发者类。
    * Receiver.h/cpp: 工作类，实际执行动作的类。

## 2. 各个文件关键代码

### 2.1 主程序
* main.cpp: 测试程序
```cpp
int main()
{
    Invoker *invoker = new Invoker();
    Receiver *receiver = new Receiver();
    ConcreteCommand *cc = new ConcreteCommand();

    invoker->setCommand(cc);
    invoker->executeAll();

    return 0;
}
```

### 2.2 命令抽象类
* Command.h: 定义命令的接口，抽象类
```cpp
#ifndef _COMMAND_H_
#define _COMMAND_H_

class Command {
public:
    virtual void execute() = 0;
};

#endif
```

### 2.3 具体命令
* ConcreteCommand.h
```cpp
class ConcreteCommand : public Command {
public:
    void execute();

private:
    Receiver *_receiver;
};
```

* ConcreteCommand.cpp
```cpp
void ConcreteCommand::execute() {
    _receiver->Action();
}
```

### 2.4 接收者类
* Receiver.h
```cpp
#ifndef _RECEIVER_H_
#define _RECEIVER_H_

class Receiver {
public:
    void Action();
};

#endif
```

* Receiver.cpp
```cpp
void Receiver::Action() {
    std::cout << "Receiver::Action()..." << std::endl;
}

```

### 2.5 触发者类
* Invoker.h
```cpp
class Invoker {
public:
    void executeAll();
    void setCommand(Command *com);

private:
    Command *_command;
};
```

* Invoker.cpp
```cpp
void Invoker::executeAll() {
    _command->execute();
}

void Invoker::setCommand(Command *com) {
    _command = com;
}

```

## x. 疑问

## y. 拓展/总结

## z. 参考
* 《设计模式：可复用面向对象软件的基础》

