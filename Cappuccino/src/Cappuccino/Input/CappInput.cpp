#include "Cappuccino/Input/CappInput.h"

#include "Cappuccino/Core/Application.h"

#include <glfw/glfw3.h>

namespace Cappuccino {

	bool Keyboard::keyPressed(KeyEvent key) {
		return glfwGetKey(Application::window, static_cast<int>(key)) == GLFW_PRESS;
	}

	bool Keyboard::keyReleased(KeyEvent key) {
		return glfwGetKey(Application::window, static_cast<int>(key)) == GLFW_RELEASE;
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

		if(button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS)
			_mouseMiddleClicked = true;
		else if(button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_RELEASE)
			_mouseMiddleClicked = false;

	}
}