#include "UDPSocket.h"
#include "Joystick.h"
#include <iostream>
#include <string>
#include <unistd.h>
#include <signal.h>

using namespace std;

constexpr int PORT = 2342; // Arbitarily chosen value (> 1024)

static bool running = true; // sorry Jack

void catchIntr(int)
{
	cout << "Exiting...\n";
	running = false;
}

int main(int argc, char **argv)
{
	// Get arguments
	if(argc < 3)
	{
		cout << "Usage: " << argv[0] << " [Controller ID (0, 1, etc.)] [packetDelay (ms)]\n";
	}
	int controllerID = atoi(argv[1]), packetDelay = atoi(argv[2]);

	signal(SIGINT, catchIntr); // catch ^C with the catchIntr() function

	cout << "Connecting to controller " << controllerID << "...\n";
	Joystick joy(controllerID);

	cout << "Opening UDP Port " << PORT << "...\n";
	UDPSocket udp(PORT);

	cout << "Waiting for client...\n";
	while(running && !udp.waitForClient()); // wait until client sends a packet

	if(running) cout << "Client Identified! (" << udp.getClientIP() << ")\nSending data in " << packetDelay << "ms intervals...\n";

	while(running) // Runs until ^C
	{
		// Put the button values into bytes
    unsigned char by1 = 0, by2 = 0;
		by1 = (joy.buttonA | joy.buttonB << 1 | joy.buttonX << 2 | joy.buttonY << 3);
		by1 = (by1 | joy.buttonBack << 4 | joy.buttonGuide << 5 | joy.buttonStart << 6 | joy.buttonLeftStick << 7);
		by2 = (joy.buttonRightStick | joy.buttonLeftBumper << 1 | joy.buttonRightBumper << 2 | joy.buttonDUp << 3);
		by2 = (by2 | joy.buttonDDown << 4 | joy.buttonDLeft << 5 | joy.buttonDRight << 6);

		// Y axes seem to be inverted on my controller, invert them back
		unsigned char lY = 255 - joy.leftStickY;
		unsigned char rY = 255 - joy.rightStickY;
		unsigned char b[8];

		// Build the char array to send
		b[0] = joy.leftStickX;
		b[1] = static_cast<unsigned char>(lY);
		b[2] = static_cast<unsigned char>(joy.leftRotation);
		b[3] = static_cast<unsigned char>(joy.rightStickX);
		b[4] = static_cast<unsigned char>(rY);
		b[5] = static_cast<unsigned char>(joy.rightRotation);
		b[6] = by1;
		b[7] = by2;

		udp.sendData(b, 8); // send data to dongle (data, length)
		usleep(packetDelay * 1000); // sleep to avoid overwhelming the dongle
	}

	joy.close();
	cout << "Done!\n";
	return 0;
}
