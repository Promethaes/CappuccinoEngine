#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
namespace Cappuccino {
	
	/*
	Desc: Allows you to actually view the game
	*/
	class Camera {
	public:
		Camera() = default;

		//shows where the player is looking at
		glm::mat4 whereAreWeLooking() const;
		
		//moves camera witht mouse movement
		void doMouseMovement(float xoffset,float yoffset);
		
		//camera movemnt using arrow keys(should be wasd)
		void move(GLFWwindow* window,float movementSpeed);

		//getters
		glm::vec3& getPosition() { return cameraPos; }
		glm::vec3& getFront() { return cameraFront; }
	private:
		void update();

		float mouseSensitivity = 0.1f;
		bool firstMouse = true;
		float yaw = -90.0f;
		float pitch = 0.0f;

		glm::vec3 cameraRight = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
		glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	};
}