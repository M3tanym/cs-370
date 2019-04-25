#include "USBController.h"
#include "SDL/SDL.h"
#include <stdexcept>
#include <thread>

USBController::USBController(int desiredID): id(desiredID)
{
	if(SDL_Init(SDL_INIT_JOYSTICK) < 0)
		throw std::runtime_error("Couldn't initialize!");
	if(SDL_NumJoysticks() < 1)
		throw std::runtime_error("No controllers connected!");

	gGameController = SDL_JoystickOpen(id);
	if(gGameController == NULL)
		throw std::runtime_error("Couldn't open controller!");

	running = true;
	t = std::thread(&USBController::update, this);
}

USBController::~USBController()
{
	SDL_JoystickClose(gGameController);
	gGameController = NULL;
	SDL_Quit();
}

void USBController::close()
{
	running = false;
	t.join();
}

void USBController::update()
{
	while(running)
	{
		SDL_Event e;
		if(SDL_PollEvent(&e) != 0)
		{
			if(e.type == SDL_JOYAXISMOTION)
			{
				if(e.jaxis.which == id)
				{
					int v = e.jaxis.value / 256 + 128;
					if(e.jaxis.axis == 0)
						leftStickX = v;
					else if(e.jaxis.axis == 1)
						leftStickY = v;
					else if(e.jaxis.axis == 2)
						rightStickX = v;
					else if(e.jaxis.axis == 3)
						rightStickY = v;
					else if(e.jaxis.axis == 4)
						leftRotation = v;
					else if(e.jaxis.axis == 5)
						rightRotation = v;
				}
			}
			else if(e.type == SDL_JOYBUTTONDOWN || SDL_JOYBUTTONUP)
			{
				int v = e.jbutton.state;
				if(e.jbutton.button == 0)
					buttonA = v;
				else if(e.jbutton.button == 1)
					buttonB = v;
				else if(e.jbutton.button == 2)
					buttonX = v;
				else if(e.jbutton.button == 3)
					buttonY = v;
				else if(e.jbutton.button == 4)
					buttonBack = v;
				else if(e.jbutton.button == 5)
					buttonGuide = v;
				else if(e.jbutton.button == 6)
					buttonStart = v;
				else if(e.jbutton.button == 7)
					buttonLeftStick = v;
				else if(e.jbutton.button == 8)
					buttonRightStick = v;
				else if(e.jbutton.button == 9)
					buttonLeftBumper = v;
				else if(e.jbutton.button == 10)
					buttonRightBumper = v;
				else if(e.jbutton.button == 11)
					buttonDUp = v;
				else if(e.jbutton.button == 12)
					buttonDDown = v;
				else if(e.jbutton.button == 13)
					buttonDLeft = v;
				else if(e.jbutton.button == 14)
					buttonDRight = v;
			}
		}
	}
}
