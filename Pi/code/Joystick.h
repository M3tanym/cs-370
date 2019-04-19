#ifndef Joystick_DEFINED
#define Joystick_DEFINED

#include "SDL/SDL.h"
#include <thread>

class Joystick
{
	public:
		Joystick(int desiredID = 0);
		~Joystick();
		void close();
		int leftStickX = 0, leftStickY = 0, leftRotation = 0;
		int rightStickX = 0, rightStickY = 0, rightRotation = 0;
		int buttonA = 0, buttonB = 0, buttonX = 0, buttonY = 0;
		int buttonBack = 0, buttonGuide = 0, buttonStart = 0;
		int buttonLeftStick = 0, buttonLeftBumper = 0;
		int buttonRightStick = 0, buttonRightBumper = 0;
		int buttonDUp = 0, buttonDDown = 0, buttonDLeft = 0, buttonDRight = 0;
	private:
		SDL_Joystick* gGameController;
		int id;
		bool running;
		std::thread t;
		void update();
};

#endif
