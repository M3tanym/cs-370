#ifndef Joysticks_H_INCLUDED
#define Joysticks_H_INCLUDED

#include <iostream>
#include "Leap.h"

typedef struct handConfig_s {
    // diameter, in mm, of the circular area around the center of the joystick that does not respond to movement
    double joystickDeadzone;
    
    // height, in mm, that a hand must go below in order for a joystick button to be clicked
    double handDepthSensitivity;
    
    // array of values representing the amount, in mm, to offset the palm position of each hand
    // elements in the array are as follows:
    //      0 : left hand x offset
    //      1 : left hand z offset
    //      2 : right hand x offset
    //      3 : right hand z offset
    double palmOffsets[4];

} hsensitivity_t;

class Joysticks
{
    public:
        // call each time you receive a new frame
        void updateFrame(const Leap::Frame &frame);

        // returns true if both hands are in the latest frame
        bool areHandsVisible() const;

        // returns true if hand handLetter is considered "down" depending on the hsensitivity settings
        bool isPressedDown(char handLetter) const;

        // change sensitivity settings
        void setSensitivity(const hsensitivity_t &hs);
               
        // returns the coordinate of the palm of the hand specified by handLetter and on the axis given
        float getPalmCoord(char handLetter, char axis) const;
    
    private:
    // call Hand::isValid() to see if the hands are in the frame or not
        Leap::Hand leftHand, rightHand;

        hsensitivity_t config;
};

#endif
