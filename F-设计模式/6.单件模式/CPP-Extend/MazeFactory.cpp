#include "MazeFactory.h"
#include "BombedMazeFactory.h"
#include "EnchantedMazeFactory.h"
#include <iostream>

MazeFactory* MazeFactory::_instance = 0;

MazeFactory* MazeFactory::instance(int type) {
    if (_instance == 0) {
        if (type == 1) {
            _instance = new BombedMazeFactory();
        } else if (type == 2) {
            _instance = new EnchantedMazeFactory();
        } else {
            _instance = new MazeFactory();
        }
    } else {
        std::cout << "_instance != 0" << std::endl;
    }

    return _instance;
}

MazeFactory::~MazeFactory()
{
    _instance = 0;
}

void MazeFactory::saySomething()
{
    std::cout << "MazeFactory..." << std::endl;
}
