#ifndef DPAD_H_INCLUDED
#define DPAD_H_INCLUDED
#include "Leap.h"

typedef struct handConfigDpad_s {
	float pitchMin;
  float rollMin;
  float pitchMax;
  float rollMax;
}dpadsens_t;

class Dpad {
public:
	
	//type Hands
	enum Hands { leftHand, rightHand };
		
	// call each time you receive a new frame
	void updateFrame(const Leap::Frame& frame);
	
	// returns true if both hands are in the latest frame
  bool areHandsVisible() const;

  // change sensitivity settings
  void setSensitivity(const dpadsens_t &hs);

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
	dpadsens_t config;
};
#endif
