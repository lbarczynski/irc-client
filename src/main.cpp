#include <iostream> //cout
#include <string>   //string
#include "IrcClient.h"

using namespace std;

int main()
{
    IrcClient * irc_client = new IrcClient("el_bary", "irc.freenode.net", 6667);
    irc_client->connect();
    irc_client->join_channel("barytest");

    return 0;
}