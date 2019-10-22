#include "Cappuccino/CappInput.h"
#include "Cappuccino/Application.h"

namespace Cappuccino {
	bool Keyboard::keyPressed(unsigned char key)
	{
		return isEvent(key);
	}

	CappInput::CappInput(bool createKeyboard, const std::optional<unsigned>& controllerIndex)
	{
		if (createKeyboard)
			keyboard = new Keyboard();
		if (controllerIndex.has_value())
			controller = Sedna::XInputManager::getController(controllerIndex.value());
	}
}