#include "Cappuccino/Input.h"
#include <Windows.h>
//All of this code is from a previous GDW game and Game Programming Club in one of last semester's sessions.

bool isEvent(unsigned char event) {
	//windows code for the state of a key
	return GetAsyncKeyState(event);
}
