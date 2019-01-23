#ifndef _MAZEFACTORY_H_
#define _MAZEFACTORY_H_

class MazeFactory {
public:
    static MazeFactory* instance(int type);
    void saySomething();
    ~MazeFactory();
protected:
    //mazeFactory();
private:
    static MazeFactory* _instance;
};

#endif