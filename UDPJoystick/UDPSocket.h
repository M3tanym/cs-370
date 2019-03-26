#ifndef UDPSocket_INCLUDED
#define UDPSocket_INCLUDED

#include <sys/socket.h>
#include <netinet/in.h>
#include <string>

class UDPSocket
{
  public:
    UDPSocket(int port);
    ~UDPSocket();
    bool waitForClient();
    void sendData(const unsigned char *data, size_t length);
    std::string getClientIP();

  private:
    struct sockaddr_in hostAddr;
    struct sockaddr_in cliAddr;
    socklen_t addrLen;
    int fd;
};

#endif
