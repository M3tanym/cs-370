#ifndef FingerButtons_H_INCLUDED
#define FingerButtons_H_INCLUDED

#define NUM_FINGERS 10

#include <iostream>
#include <cstring>
#include "Leap.h"

typedef struct fingerConfig_s {

    // array of values representing sensitivites for each finger
    // each value should be between 0 (finger totally straight)
    // and 1 (finger entirely normal to palm, anatomically impossible)
    // elements 0-4 represent left pinky through left thumb, and
    // 5-9 represent right thumb through right pinky.
    double fingerSensitivities[NUM_FINGERS];

} fsensitivity_t;

class FingerButtons {
    public:
        // call each time you receive a new frame
        void updateFrame(const Leap::Frame &frame);

        // returns true if both hands are in the latest frame
        bool areHandsVisible() const;

        // returns true if finger fingerId is considered "down" depending on the fsensitivity settings
        bool isPressedDown(int fingerId) const;

        // checks all fingers for button presses and calls appropriate functions
        bool checkButtonPresses() const;

        // change sensitivity settings
        void setSensitivity(const fsensitivity_t &fs);

        // set the callback functions for each finger
        void setCallback(void (* fns [NUM_FINGERS])(int));
        
        // set all callbacks to be a single function (determine which finger it was by the parameter)
        void setAllCallbacks(void (*fn)(int));

        // I would write a method to get the callback methods so you can just set one, but I don't know how.
        // So I will instead provide this method, which allows you to modify the function pointer of finger id.
        void setCallback(void (*fn)(int), int id);

    private:
        // call Hand::isValid() to see if the hands are in the frame or not
        Leap::Hand leftHand, rightHand;

        fsensitivity_t config;

        // Callback functions for each finger
        // (each function is void and takes a single int parameter, the fingerId 0-9)
        void (* callbacks [NUM_FINGERS])(int);
};
#endif
