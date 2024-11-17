#include "match.h"

Match::Match()
{
}

void Match::start()
{
    try
    {
        //gameloop.start();
    }
    catch (...)
    {
    }
}

void Match::end()
{
    //gameloop.stop();
    //gameloop.join();
}

bool Match::is_connected()
{ return true;
}

bool Match::matches(int)
{
    return true;
}

void Match::disconnect()
{
}
