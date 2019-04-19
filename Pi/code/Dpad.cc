#include "Dpad.h"

using namespace Leap;

//need to implement edge cases (non-adjacent pitch and roll)

//update frame method--update hand tilt position
void Dpad::updateFrame(const Leap::Frame &frame) {
    //for each hand, update hand variables/tilt
    tilt = false;
     for (const Hand &h: frame.hands()) {
         if(h.isValid()){
              if(h.palmNormal().roll()!=0 || h.direction().pitch()!=0)
                    tilt = true;
              //if((up(h) == true && down(h) == true) || (right(h) == true && left(h) == true))
                    //tilt = false;
         }
}

//returns true if hand tilted "up" depending on pitch position (gets tilt around x axis)
//pitch returns negative value if in "up" direction (I think--need to test with leap to make sure)
bool Dpad::up(const Hand &hand) const {
    float pitch = hand.direction().pitch();
    
    if(pitch >= 0 && pitch <=180){
        return false;
    }
    else if(pitch <=0 && pitch >=-180){
        return true;
    }
    else {
        return false;
    }
}

//returns true if hand tilted "down" depending on pitch position (gets tilt around x axis)
//pitch returns positive value if in "down" direction (I think--need to test with leap to make sure)
bool Dpad::up(const Hand &hand) const {
    float pitch = hand.direction().pitch();
    
    if(pitch >= 0 && pitch <=180){
        return true;
    }
    else if(pitch <=0 && pitch >=-180){
        return false;
    }
    else {
        return false;
    }
}

//returns true if hand tilted "left" depending on roll position (gets tilt around z axis)
//roll returns negative value if in "left" direction (I think--need to test with leap to make sure)
bool Dpad::left(const Hand &hand) const {
    float roll = hand.palmNormal().roll();
    
    if(roll >= 0 && roll <=180){
        return false;
    }
    else if(roll <=0 && roll >=-180){
        return true;
    }
    else {
        return false;
    }
}


//returns true if hand tilted "right" depending on roll position (gets tilt around z axis)
//roll returns positive value if in "left" direction (I think--need to test with leap to make sure)
bool Dpad::right(const Hand &hand) const {
    float roll = hand.palmNormal().roll();
    
    if(roll >= 0 && roll <=180){
        return true;
    }
    else if(roll <=0 && roll >=-180){
        return false;
    }
    else {
        return false;
    }
}
