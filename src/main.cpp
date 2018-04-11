#include <iostream> //cout
#include <string>   //string
#include "IrcClient.h"
#include "TcpClient.h"

using namespace std;

int main()
{
    //cout << "Hello world!" << endl;
    //IrcClient *client = new IrcClient("console_user", "irc.freenode.net", 6667, "barytest");
    //client->connect();
    TcpClient *client = new TcpClient("irc.freenode.net", 6667);
    if (!client->connect())
    {
        return 1;
    }

    string r = "";
    do
    {
        r = client->receive_data(512);
        cout << "Received data: " << r << endl;
    } while (r.length() > 0);

    client->send_data("hello!");

    return 0;
}