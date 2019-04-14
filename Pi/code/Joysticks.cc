#include "Joysticks.h"

using namespace Leap;

void Joysticks::updateFrame(const Leap::Frame &frame) //copied from FingerButtons.cc
{
    // remove past hands so if a hand was moved out of the frame, we don't consider it
    leftHand = Hand::invalid();
    rightHand = Hand::invalid();
    
    // look through hands and set the appropriate variables.
    // if there are multiple left hands, for example, which one
    // is chosen is unspecified (hands() uses an arbitrary ordering)
    for (const Hand &h: frame.hands()) {
        if (h.isValid()) {
            if (h.isLeft()) {
                leftHand = h;
            }
            else if (h.isRight()) {
                rightHand = h;
            }
        }
    }
}

bool Joysticks::areHandsVisible() const //copied from FingerButtons.cc
{
    return leftHand.isValid() && rightHand.isValid();
}

bool Joysticks::isPressedDown(char handLetter) const
{
    Vector handCenter;
    Hand hand;
  
    if(handLetter == 'L')
    {
        if(!leftHand.isValid()) return false;
        
        hand = leftHand;
        handCenter = leftHand.palmPosition();
    }
    else if (handLetter == 'R') {
       if(!rightHand.isValid()) return false;
        
        hand = rightHand;
        handCenter = rightHand.palmPosition();
    }
    else {
        return false;
    }
  
    double currHeight = hand.getPalmY(handLetter);
  
    return currHeight <= config.handDepthSensitivity;
}

// checks palms for joystick button presses and calls appropriate functions
bool checkJoystickPresses() const;

// change sensitivity settings
void setSensitivity(const hsensitivity_t &hs);

// returns the x coordinate of the palm of the hand specified by handLetter
float getPalmX(char handLetter);

// returns the y coordinate of the palm of the hand specified by handLetter
float getPalmY(char handLetter);
