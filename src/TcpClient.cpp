#include "TcpClient.h"

//Size of each chunk of data received, try changing this
#define CHUNK_SIZE 512
#define SOCKET_DOMAIN AF_INET
#define SOCKET_TYPE SOCK_STREAM
#define SOCKET_PROTOCOL 0 // Specifying a protocol of 0 causes socket() to use an unspecified default protocol appropriate for the requested socket type.

TcpClient::TcpClient(string address, int port)
{
    this->address = address;
    this->port = port;
    this->socket_fd = -1;
}

bool TcpClient::create_socket()
{
    socket_fd = ::socket(SOCKET_DOMAIN, SOCKET_TYPE, SOCKET_PROTOCOL);
    if (socket_fd == -1)
    {
        perror("Could not create socket");
        return false;
    }

    cout << "Socket created" << endl;
    return true;
}

void TcpClient::setup_address_port()
{
    //setup address structure
    const char *address_cstr = address.c_str();
    if (inet_addr(address_cstr) == -1)
    {
        struct hostent *host_entity;
        struct in_addr **addr_list;

        //resolve the hostname, its not an ip address
        if ((host_entity = gethostbyname(address_cstr)) == NULL)
        {
            //gethostbyname failed
            herror("gethostbyname");
            cout << "Failed to resolve hostname\n";
        }

        //Cast the h_addr_list to in_addr , since h_addr_list also has the ip address in long format only
        addr_list = (struct in_addr **)host_entity->h_addr_list;
        for (int i = 0; addr_list[i] != NULL; i++)
        {
            server.sin_addr = *addr_list[i];
            cout << address << " resolved to " << inet_ntoa(*addr_list[i]) << endl;
            break;
        }
    }
    //plain ip address
    else
    {
        server.sin_addr.s_addr = inet_addr(address_cstr);
    }

    server.sin_family = SOCKET_DOMAIN;
    server.sin_port = htons(port);
}

bool TcpClient::connect()
{
    if (!create_socket())
    {
        return false;
    }
    setup_address_port();
    //Connect to remote server
    if (::connect(socket_fd, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        return false;
    }

    cout << "Connected\n";
    return true;
}

string TcpClient::receive_data(int size = 512)
{
    char buffer[size];
    const int flags = 0;
    string received_message = "";
    size_t bytes_received = 0;
    do
    {
        bytes_received = ::recv(socket_fd, buffer, sizeof(buffer), flags);
        if (bytes_received < 0)
        {
            puts("recv failed");
            return received_message;
        }
        received_message += sub_array(buffer, 0, bytes_received);
        buffer[0] = 0; // clear buffer array
    } while (bytes_received >= size);
    // TODO: that should be moved to irc clinet!
    string endline_str = "\r\n";
    int rm_l = received_message.length();
    int el_l = endline_str.length();
    if(rm_l >= el_l && received_message.substr(rm_l - el_l, el_l).find(endline_str) != string::npos)
    {
        received_message = received_message.substr(0, rm_l - el_l);
    }

    return received_message;
}

// TODO: move this to external util class
string TcpClient::sub_array(char *array, int begin, size_t size)
{
    char buffer[size];
    int j = 0;
    for (int i = begin; i < begin + size; i++)
    {
        buffer[j++] = array[i];
    }

    return buffer;
}

bool TcpClient::send_data(string data)
{
    const char *data_cstr = data.c_str();
    const int str_len = strlen(data_cstr);
    const int flags = 0;

    ssize_t bytes_sent = ::send(socket_fd, data_cstr, str_len, flags);
    if (bytes_sent < 0)
    {
        string error_msg = "Send failed : " + data;
        perror(error_msg.c_str());
        return false;
    }
    return true;
}