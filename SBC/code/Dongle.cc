#include "Dongle.h"
#include <stdexcept>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

Dongle::Dongle(int port, size_t packetDelay) : packetDelay(packetDelay)
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
		throw std::runtime_error("Couldn't bind socket!");
}

Dongle::~Dongle()
{
	close(fd);
}

std::string Dongle::getClientIP()
{
	char ip[INET6_ADDRSTRLEN];
  inet_ntop(AF_INET, &cliAddr.sin_addr, ip, INET6_ADDRSTRLEN);
	return ip;
}

bool Dongle::waitForClient()
{
	char buf[1];
	int recvLen = recvfrom(fd, buf, sizeof(buf), 0, (struct sockaddr *)&cliAddr, &addrLen);
	if(recvLen > 0)
		return true;
	return false;
}

void Dongle::update(joystickState js)
{
	// Put the button values into bytes
	unsigned char by1 = 0, by2 = 0;
	by1 = (js.buttonA | js.buttonB << 1 | js.buttonX << 2 | js.buttonY << 3);
	by1 = (by1 | js.buttonBack << 4 | js.buttonGuide << 5 | js.buttonStart << 6 | js.buttonLeftStick << 7);
	by2 = (js.buttonRightStick | js.buttonLeftBumper << 1 | js.buttonRightBumper << 2 | js.buttonDUp << 3);
	by2 = (by2 | js.buttonDDown << 4 | js.buttonDLeft << 5 | js.buttonDRight << 6);

	unsigned char lY = 255 - js.leftStickY;
	unsigned char rY = 255 - js.rightStickY;
	unsigned char b[8];

	// Build the char array to send
	b[0] = js.leftStickX;
	b[1] = static_cast<unsigned char>(lY);
	b[2] = static_cast<unsigned char>(js.leftTrigger);
	b[3] = static_cast<unsigned char>(js.rightStickX);
	b[4] = static_cast<unsigned char>(rY);
	b[5] = static_cast<unsigned char>(js.rightTrigger);
	b[6] = by1;
	b[7] = by2;

	sendData(b, 8);
	usleep(packetDelay * 1000); // sleep to avoid overwhelming the dongle
}

void Dongle::sendData(const unsigned char *data, size_t length)
{
	sendto(fd, data, length, 0, (struct sockaddr *)&cliAddr, addrLen);
}
