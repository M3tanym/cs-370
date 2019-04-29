#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <cstring>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

#include "Leap.h"
#include "Dongle.h"
#include "FingerButtons.h"
#include "Joysticks.h"
#include "Dpad.h"

using namespace std;
using namespace Leap;

/******************** GLOBAL VARIABLES ********************/

constexpr int DEBUG = 0; // Debug level flag (0: off, 1: few, 2: more, 3: all)
constexpr int PORT = 2342; // Arbitarily chosen value (> 1024). Must also be set in ESP code
constexpr int PACKETDELAY = 15; // Empirically obtained optimal delay time (ms)
static bool running = true; // running flag
static FingerButtons fButtons;
static char fingerState[12] = "||||/ \\||||";
static Joysticks joysticks;


/******************** VARIOUS FUNCTIONS ********************/

void catchIntr(int)
{
	cout << "[System] Exiting...\n";
	running = false;
}

void resetStatus() {
	const char *cpy = "||||/ \\||||";
	for (int i = 0; i < 12; i++) {
		fingerState[i] = cpy[i];
	}
}

void changeStatus(int fingerId) {
	if (fingerId >= 0 && fingerId < 5)
		fingerState[fingerId] = '.';
	else if (fingerId >= 5 && fingerId < 10)
		fingerState[fingerId+1] = '.';
}

/******************** EVENT LISTENER ********************/

class EventListener : public Listener {
  public:
	  virtual void onInit(const Controller&);
	  virtual void onConnect(const Controller&);
	  virtual void onDisconnect(const Controller&);
	  virtual void onExit(const Controller&);
	  virtual void onFrame(const Controller&);
	  virtual void onFocusGained(const Controller&);
	  virtual void onFocusLost(const Controller&);
	  virtual void onDeviceChange(const Controller&);
	  virtual void onServiceConnect(const Controller&);
	  virtual void onServiceDisconnect(const Controller&);
};

void EventListener::onInit(const Controller& controller) {
   if(DEBUG > 0) cout << "[Controller] Initialized" << endl;
}

void EventListener::onConnect(const Controller& controller) {
  if(DEBUG > 1) cout << "[Controller] Connected" << endl;
}

void EventListener::onDisconnect(const Controller& controller) {
   // Note: not dispatched when running in a debugger.
   if(DEBUG > 1) cout << "[Controller] Disconnected" << endl;
}

void EventListener::onExit(const Controller& controller) {
  if(DEBUG > 1) cout << "[Controller] Exited" << endl;
}

void EventListener::onFrame(const Controller& controller) {
	// Get the most recent frame and tell FingerButtons about it
	const Frame frame = controller.frame();
	fButtons.updateFrame(frame);
	joysticks.updateFrame(frame);

	// call the appropriate handlers
	resetStatus();
	fButtons.checkButtonPresses();
	// cout << fingerState << endl;
}

void EventListener::onFocusGained(const Controller& controller) {
   if(DEBUG > 1) cout << "[Controller] Focus Gained" << endl;
}

void EventListener::onFocusLost(const Controller& controller) {
   if(DEBUG > 1) cout << "[Controller] Focus Lost" << endl;
}

void EventListener::onDeviceChange(const Controller& controller) {
  if(DEBUG > 1) cout << "[Controller] Device Changed" << endl;
  const DeviceList devices = controller.devices();

  for (int i = 0; i < devices.count(); ++i) {
    if(DEBUG > 1) cout << "[Controller] ID: " << devices[i].toString();
    if(DEBUG > 1) cout << ", Streaming: " << (devices[i].isStreaming() ? "true" : "false") << endl;
  }
}

void EventListener::onServiceConnect(const Controller& controller) {
  if(DEBUG > 1) cout << "[Controller] Service Connected" << endl;
}

void EventListener::onServiceDisconnect(const Controller& controller) {
  if(DEBUG > 1) cout << "[Controller] Service Disconnected" << endl;
}

/******************** MAIN ********************/

int main(int argc, char **argv)
{
	cout << "[System] Running!\n";
	signal(SIGINT, catchIntr); // catch ^C with the catchIntr() function

	// Create a sample listener and controller
	EventListener listener;
	Controller controller;

	// Have the listener receive events from the controller
	controller.addListener(listener);

	// Open the UDP port
	Dongle dongle(PORT, PACKETDELAY);

	// Initialize fButtons
	fButtons.setAllCallbacks(changeStatus);

	// Set all sensitivies
	fsensitivity_t s{0.45, 0.5, 0.5, 0.5, 0.48, 0.38, 0.35, 0.5, 0.6, 0.5};
	fButtons.setSensitivity(s);

	hsensitivity_t jsens;
	jsens.joystickDeadzone = 30;
	jsens.handDepthSensitivity = 300;
	jsens.joystickSensitivity = 2;
	jsens.palmOffsets[0] = -300;  //      0 : left hand x offset
	jsens.palmOffsets[1] = 100;  //      1 : left hand y offset
	jsens.palmOffsets[2] = -128;  //      2 : left hand z offset
	jsens.palmOffsets[3] = 0;  //      3 : right hand x offset
	jsens.palmOffsets[4] = 100;  //      4 : right hand y offset
	jsens.palmOffsets[5] = -128; //      5 : right hand z offset
	joysticks.setSensitivity(jsens);

  // Connect to the dongle
	cout << "[System] Setup complete! Waiting for dongle...\n";
	while(running && !dongle.waitForClient()); // wait until client sends a packet

	if(running) cout << "[System] Dongle Identified! (" << dongle.getClientIP() << ")\n";

	while(running) // Runs until ^C
	{
		joystickState js;
		js.buttonA = fButtons.isPressedDown(5);
		js.buttonB = fButtons.isPressedDown(4);
		js.buttonX = fButtons.isPressedDown(6);
		js.buttonY = fButtons.isPressedDown(3);
		js.buttonBack = fButtons.isPressedDown(0);
		js.buttonGuide = 0;
		js.buttonStart = fButtons.isPressedDown(9);
		js.buttonLeftStick = fButtons.isPressedDown(1);
		js.buttonRightStick = fButtons.isPressedDown(8);
		js.buttonLeftBumper = fButtons.isPressedDown(2);
		js.buttonRightBumper = fButtons.isPressedDown(7);
		js.buttonDUp = 0;
		js.buttonDDown = 0;
		js.buttonDLeft = 0;
		js.buttonDRight = 0;
		js.leftStickX = joysticks.getPalmCoord('L', 'X');
		js.leftStickY = joysticks.getPalmCoord('L', 'Z');
		js.leftTrigger = 255 - joysticks.getPalmCoord('L', 'Y');
		js.rightStickX = joysticks.getPalmCoord('R', 'X');
		js.rightStickY = joysticks.getPalmCoord('R', 'Z');
		js.rightTrigger = 255 - joysticks.getPalmCoord('R', 'Y');

		dongle.update(js);
	}

  // Remove the sample listener when done
  controller.removeListener(listener);
	return 0;
}
