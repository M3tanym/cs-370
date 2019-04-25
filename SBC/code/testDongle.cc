#include "Dongle.h"
#include "USBController.h"
#include <iostream>
#include <string>
#include <unistd.h>
#include <signal.h>

using namespace std;

constexpr int PORT = 2342; // Arbitarily chosen value (> 1024)

bool running = true;

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
		return 1;
	}
	int controllerID = atoi(argv[1]), packetDelay = atoi(argv[2]);

	signal(SIGINT, catchIntr); // catch ^C with the catchIntr() function

	cout << "Connecting to controller " << controllerID << "...\n";
	USBController joy(controllerID);

	cout << "Opening UDP Port " << PORT << "...\n";
	Dongle dongle(PORT, packetDelay);

	cout << "Waiting for client...\n";
	while(running && !dongle.waitForClient()); // wait until client sends a packet

	if(running) cout << "Client Identified! (" << dongle.getClientIP() << ")\nSending data in " << packetDelay << "ms intervals...\n";

	while(running) // Runs until ^C
	{
		joystickState js;
		js.buttonA = joy.buttonA;
		js.buttonB = joy.buttonB;
		js.buttonX = joy.buttonX;
		js.buttonY = joy.buttonY;
		js.buttonBack = joy.buttonBack;
		js.buttonGuide = joy.buttonGuide;
		js.buttonStart = joy.buttonStart;
		js.buttonLeftStick = joy.buttonLeftStick;
		js.buttonRightStick = joy.buttonRightStick;
		js.buttonLeftBumper = joy.buttonLeftBumper;
		js.buttonRightBumper = joy.buttonRightBumper;
		js.buttonDUp = joy.buttonDUp;
		js.buttonDDown = joy.buttonDDown;
		js.buttonDLeft = joy.buttonDLeft;
		js.buttonDRight = joy.buttonDRight;
		js.leftStickX = joy.leftStickX;
		js.leftStickY = joy.leftStickY;
		js.leftTrigger = joy.leftRotation;
		js.rightStickX = joy.rightStickX;
		js.rightStickY = joy.rightStickY;
		js.rightTrigger = joy.rightRotation;
		dongle.update(js);
	}

	joy.close();
	cout << "Done!\n";
	return 0;
}
