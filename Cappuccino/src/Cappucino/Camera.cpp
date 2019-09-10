#include "Cappuccino/Camera.h"
namespace Cappuccino {


	glm::mat4 Camera::whereAreWeLooking() const {
		return glm::lookAt(_cameraPos, _cameraPos + _cameraFront, _cameraUp);
	}

	void Camera::doMouseMovement(float xoffset,float yoffset) {
		xoffset *= _mouseSensitivity;
		yoffset *= _mouseSensitivity;

		_yaw += xoffset;
		_pitch += yoffset;

		if (_pitch > 89.0f)
			_pitch = 89.0f;
		if (_pitch < -89.0f)
			_pitch = -89.0f;
		update();
	}

	void Camera::move(GLFWwindow* window,float _movementSpeed) {
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
			_cameraPos += _movementSpeed * _cameraFront;
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
			_cameraPos -= _movementSpeed * _cameraFront;
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
			_cameraPos -= glm::normalize(glm::cross(_cameraFront, _cameraUp)) * _movementSpeed;
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
			_cameraPos += glm::normalize(glm::cross(_cameraFront, _cameraUp)) * _movementSpeed;
	}

	void Camera::update() {
		glm::vec3 _front;
		_front.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
		_front.y = sin(glm::radians(_pitch));
		_front.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));
		_cameraFront = glm::normalize(_front);

		_cameraRight = glm::normalize(glm::cross(_cameraFront, _cameraUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		//_cameraUp = glm::normalize(glm::cross(cameraRight, _cameraFront));
	}


}
