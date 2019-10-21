#pragma once
#include "Cappuccino/Input.h"
#include "Cappuccino/Events.h"
#include "Cappuccino/XinputManager.h"

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

		

		Keyboard* keyboard = nullptr;
		Sedna::XinputController* controller = nullptr;
		//add in sticks and triggers when needed


	};
}