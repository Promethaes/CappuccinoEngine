#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Cappuccino/CappMacros.h"
#include "Cappuccino/Scene Manager.h"

#pragma region PROGRAM SETTINGS

constexpr GLuint  SCR_WIDTH  = 800;
constexpr GLuint  SCR_HEIGHT = 600;
constexpr GLchar* SCR_TITLE  = const_cast<GLchar*>("Cappuccino Engine v1.0");

#pragma endregion

int main() {

	#pragma region GLFW/GLAD SETUP

	CAPP_PRINT_N("----------INITIALIZING GLFW----------");
	CAPP_PRINT_N("Initializing...");
	if (!glfwInit()) {
		CAPP_PRINT_ERROR("Error initializing GLFW! Exiting...\n");
		SYS_EXIT(-1);
	}
	
	CAPP_PRINT_N("Setting GLFW window hints...");
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	
	CAPP_PRINT_N("Creating window...");
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, SCR_NAME, NULL, NULL);

	if(window == NULL) {
		const char* error;
		glfwGetError(&error);
		CAPP_PRINT_ERROR(error);
		
		CAPP_PRINT_ERROR("Error creating GLFW window! Exiting...\n");
		glfwTerminate();
		SYS_EXIT(-2);
	}

	CAPP_PRINT_N("Setting window settings...\n");
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height) { glViewport(0, 0, width, height); });


	
	CAPP_PRINT_N("----------INITIALIZING GLAD----------");
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		CAPP_PRINT_ERROR("Error initializing GLAD! Exiting...\n");
		glfwTerminate();
		SYS_EXIT(-3);
	}

	CAPP_PRINT_N("OpenGL function pointers loaded.\n");

	#pragma endregion 


	
	#pragma region RENDER LOOP

	CAPP_PRINT_N("----------STARTING RENDER LOOP----------");	
	CAPP_PRINT_N("OpenGL version %s", reinterpret_cast<char const*>(glGetString(GL_VERSION)));
	CAPP_PRINT_N("Using %s %s\n", reinterpret_cast<char const*>(glGetString(GL_VENDOR)), reinterpret_cast<char const*>(glGetString(GL_RENDERER)));
	
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		// TODO: PROCESS INPUTS HERE

		// Clear the screen
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// TODO: RENDER HERE

		// Swap the buffers and poll events for the next frame
		glfwSwapBuffers(window);
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
