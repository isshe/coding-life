
#include "MazeFactory.h"
#include "BombedMazeFactory.h"
#include "EnchantedMazeFactory.h"

int main()
{
    MazeFactory *mf = MazeFactory::instance(0);
    mf->saySomething();
    delete mf;

    // 还要知道类型，还要类型转换，这么用是不是很不和谐？（要求用户完全知道自己创建的是什么类型）
    BombedMazeFactory *bmf = (BombedMazeFactory *)MazeFactory::instance(1);
    bmf->saySomething();
    delete bmf;

    EnchantedMazeFactory *emf = (EnchantedMazeFactory *)MazeFactory::instance(2);
    emf->saySomething();
    delete emf;

    return 0;
}