#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Cappuccino/CappMacros.h"


#pragma region PROGRAM SETTINGS

const GLuint  SCR_WIDTH		= 800*2;
const GLuint  SCR_HEIGHT	= 600*2;
const GLchar* SCR_NAME		= "Cappuccino Engine v1.0";

#pragma endregion

int main() {

	#pragma region GLFW/GLAD SETUP

	CAPP_PRINT("----------INITIALIZING GLFW----------");
	//glfwInit();
	if (!glfwInit()) {
		CAPP_PRINT_ERROR("Error initializing GLFW!\nExiting...");
		SYS_EXIT(-1);
	}
	CAPP_PRINT("Success! Setting GLFW Window Hints...");
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	
	CAPP_PRINT("----------CREATING GLFW WINDOW----------");
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, SCR_NAME, NULL, NULL);

	if(window == NULL) {
		CAPP_PRINT_ERROR("Error creating GLFW window!\nExiting...");
		glfwTerminate();
		SYS_EXIT(-2);
	}

	CAPP_PRINT("Success! Setting window settings...");
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height) { glViewport(0, 0, width, height); });


	
	CAPP_PRINT("----------INITIALIZING GLAD----------");
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		CAPP_PRINT_ERROR("Error initializing GLAD!\nExiting...");
		glfwTerminate();
		SYS_EXIT(-3);
	}

	CAPP_PRINT("Success! OpenGL function pointers loaded.");

	#pragma endregion 

	#pragma region RENDER LOOP

	CAPP_PRINT("\n----------STARTING RENDER LOOP...\n");
	while (!glfwWindowShouldClose(window)) {

		// TODO: PROCESS INPUTS

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	#pragma	endregion

	#pragma region PROGRAM TERMINATION

	CAPP_PRINT("----------CLEANING UP AND EXITING...");

	glfwTerminate();
	SYS_EXIT(0);

	#pragma	endregion
}
