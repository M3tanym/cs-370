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
  
    double currHeight = hand.getPalmCoord(handLetter, 'Y');
  
    return currHeight <= config.handDepthSensitivity;
}

void Joysticks::setSensitivity(const hsensitivity_t &hs)
{
    config = hs;
}

float Joysticks::getPalmCoord(char handLetter, char axis)
{
    Vector handCenter;
    int intAxis;
    
    //determine what hand to look at
    if      (handLetter == 'L' || handLetter == 'l')    handCenter = leftHand.palmPosition();
    else if (handLetter == 'R' || handLetter == 'r')    handCenter = rightHand.palmPosition();
    else throw ("Error: could not determine hand from given hand letter: "s + string(handLetter));
    
    //determine what axis to access in the vector
    if      (axis == 'X' || axis == 'x')     intAxis = 0;
    else if (axis == 'Y' || axis == 'y')     intAxis = 1;
    else if (axis == 'Z' || axis == 'z')     intAxis = 2;
    else throw ("Error: could not determine axis from given letter: "s + string(axis));
    
    return handCenter[intAxis];
}

