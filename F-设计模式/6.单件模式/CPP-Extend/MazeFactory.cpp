#include "MazeFactory.h"

MazeFactory* MazeFactory::_instance = 0;

MazeFactory::mazeFactory(int type) {
    _type = type;
}

MazeFactory* MazeFactory::instance() {
    if (_instance == 0) {
        if (_type == 1) {

        } else if (_type == 2) {
            _instance = new
        } else {
            _instance = new MazeFactory();
        }
    }
}
