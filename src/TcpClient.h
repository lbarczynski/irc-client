#include <iostream>     //cout
#include <string>
#include <string.h>
#include <stdio.h>      //printf
#include <sys/time.h>
#include <sys/socket.h> //socket
#include <arpa/inet.h>  //inet_addr
#include <netdb.h>      //hostent
#include<unistd.h>    //usleep
#include<fcntl.h> //fcntl

using namespace std;

class TcpClient
{
  public:
    TcpClient(string address, int port);
    bool connect();
    bool send_data(string data);
    string receive_data(int);
    bool disconnect();

  private:
    std::string address;
    int port;
    int socket_fd; // file descriptor that can be used in function calls that operate on sockets
    struct sockaddr_in server;
    bool create_socket();
    void setup_address_port();
    string sub_array(char * array, int begin, size_t size);
};