#include "UDPSocket.h"
#include <stdexcept>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

UDPSocket::UDPSocket(int port)
{
	if((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
		throw std::runtime_error("Couldn't create socket!");

	struct timeval read_timeout;
	read_timeout.tv_sec = 0;
	read_timeout.tv_usec = 100000;
	if(setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &read_timeout, sizeof(read_timeout)) < 0)
		throw std::runtime_error("Couldn't set socket options!");

	addrLen = sizeof(hostAddr);
	hostAddr.sin_family = AF_INET;
	hostAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	hostAddr.sin_port = htons(port);

	if(bind(fd, (struct sockaddr *)&hostAddr, sizeof(hostAddr)) < 0)
		throw std::runtime_error("Couldn't bind!");
}

UDPSocket::~UDPSocket()
{
	close(fd);
}

std::string UDPSocket::getClientIP()
{
	char ip[INET6_ADDRSTRLEN];
  inet_ntop(AF_INET, &cliAddr.sin_addr, ip, INET6_ADDRSTRLEN);
	return ip;
}

bool UDPSocket::waitForClient()
{
	char buf[1];
	int recvLen = recvfrom(fd, buf, sizeof(buf), 0, (struct sockaddr *)&cliAddr, &addrLen);
	if(recvLen > 0)
		return true;
	return false;
}

void UDPSocket::sendData(const unsigned char *data, size_t length)
{
	sendto(fd, data, length, 0, (struct sockaddr *)&cliAddr, addrLen);
}
