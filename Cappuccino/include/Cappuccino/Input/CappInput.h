#pragma once

#include "Cappuccino/Input/CappEvents.h"
#include "Cappuccino/Input/XinputManager.h"


#include <optional>

namespace Cappuccino {
	class Keyboard {
	public:

		
		bool keyPressed(KeyEvent key);
		bool keyReleased(KeyEvent key);
	};

	class ClickListener {
	public:

		void checkClick(int button, int action, int mods);

		bool leftClicked()  const { return _mouseLeftClicked; }
		bool rightClicked() const { return _mouseRightClicked; }
	private:
		bool _mouseLeftClicked = false;
		bool _mouseRightClicked = false;
		bool _mouseMiddleClicked = false;
	};

	class CappInput {
	public:
		/*
		Req: bool to create keyboard, proper controller index if you want to create a controller
		*/
		CappInput(bool createKeyboard, const std::optional<unsigned>& controllerIndex);

		void update();

		ClickListener clickListener;
		Keyboard* keyboard = nullptr;
		Sedna::XInputController* controller = nullptr;
		Sedna::Stick sticks[2];
		Sedna::Triggers triggers;
		//add in sticks and triggers when needed


	};
}