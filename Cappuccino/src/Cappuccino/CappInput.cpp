#include "Cappuccino/CappInput.h"
#include "Cappuccino/Application.h"
#include "glfw/glfw3.h"

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
	void CappInput::update()
	{
		if (controller) {
			controller->getTriggers(triggers);
			controller->updateSticks(sticks);

		}
	}
	void ClickListener::checkClick(int button, int action, int mods)
	{

		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
			_mouseLeftClicked = true;
		else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
			_mouseLeftClicked = false;

		if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
			_mouseRightClicked = true;
		else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
			_mouseRightClicked = false;

	}
}