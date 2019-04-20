#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <cstring>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

#include "UDPJoystick.h"
#include "Leap.h"
#include "FingerButtons.h"
#include "Joysticks.h"


using namespace std;
using namespace Leap;

/******************** GLOBAL VARIABLES ********************/

constexpr int DEBUG = 0; // Debug level flag (0: off, 1: few, 2: more, 3: all)
constexpr int PORT = 2342; // Arbitarily chosen value (> 1024). Must also be set in ESP code
constexpr int PACKETDELAY = 30; // Empirically obtained optimal delay time (ms)
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
	UDPJoystick udp(PORT, PACKETDELAY);

	// Initialize fButtons
	fButtons.setAllCallbacks(changeStatus);

	// Set all sensitivies
	fsensitivity_t s;
	for (int i = 0; i < 10; i++) {
		s.fingerSensitivities[i] = 0.4;
	}
	fButtons.setSensitivity(s);
	
	hsensitivity_t s;
	s.joystickDeadzone = 30;
	s.handDepthSensitivity = 300;
	s.rangeOfMotionScalingFactor = 1;
	s.palmOffsets[0] = -300;  //      0 : left hand x offset
	s.palmOffsets[1] = -128;  //      1 : left hand z offset
	s.palmOffsets[2] = 0;  //      2 : right hand x offset
	s.palmOffsets[3] = -128;  //      3 : right hand z offset
	joysticks.setSensitivity(s);

  // Connect to the dongle
	cout << "[System] Setup complete! Waiting for dongle...\n";
	while(running && !udp.waitForClient()); // wait until client sends a packet

	if(running) cout << "[System] Dongle Identified! (" << udp.getClientIP() << ")\n";

	while(running) // Runs until ^C
	{
		joystickState js;
		// TODO Change these to real values
		js.buttonA = !(fingerState[0] == '|');
		js.buttonB = !(fingerState[1] == '|');
		js.buttonX = !(fingerState[2] == '|');
		js.buttonY = !(fingerState[3] == '|');
		js.buttonBack = 0;
		js.buttonGuide = 0;
		js.buttonStart = 0;
		js.buttonLeftStick = joysticks.isPressedDown('L');
		js.buttonRightStick = joysticks.isPressedDown('R');
		js.buttonLeftBumper = 0;
		js.buttonRightBumper = 0;
		js.buttonDUp = 0;
		js.buttonDDown = 0;
		js.buttonDLeft = 0;
		js.buttonDRight = 0;
		js.leftStickX = joysticks.getPalmCoord('L', 'X');
		js.leftStickY = joysticks.getPalmCoord('L', 'Y');
		js.leftTrigger = 0;
		js.rightStickX = joysticks.getPalmCoord('R', 'X');
		js.rightStickY = joysticks.getPalmCoord('R', 'Y');
		js.rightTrigger = 0;

		udp.update(js);
	}

  // Remove the sample listener when done
  controller.removeListener(listener);
	return 0;
}
