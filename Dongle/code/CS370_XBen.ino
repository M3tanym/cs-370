// XBen Controller
#include <XInput.h>

#define BAUD 57600 // BAUD is bitrate

void setup()
{
  Serial1.begin(BAUD); // ESP Serial line
  XInput.setTriggerRange(0, 255);
  XInput.setJoystickRange(0, 255);
  XInput.begin();
}

void loop()
{
  // Wait for the serial header "!<"
  bool wait = true;
  while(wait)
  {
    while(Serial1.available() < 1);
    if(Serial1.read() != '!') continue;
    while(Serial1.available() < 1);
    if(Serial1.read() != '<') continue;
    wait = false;
  }

  // At this point, we know this is a valid Serial frame
  // Read the next 12 bytes
  const int packetSize = 12;
  unsigned char b[packetSize];
  for(int i = 0; i < packetSize; i++)
  {
    while(Serial1.available() < 1);
    b[i] = Serial1.read();
  }
  
  // Retrieve the controls from the bytes
  uint16_t leftStickX = b[0], leftStickY = b[1], leftRotation = b[2];
  uint16_t rightStickX = b[3], rightStickY = b[4], rightRotation = b[5];
  uint16_t buttonA = (b[6] & 0b1), buttonB = (b[6] & 0b10) >> 1, buttonX = (b[6] & 0b100) >> 2, buttonY = (b[6] & 0b1000) >> 3;
  uint16_t buttonBack = (b[6] & 0b10000) >> 4, buttonGuide = (b[6] & 0b100000) >> 5, buttonStart = (b[6] & 0b1000000) >> 6;
  uint16_t buttonLeftStick = (b[6] & 0b10000000) >> 7, buttonRightStick = (b[7] & 0b1);
  uint16_t buttonLeftBumper = (b[7] & 0b10) >> 1, buttonRightBumper = (b[7] & 0b100) >> 2;
  uint16_t buttonDUp = (b[7] & 0b1000) >> 3, buttonDDown = (b[7] & 0b10000) >> 4, buttonDLeft = (b[7] & 0b100000) >> 5, buttonDRight = (b[7] & 0b1000000) >> 6;

  // Set the controller values
  XInput.setJoystick(JOY_LEFT, leftStickX, leftStickY);
  XInput.setJoystick(JOY_RIGHT, rightStickX, rightStickY);
  XInput.setTrigger(TRIGGER_LEFT, leftRotation);
  XInput.setTrigger(TRIGGER_RIGHT, rightRotation);

  XInput.setDpad(buttonDUp, buttonDDown, buttonDLeft, buttonDRight);
  
  XInput.setButton(BUTTON_A, buttonA);
  XInput.setButton(BUTTON_B, buttonB);
  XInput.setButton(BUTTON_X, buttonX);
  XInput.setButton(BUTTON_Y, buttonY);

  XInput.setButton(BUTTON_LB, buttonLeftBumper);
  XInput.setButton(BUTTON_RB, buttonRightBumper);

  XInput.setButton(BUTTON_L3, buttonLeftStick);
  XInput.setButton(BUTTON_R3, buttonRightStick);
  
  XInput.setButton(BUTTON_BACK, buttonBack);
  XInput.setButton(BUTTON_LOGO, buttonGuide);
  XInput.setButton(BUTTON_START, buttonStart);
}
