#include "IrcClient.h"


IrcClient::IrcClient(string username, string address, int port, string channel)
{
    this->username = username;
    this->address = address;
    this->port = port;
    this->channel = channel;
}

void IrcClient::connect()
{
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        perror("Could not create socket");
    }

    cout << "Socket created\n";

    //setup address structure
    if (inet_addr(address.c_str()) == -1)
    {
        struct hostent *he;
        struct in_addr **addr_list;

        //resolve the hostname, its not an ip address
        if ((he = gethostbyname(address.c_str())) == NULL)
        {
            //gethostbyname failed
            herror("gethostbyname");
            cout << "Failed to resolve hostname\n";

            return;
        }

        //Cast the h_addr_list to in_addr , since h_addr_list also has the ip address in long format only
        addr_list = (struct in_addr **)he->h_addr_list;

        for (int i = 0; addr_list[i] != NULL; i++)
        {
            //strcpy(ip , inet_ntoa(*addr_list[i]) );
            server.sin_addr = *addr_list[i];

            cout << address << " resolved to " << inet_ntoa(*addr_list[i]) << endl;

            break;
        }
    }
    //plain ip address
    else
    {
        server.sin_addr.s_addr = inet_addr(address.c_str());
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(port);

    //Connect to remote server
    if (::connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        return;
    }

    cout << "Connected\n";
    cout << receive(512);
    cout << receive(512);
    cout << receive(512);
    join_channel(channel);
}

string IrcClient::receive(int size=512)
{
    char buffer[size];
    string reply;
     
    //Receive a reply from the server
    if( recv(sock , buffer , sizeof(buffer) , 0) < 0)
    {
        puts("recv failed");
    }
     
    reply = buffer;
    return reply;
}
 

bool IrcClient::send_data(string data)
{
    //Send some data
    if( send(sock , data.c_str() , strlen( data.c_str() ) , 0) < 0)
    {
        perror("Send failed : ");
        return false;
    }
    cout<<"Data send\n";
     
    return true;
}

void IrcClient::send_cmd(string cmd, string message)
{
    string cmd_to_send = cmd + " " + message + "\r\n";
    send_data(cmd_to_send);
}

void IrcClient::join_channel(string channel)
{
    send_cmd("NICK", username);
    send_cmd("USER", username + " * * :" + username);
    send_cmd("JOIN", channel);
    send_cmd("PRIVMSG " + channel, ":hello from console!");

}