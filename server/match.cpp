#include "match.h"

void Match::start()
{
    try
    {
        gameloop.start();
    }
    catch (...)
    {
    }
}

void Match::end()
{
    gameloop.stop();
    gameloop.join();
}
