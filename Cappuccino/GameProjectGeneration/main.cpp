#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Fmod/fmod.h>
#include <Fmod/fmod_common.h>
#include <Fmod/fmod.hpp>
#include <Fmod/fmod_errors.h>


#include <iostream>
#include <vector>
#include <time.h>
#include <Cappuccino/Camera.h>
#include <Cappuccino/ShaderProgram.h>
#include <Cappuccino/XinputManager.h>
#include <Cappuccino/Scene Manager.h>
#include <Cappuccino/Test Scene.h>
#include <Cappuccino/f16.h>
#include <Cappuccino/CappMacros.h>

#pragma region PROGRAM SETTINGS

constexpr GLuint  SCR_WIDTH  = 800*2;
constexpr GLuint  SCR_HEIGHT = 600*2;
constexpr GLchar* SCR_NAME   = "Cappuccino Engine v1.0";

#pragma endregion

#define Scenes Cappuccino::Scene::scenes
#define GameObjects Cappuccino::GameObject::gameObjects

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void processInput(Cappuccino::Camera& defaultCamera, float dt, GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

float lastX = 400, lastY = 300;
float yaw = -90.0f;
float pitch = 0.0f;
bool firstMouse = true;

Cappuccino::Camera* defaultCamera = new Cappuccino::Camera();

float dt = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame
int main() {
	srand(time(0));

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
	CAPP_PRINT_N("Using OpenGL version %s", reinterpret_cast<char const*>(glGetString(GL_VERSION)));
	CAPP_PRINT_N("Using %s GPU to render", reinterpret_cast<char const*>(glGetString(GL_RENDERER)));
	
	while (!glfwWindowShouldClose(window)) {

		// TODO: PROCESS INPUTS HERE!!!

		// Clear the screen
		float currentFrame = glfwGetTime();
		dt = currentFrame - lastFrame;
		lastFrame = currentFrame;
		processInput(*defaultCamera, dt, window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// TODO: RENDER HERE!!!
		for (unsigned i = 0; i < Scenes.size(); i++) {
			if (!Scenes[i]->isInit())
				Scenes[i]->init();
			if (Scenes[i]->isInit())
				Scenes[i]->baseUpdate(dt, *defaultCamera);
		}
		for (auto x : GameObjects)
			x->baseUpdate(dt);
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
//this spreads viruses
	//system("pause");
#endif
	return 0;

	#pragma	endregion
}

void processInput(Cappuccino::Camera& defaultCamera, float dt, GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	defaultCamera.move(window, 2.5f * dt);
}
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	defaultCamera->doMouseMovement(xoffset, yoffset);
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {

	glViewport(0, 0, width, height);

}
