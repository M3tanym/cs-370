#ifndef Dongle_INCLUDED
#define Dongle_INCLUDED

#include <sys/socket.h>
#include <netinet/in.h>
#include <string>

typedef struct JoystickState
{
	bool buttonA, buttonB, buttonX, buttonY;
	bool buttonBack, buttonGuide, buttonStart;
	bool buttonLeftStick, buttonRightStick;
	bool buttonLeftBumper, buttonRightBumper;
	bool buttonDUp, buttonDDown, buttonDLeft, buttonDRight;
	unsigned char leftStickX, leftStickY, leftTrigger;
	unsigned char rightStickX, rightStickY, rightTrigger;
} joystickState;

class Dongle
{
  public:
    Dongle(int port, size_t packetDelay);
    ~Dongle();
    bool waitForClient();
    void update(joystickState js);
    void sendData(const unsigned char *data, size_t length);
    std::string getClientIP();
		size_t packetDelay;

  private:
    struct sockaddr_in hostAddr;
    struct sockaddr_in cliAddr;
    socklen_t addrLen;
    int fd;
};

#endif
