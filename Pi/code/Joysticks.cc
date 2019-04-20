#include "Joysticks.h"
#include <string>

using namespace std;
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
    if(handLetter == 'L' || handLetter == 'l')
    {
        if(!leftHand.isValid()) return false;
    }
    else if (handLetter == 'R' || handLetter == 'r') {
       if(!rightHand.isValid()) return false;
    }
    else {
        return false;
    }
  
    double currHeight = getPalmCoord(handLetter, 'Y');
  
    return currHeight <= config.handDepthSensitivity;
}

void Joysticks::setSensitivity(const hsensitivity_t &hs)
{
    config = hs;
}

//float Joysticks::getJoystickVal(char handLetter, char axis, double palmCoord) const
//{
//	double 
//	if (axis == 'X' || axis == 'x')
//}
 
float Joysticks::getPalmCoord(char handLetter, char axis) const
{
    Vector handPos;
    int intAxis;
	//double centerOffset;
	int offsetIndex = 0;
	double joystickVal;
    
    //determine what hand to look at
    if		(handLetter == 'L' || handLetter == 'l')    handPos = leftHand.palmPosition();
	else if (handLetter == 'R' || handLetter == 'r'){	handPos = rightHand.palmPosition(); offsetIndex += 2;}
    else throw ("Error: could not determine hand from given hand letter: "s + handLetter);
    
    //determine what axis to access in the vector
    if      (axis == 'X' || axis == 'x')     intAxis = 0;
	else if (axis == 'Y' || axis == 'y') {   intAxis = 1; return handPos[intAxis]; }
	else if (axis == 'Z' || axis == 'z') {	 intAxis = 2; offsetIndex += 1; }
    else throw ("Error: could not determine axis from given letter: "s + axis);
    

	//if (axis == 'X' || axis == 'x')
	//{
	//	if ((handLetter == 'R' || handLetter == 'r') && (handPos[intAxis] < 0)) // is the right hand x negative?
	//		return 128.0;
	//	if ((handLetter == 'L' || handLetter == 'l') && (handPos[intAxis] > 0)) // is the left hand x positive?
	//		return 128.0;
	//}

	joystickVal = handPos[intAxis] - config.palmOffsets[offsetIndex];

	if (joystickVal > 255) joystickVal = 255;
	else if (joystickVal < 0) joystickVal = 0;

    return joystickVal;
}

