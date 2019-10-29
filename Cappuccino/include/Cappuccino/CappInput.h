#pragma once
#include "Cappuccino/Input.h"
#include "Cappuccino/Events.h"
#include "Cappuccino/XInputManager.h"

#include <optional>

namespace Cappuccino {
	class Keyboard {
	public:

		bool keyPressed(unsigned char);
	};

	class CappInput {
	public:
		/*
		Req: bool to create keyboard, proper controller index if you want to create a controller
		*/
		CappInput(bool createKeyboard, const std::optional<unsigned>& controllerIndex);

		void update();

		Keyboard* keyboard = nullptr;
		Sedna::XInputController* controller = nullptr;
		Sedna::Stick sticks[2];
		Sedna::Triggers triggers;
		//add in sticks and triggers when needed


	};
}