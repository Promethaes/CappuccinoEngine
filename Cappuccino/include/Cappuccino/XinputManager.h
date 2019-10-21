#pragma once
#include <Windows.h>
#include <Xinput.h>
#include <cmath>

//special command that needs to be used in order to include the xinput library
#pragma comment(lib,"Xinput.lib")

namespace Sedna {
	//Buttons used for the controllers
	enum CONTROLLER_INPUT_BUTTONS
	{
		DPAD_UP = XINPUT_GAMEPAD_DPAD_UP,
		DPAD_DOWN = XINPUT_GAMEPAD_DPAD_DOWN,
		DPAD_LEFT = XINPUT_GAMEPAD_DPAD_LEFT,
		DPAD_RIGHT = XINPUT_GAMEPAD_DPAD_RIGHT,
		A = XINPUT_GAMEPAD_A,
		B = XINPUT_GAMEPAD_B,
		X = XINPUT_GAMEPAD_X,
		Y = XINPUT_GAMEPAD_Y,
		LB = XINPUT_GAMEPAD_LEFT_SHOULDER,
		RB = XINPUT_GAMEPAD_RIGHT_SHOULDER,
		THUMB_LEFT = XINPUT_GAMEPAD_LEFT_THUMB,
		THUMB_RIGHT = XINPUT_GAMEPAD_RIGHT_THUMB,
		SELECT = XINPUT_GAMEPAD_BACK,
		START = XINPUT_GAMEPAD_START
	};

	enum STICK_NAMES {
		LS,
		RS
	};

	enum TRIGGER_NAMES {
		LT,
		RT
	};

	//The X & Y input of the controller given as a float value from -1 -> 1
	struct Stick {
		float x, y;
	};

	//The left and right trigger values that are  given as a float value from 0 -> 1
	struct Triggers {
		float LT, RT;
	};

	/*
	Desc: Sets and checks all data use for the controller
	*/
	class XinputController {
	public:

		//sets the controller number to whatever number you pass in, such as changing player 2 to player 1
		void setControllerIndex(int index);

		//updates the controller state
		void update();

		//set the deadzone for the sticks
		void deadZoneSticks(float dz);

		//set the deadzone for the triggers
		void deadZoneTriggers(float dz);

		//populates the sticks of the controller, and also updates them
		void updateSticks(Stick sticks[2]);

		//updates the triggers
		void getTriggers(Triggers& triggers);

		//pass in a button from the enum list, this function will check if its pressed
		bool isButtonPressed(int button);

		//checks if a button is released
		bool isButtonReleased(int button);

		//sets the vibration speed of the motors in the controller...neat!
		void setVibration(float left, float right);

		//checks to see if the controller is vibrating
		bool isVibrating();

	private:

		//xinput state that deals with vibration
		XINPUT_VIBRATION vibration;

		//this is here so that if we initialize the controller improperly the program doesn't run
		int index = -1;

		//state that handles all of the xinput related things except for vibration
		XINPUT_STATE state;

		//floats that keep track of the deadzones for sticks and triggers
		float deadZoneStick, deadZoneTrigger;
	};

	/*
	Desc:
	*/
	class XinputManager {
	public:

		XinputManager();

		//checks to see if the controller at the index passed in is connected
		static bool controllerConnected(int index);

		//gets the controller at the index passed in
		static XinputController* getController(int index);

		//updates all the controllers
		static void update();
	private:

		//setting controller to an index for player
		static XinputController controllers[4];
	};
}