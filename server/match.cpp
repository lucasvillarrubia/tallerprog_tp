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

void Match::add_player(Player* player)
{
    player_list.push_back(player);
}

bool Match::matches(int)
{
    return true;
}

void Match::disconnect()
{
}
