#ifndef Joysticks_H_INCLUDED
#define Joysticks_H_INCLUDED

#include "Leap.h"

typedef struct handConfig_s {
    //diameter, in mm, of the circular area around the center of the joystick that does not respond to movement
    double joystickDeadzone;
    //height, in mm, that a hand must go below in order for a joystick button to be clicked
    double handDepthSensitivity;

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
               
        // returns the x coordinate of the palm of the hand specified by handLetter
        float getPalmX(char handLetter);
        
        // returns the y coordinate of the palm of the hand specified by handLetter
        float getPalmY(char handLetter);
    
        // returns the z coordinate of the palm of the hand specified by handLetter
        float getPalmZ(char handLetter);
    
    private:
    // call Hand::isValid() to see if the hands are in the frame or not
        Leap::Hand leftHand, rightHand;

        hsensitivity_t config;
}

#endif
