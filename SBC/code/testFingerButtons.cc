#include <iostream>
#include <fstream>
#include <cstring>
#include <unistd.h>
#include <vector>
#include <chrono>
#include <stdlib.h> // :( for system()

#include "Leap.h"
#include "FingerButtons.h"

using namespace std;
using namespace Leap;
static FingerButtons fButtons;

static char status[12] = "||||/ \\||||";

void resetStatus() {
    const char *cpy = "||||/ \\||||";
    for (int i = 0; i < 12; i++) {
        status[i] = cpy[i];
    }
}

void changeStatus(int fingerId) {
    if (fingerId >= 0 && fingerId < 5)
        status[fingerId] = '.';
    else if (fingerId >= 5 && fingerId < 10)
        status[fingerId+1] = '.';
}

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
   cout << "[Controller] Initialized" << endl;
}

void EventListener::onConnect(const Controller& controller) {
  cout << "[Controller] Connected" << endl;
}

void EventListener::onDisconnect(const Controller& controller) {
   // Note: not dispatched when running in a debugger.
   cout << "[Controller] Disconnected" << endl;
}

void EventListener::onExit(const Controller& controller) {
  cout << "[Controller] Exited" << endl;
}

void EventListener::onFrame(const Controller& controller) {
    // Get the most recent frame and tell FingerButtons about it
    const Frame frame = controller.frame();
    fButtons.updateFrame(frame);

    // call the appropriate handlers
    resetStatus();
    // this method will call appropriate event handlers. This isn't the only way to use this class:
    // you could instead just call the individual .isPressedDown(id) methods
    fButtons.checkButtonPresses();
    cout << status << endl;
}

void EventListener::onFocusGained(const Controller& controller) {
   cout << "[Controller] Focus Gained" << endl;
}

void EventListener::onFocusLost(const Controller& controller) {
   cout << "[Controller] Focus Lost" << endl;
}

void EventListener::onDeviceChange(const Controller& controller) {
  cout << "[Controller] Device Changed" << endl;
  const DeviceList devices = controller.devices();

  for (int i = 0; i < devices.count(); ++i) {
    cout << "[Controller] ID: " << devices[i].toString();
    cout << ", Streaming: " << (devices[i].isStreaming() ? "true" : "false") << endl;
  }
}

void EventListener::onServiceConnect(const Controller& controller) {
  cout << "[Controller] Service Connected" << endl;
}

void EventListener::onServiceDisconnect(const Controller& controller) {
  cout << "[Controller] Service Disconnected" << endl;
}

int main(int argc, const char* argv[])
{

  // initialize fButtons
  fButtons.setAllCallbacks(changeStatus);

  // set all sensitivies to 0.325
  fsensitivity_t s{0.45, 0.5, 0.5, 0.5, 0.5, 0.4, 0.35, 0.5, 0.6, 0.5};
  fButtons.setSensitivity(s);

  // Create a sample listener and controller
  EventListener listener;
  Controller controller;

  // Have the listener receive events from the controller
  controller.addListener(listener);

  bool running = true;
  while (running)
  {
    string command = "";
    cout << ">> ";
    cin >> command;
    if (command.compare("exit") == 0 || command.compare("quit") == 0 || command.compare("q") == 0)
    {
        running = false;
    }
    else
    {
      cout << "Unknown command!" << endl;
    }
  }
  cout << "[System] Exiting..." << endl;
  // Remove the sample listener when done
  controller.removeListener(listener);
  return 0;
}
