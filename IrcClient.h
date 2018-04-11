#include <iostream>     //cout
#include <string>
#include <string.h>
#include <stdio.h>      //printf
#include <sys/socket.h> //socket
#include <arpa/inet.h>  //inet_addr
#include <netdb.h>      //hostent

using namespace std;

class IrcClient
{
  public:
    IrcClient(std::string username, std::string address, int port, std::string channel);
    void connect();

  private:
    std::string username;
    std::string address;
    int port;
    std::string channel;
    int sock;
    struct sockaddr_in server;
    void send_cmd(std::string cmd, std::string message);
    void join_channel(std::string channel);
    bool send_data(std::string data);
    void error(const char *msg);
    std::string receive(int);
};