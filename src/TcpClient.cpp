#include "TcpClient.h"

TcpClient::TcpClient(string address, int port)
{
    this->address = address;
    this->port = port;
}

bool TcpClient::connect()
{
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        show_error("Could not create socket");
        return false;
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
        return false;
    }

    cout << "Connected\n";
    return true;
}

//Size of each chunk of data received, try changing this
#define CHUNK_SIZE 512

/*
    Receive data in multiple chunks by checking a non-blocking socket
    Timeout in seconds
*/
string TcpClient::recv_timeout(int s , int timeout)
{
    int size_recv , total_size= 0;
    struct timeval begin , now;
    char chunk[CHUNK_SIZE];
    double timediff;

    string toReturn = "";
     
    //make socket non blocking
    fcntl(s, F_SETFL, O_NONBLOCK);
     
    //beginning time
    gettimeofday(&begin , NULL);
     
    while(1)
    {
        gettimeofday(&now , NULL);
         
        //time elapsed in seconds
        timediff = (now.tv_sec - begin.tv_sec) + 1e-6 * (now.tv_usec - begin.tv_usec);
         
        //if you got some data, then break after timeout
        if( total_size > 0 && timediff > timeout )
        {
            break;
        }
         
        //if you got no data at all, wait a little longer, twice the timeout
        else if( timediff > timeout*2)
        {
            break;
        }
         
        memset(chunk ,0 , CHUNK_SIZE);  //clear the variable
        if((size_recv =  recv(s , chunk , CHUNK_SIZE , 0) ) < 0)
        {
            //if nothing was received then we want to wait a little before trying again, 0.1 seconds
            usleep(100000);
        }
        else
        {
            total_size += size_recv;
            toReturn += chunk;
            //reset beginning time
            gettimeofday(&begin , NULL);
        }
    }
     
    return toReturn;
}

string TcpClient::receive_data(int size = 512)
{

    return recv_timeout(sock, 4);
    // cout << "receive_data()" << endl;
    // char buffer[size];
    // string reply;

    // //Receive a reply from the server
    // if (recv(sock, buffer, sizeof(buffer), 0) < 0)
    // {
    //     puts("recv failed");
    // }

    // reply = buffer;
    // return reply;
}

bool TcpClient::send_data(string data)
{
    //Send some data
    if (send(sock, data.c_str(), strlen(data.c_str()), 0) < 0)
    {
        perror("Send failed : ");
        return false;
    }
    cout << "Data send\n";

    return true;
}

void TcpClient::show_error(const char *msg)
{
    cout << msg << endl;
}