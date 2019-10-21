#include "Cappuccino/XinputManager.h"


Sedna::XinputController Sedna::XinputManager::controllers[4];
Sedna::XinputManager::XinputManager() {
	//sets the index at the index
	for (int index = 0; index < 4; index++) {
		controllers[index].setControllerIndex(index);
	}
}

bool Sedna::XinputManager::controllerConnected(int index) {
	//prevent those pesky programmers from going out of bounds
	if (index < 0 || index >= 4)
		return false;
	
	XINPUT_STATE connected;
	
	//return the populated xinput state if it was succesfully populated
	return XInputGetState(index,&connected) == ERROR_SUCCESS;
}

Sedna::XinputController * Sedna::XinputManager::getController(int index) {
	//if we're in index, return the controlelr at the index
	if(index >=0 && index < 4)
	return &controllers[index];
	
	return nullptr;
}

void Sedna::XinputManager::update() {
	//updates the controllers if they're plugged in
	for (int index = 0; index < 4; index++) 
		if (controllerConnected(index)) 
			controllers[index].update();
	

}

void Sedna::XinputController::setControllerIndex(int index) {
	this->index = index;
}

void Sedna::XinputController::update() {
	//updates the state of the controller
	if (index >= 0 && index < 4) 
		XInputGetState(index, &state);
}

void Sedna::XinputController::deadZoneSticks(float dz) {
	deadZoneStick = dz;
}

void Sedna::XinputController::deadZoneTriggers(float dz) {
	deadZoneTrigger = dz;
}


void Sedna::XinputController::updateSticks(Stick sticks[2]) {
	//LEFT STICK
	//the numbers that windows uses by default are very strange, so we divide these floats accordingly to get a value from -1 to 1
	float x = (float)state.Gamepad.sThumbLX / 32768,
		y = (float)state.Gamepad.sThumbLX / 32767;

	//if the length of the resultant vector is outside of the deadzone
	if (sqrt(x*x + y * y) > deadZoneStick) {

		//these statements just convert the number to a number between -1 and 1
		if (state.Gamepad.sThumbLX < 0)
			sticks[0].x = (float)state.Gamepad.sThumbLX / 32768;//convert to a float from 1 -> 1;
		else
			sticks[0].x = (float)state.Gamepad.sThumbLX / 32767;

		if (state.Gamepad.sThumbLY < 0)
			sticks[0].y = (float)state.Gamepad.sThumbLY / 32768;//convert to a float from 1 -> 1;
		else
			sticks[0].y = (float)state.Gamepad.sThumbLY / 32767;

	}
	else {

		sticks[0].x = 0.0f;
		sticks[0].y = 0.0f;

	}

	x = (float)state.Gamepad.sThumbRX / 32768,
	y = (float)state.Gamepad.sThumbRX / 32767;


	if (sqrt(x*x + y * y) > deadZoneStick) {
		//RIGHT STICK
		if (state.Gamepad.sThumbRX < 0)
			sticks[1].x = (float)state.Gamepad.sThumbRX / 32768;//conver to a float from -1 -> 1;
		else
			sticks[1].x = (float)state.Gamepad.sThumbRX / 32767;

		if (state.Gamepad.sThumbRY < 0)
			sticks[1].y = (float)state.Gamepad.sThumbRY / 32768;//conver to a float from -1 -> 1;
		else
			sticks[1].y = (float)state.Gamepad.sThumbRY / 32767;
	}

	else {

		sticks[1].x = 0.0f;
		sticks[1].y = 0.0f;

	}
}

void Sedna::XinputController::getTriggers(Triggers & triggers) {
	//get the converted float value of the triggers
	float l = (float)state.Gamepad.bLeftTrigger / 255, r = (float)state.Gamepad.bRightTrigger / 255;
	triggers = Triggers{ l < deadZoneTrigger ? 0 : l,r < deadZoneTrigger ? 0 : r };//converts byte to a number from 0 to 1
}

bool Sedna::XinputController::isButtonPressed(int button) {
	return button & state.Gamepad.wButtons;//bitwise &, checks if button and wButtons are equal in terms of state of the memory
}

bool Sedna::XinputController::isButtonReleased(int button) {
	return !isButtonPressed(button);
}

void Sedna::XinputController::setVibration(float left, float right) {
	///https://lcmccauley.wordpress.com/tag/xinput-vibration/

	//reserve the memory for the vibration
	ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));

	//converts the motor speed to a nuber between 0 and 1
	vibration.wLeftMotorSpeed = int(left*65535.0f);
	vibration.wRightMotorSpeed = int(right*65535.0f);

	XInputSetState(index, &vibration);
}

bool Sedna::XinputController::isVibrating() {
	if (vibration.wLeftMotorSpeed > 0.0f || vibration.wRightMotorSpeed > 0.0f)
		return true;
	return false;
}
