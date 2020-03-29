#pragma once
#include "Events.h"

//function header
//all of this code is from a previous GDW game and Game Programming Club in one of last semester's sessions.

/*
Purp.: checks the inputted character and if it is a key event
Req.: unsigned char representing the windows key code
Returns: bool dependant on whether it is an event or not
*/
extern bool isEvent(unsigned char);
