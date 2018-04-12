#include "IrcClient.h"

IrcClient::IrcClient(string user, string address, int port)
{
    this->username = user;
    this->tcp_client = new TcpClient(address, port);
}

void IrcClient::connect()
{
    this->tcp_client->connect();
}

bool IrcClient::send_cmd(string cmd, string message)
{
    string to_send = cmd + " " + message + "\r\n";
    cout << "[SEND CMD]: " << to_send << endl;
    return this->tcp_client->send_data(to_send);
}

void IrcClient::join_channel(string channel_name)
{
    bool joined = false;
    while (!joined)
    {
        string response = this->tcp_client->receive_data(512);
        cout << "[RECEIVED]: " << response << endl;
        if (response.find("No Ident response") != string::npos)
        {
            send_cmd("NICK", username);
            send_cmd("USER", username + " * * :" + username);
        }
        if (response.find("376") != string::npos)
        {
            //we're accepted, now let's join the channel!
            send_cmd("JOIN", "#" + channel_name);
        }
        if (response.find("433") != string::npos)
        {
            //username already in use? try to use username with _
            username = "_" + username;
            send_cmd("NICK", username);
            send_cmd("USER", username + " * * :" + username);
        }
        if (response.find("PING") != string::npos)
        {
            //we're accepted, now let's join the channel!
            ///send_cmd("PONG", ":" + resp.split(":")[1]);
        }
        if (response.find("366") != string::npos)
        {
            //we've joined
            joined = true;
            cout << "Joined!" << endl;
        }
    }

    while (true)
    {
        string response = this->tcp_client->receive_data(512);
        cout << "[RECEIVED]: " << response << endl;
    }
}