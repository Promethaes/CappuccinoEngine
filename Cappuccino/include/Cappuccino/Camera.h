#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
namespace Cappuccino {
	/*
	Desc: Allows you to actually view the game, abstracts the camera into a class for easy duplication and editing
	*/
	class Camera {
	public:
		Camera() = default;

		/*
		Purp: returns the view matrix
		*/
		glm::mat4 whereAreWeLooking() const;

		void lookAt(const glm::vec3& lookVec);

		/*
		Purp: does mouse movement calculations, changes the view matrix
		Req: the offset you want for the mouse coordinates
		*/
		void doMouseMovement(float xoffset, float yoffset);

		/*
		Purp: allows the camera to move around the 3D world
		*/
		void move(GLFWwindow* window, float movementSpeed);

		//getters
		glm::vec3& getPosition() { return _cameraPos; }
		glm::vec3& getFront() { return _cameraFront; }
		glm::vec3& getRight() { return _cameraRight; }
		glm::vec3& getUp() { return _cameraUp; }
		glm::vec3& getRelativeUp() { return _cameraRelativeUp; }

		void setPosition(const glm::vec3& pos) { _cameraPos = pos; }
	private:
		//updates the camera
		void update();

		float _mouseSensitivity = 0.1f;
		bool  _firstMouse = true;
		float _yaw = -90.0f;
		float _pitch = 0.0f;

		glm::vec3 _cameraRight = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 _cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
		glm::vec3 _cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 _cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 _cameraRelativeUp = _cameraUp;
	};
}