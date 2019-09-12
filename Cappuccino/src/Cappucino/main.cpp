#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Cappuccino/CappMacros.h"

#pragma region PROGRAM SETTINGS

constexpr GLuint  SCR_WIDTH  = 800;
constexpr GLuint  SCR_HEIGHT = 600;
constexpr GLchar* SCR_NAME   = "Cappuccino Engine v1.0";

#pragma endregion

int main() {

	#pragma region GLFW/GLAD SETUP

	CAPP_PRINT_N("----------INITIALIZING GLFW----------");
	if (!glfwInit()) {
		CAPP_PRINT_ERROR("Error initializing GLFW!\nExiting...");
		SYS_EXIT(-1);
	}
	
	CAPP_PRINT_N("Success! Setting GLFW Window Hints...\n");
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	
	CAPP_PRINT_N("----------CREATING GLFW WINDOW----------");
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, SCR_NAME, NULL, NULL);

	if(window == NULL) {
		const char* error;
		glfwGetError(&error);
		CAPP_PRINT_ERROR(error);
		
		CAPP_PRINT_ERROR("Error creating GLFW window!\nExiting...");
		glfwTerminate();
		SYS_EXIT(-2);
	}

	CAPP_PRINT_N("Success! Setting window settings...\n");
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height) { glViewport(0, 0, width, height); });


	
	CAPP_PRINT_N("----------INITIALIZING GLAD----------");
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		CAPP_PRINT_ERROR("Error initializing GLAD!\nExiting...");
		glfwTerminate();
		SYS_EXIT(-3);
	}

	CAPP_PRINT_N("Success! OpenGL function pointers loaded.\n");

	#pragma endregion 

	#pragma region RENDER LOOP

	CAPP_PRINT_N("----------STARTING RENDER LOOP----------");	
	CAPP_PRINT_N("Using OpenGL version %s", (char*)glGetString(GL_VERSION));
	CAPP_PRINT_N("Using %s GPU to render", (char*)glGetString(GL_RENDERER));
	
	while (!glfwWindowShouldClose(window)) {

		// TODO: PROCESS INPUTS HERE!!!

		// Clear the screen
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// TODO: RENDER HERE!!!

		// Swap the buffers and poll events for the next frame
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	#pragma	endregion

	#pragma region PROGRAM TERMINATION

	CAPP_PRINT_N("----------CLEANING UP AND EXITING----------");

	glfwTerminate();
	CAPP_PRINT_N("GLFW Terminated.\n");

#if _DEBUG
	system("pause");
#endif
	return 0;

	#pragma	endregion
}
