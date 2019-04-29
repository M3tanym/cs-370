
#include "FingerButtons.h"


using namespace Leap;

void FingerButtons::updateFrame(const Leap::Frame &frame) {
    
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


bool FingerButtons::areHandsVisible() const {
    return leftHand.isValid() && rightHand.isValid();
}

Finger::Type getFingerType(int id) {
    switch (id) {
        case 0:
        case 9:
            return Finger::Type::TYPE_PINKY;
        case 1:
        case 8:
            return Finger::Type::TYPE_RING;
        case 2:
        case 7:
            return Finger::Type::TYPE_MIDDLE;
        case 3:
        case 6:
            return Finger::Type::TYPE_INDEX;
        case 4:
        case 5:
            return Finger::Type::TYPE_THUMB;
        default:
            throw "Received invalid finger id";
    }
}

// returns true if finger fingerId is considered "down" depending on the fsensitivity settings
bool FingerButtons::isPressedDown(int fingerId) const {
    return getPressedDown(fingerId) >= config.fingerSensitivities[fingerId];
}
    
double FingerButtons::getPressedDown(int fingerId) const {

    Vector norm;
    Finger finger;
    
    if (fingerId >= 0 && fingerId < 5) {
        if (!leftHand.isValid()) return 0;
        
        norm = leftHand.palmNormal().normalized();
        
        // get all of the fingers of this type on the left hand
        FingerList fl = leftHand.fingers().fingerType(getFingerType(fingerId));
        
        // can it see it?
        if (fl.count() == 0) return 0;
        
        finger = fl[0];
    }
    else if (fingerId >= 5 && fingerId < 10) {
        if (!rightHand.isValid()) return 0;
        
        norm = rightHand.palmNormal().normalized();
        
        // get all of the fingers of this type on the rigth hand
        FingerList fl = rightHand.fingers().fingerType(getFingerType(fingerId));
        
        // can it see it?
        if (fl.count() == 0) return 0;
        
        finger = fl[0];
    }
    else {
        return 0;
    }
    
    
    if (!finger.isValid()) return 0;
    
    // get positions of the start and end of the finger
    Vector start = finger.bone(Bone::Type::TYPE_METACARPAL).prevJoint();
    Vector end   = finger.bone(Bone::Type::TYPE_DISTAL).nextJoint();
    
    // create a vector for this finger
    Vector fingerVec(end - start);
    
    // dot product with the vector normal to the palm will give a value representing
    // how parallel the finger vector is to the palm normal
    // by dividing by the magnitude, we get a value from 0 (fully extended) to 1 (directly orthogonal to the palm)
    double mag = fingerVec.dot(norm) / fingerVec.magnitude();
    
    // if this value is greater than the required sensitivity, the finger is down
    return mag;
}


bool FingerButtons::checkButtonPresses() const {
    bool called = false;
    for (int i = 0; i < NUM_FINGERS; i++) {
        if (isPressedDown(i) && callbacks[i] != nullptr) {
            // call the function associated with this finger
            callbacks[i](i);
            called = true;
        }
    }
    return called;
}


void FingerButtons::setSensitivity(const fsensitivity_t &fs) {
    config = fs;
}


void FingerButtons::setCallback(void (* fns [NUM_FINGERS])(int)) {
    for (int i = 0; i < NUM_FINGERS; i++) {
        setCallback(fns[i], i);
    }
}

void FingerButtons::setAllCallbacks(void (*fn)(int)) {
    for (int i = 0; i < NUM_FINGERS; i++) {
        setCallback(fn, i);
    }
}


void FingerButtons::setCallback(void (*fn)(int), int id) {
    callbacks[id] = fn;
}