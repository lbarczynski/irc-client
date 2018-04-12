#include "TcpClient.h"

using namespace std;

class IrcClient
{
public:
  IrcClient(string username, string address, int port);
  void connect();
  void join_channel(string channel_name);

private:
  string username;
  string channel;
  TcpClient *tcp_client;
  bool send_cmd(string cmd, string message);
};