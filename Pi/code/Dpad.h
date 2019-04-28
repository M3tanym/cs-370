#ifndef DPAD_H_INCLUDED
#define DPAD_H_INCLUDED
#include "Leap.h"

typedef struct handConfigDpad_s {
	//for each hand, there are four possible positions (positive and negative pitch, positive and negative roll for each)
    double handPositions[8];
}handSensitivitiesDpad_t;

class Dpad {
public:
	
	//type Hands
	enum Hands { leftHand, rightHand };
		
	// call each time you receive a new frame
	void updateFrame(const Leap::Frame& frame);
	
	// returns true if both hands are in the latest frame
        bool areHandsVisible() const;
	
	// returns true if hand is considered "down" depending on the handSensitivitiesDpad_t settings
        //bool isPressedDown(Hands h) const;

        // change sensitivity settings
        void setSensitivity(const handSensitivitiesDpad_t &hs);

	//returns true if d-pad pressed up
	bool up(Hands h) const;

	//returns true if d-pad pressed down
	bool down(Hands h) const;

	//returns true if d-pad pressed left
	bool left(Hands h) const;

	//returns true if d-pad pressed right
	bool right(Hands h) const;
private:
	const Leap::Hand& whichHand(Hands h) const;
	Leap::Hand leftH, rightH;
	handSensitiviesDpad_t config;

};
#endif
