    
#ifndef DPAD_H_INCLUDED
#define DPAD_H_INCLUDED

#include "Leap.h"

class Dpad {
    public:
        // call each time you receive a new frame
        void updateFrame(const Leap::Frame &frame);   

        //returns true if d-pad pressed up
        bool up(const Hand &hand) const;
        
        //returns true if d-pad pressed down
        bool down(const Hand &hand) const;
        
        //returns true if d-pad pressed left
        bool left(const Hand &hand) const;
        
        //returns true if d-pad pressed right
        bool right(const Hand &hand) const;
    
    //private:  
        enum hands{0, 1};
        
};
#endif
