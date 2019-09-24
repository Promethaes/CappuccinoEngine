#include "Cappuccino/Application.h"
#include "Cappuccino/Camera.h"
#include "Cappuccino/Game Object.h"
#include "Cappuccino/Scene Manager.h"
#include "Cappuccino/Test Scene.h"

namespace Cappuccino {
#define GameObjects Cappuccino::GameObject::gameObjects

	float dt = 0.0f;	// Time between current frame and last frame
	float lastFrame = 0.0f; // Time of last frame

#if SCENETEST
	void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	float lastX = 400, lastY = 300;
	float yaw = -90.0f;
	float pitch = 0.0f;
	bool firstMouse = true;

#endif

	GLuint  Application::_width = 10u;
	GLuint  Application::_height = 10u;
	GLchar* Application::_title = "Cappuccino Engine";
	GLuint  Application::_contextVersionMajor = 4u;
	GLuint  Application::_contextVersionMinor = 2u;

	bool Application::_instantiated = false;

	Application::Application(const GLuint width, const GLuint height, const GLchar* title, const GLuint contextVersionMajor, const GLuint contextVersionMinor) {
		_width = width;
		_height = height;
		_title = const_cast<GLchar*>(title);
		_contextVersionMajor = contextVersionMajor;
		_contextVersionMinor = contextVersionMinor;

		_instantiated = true;
	}

	bool Application::isInstantiated() { return _instantiated; }

	void Application::run() {
#pragma region GLFW/GLAD SETUP

		CAPP_PRINT_N("----------INITIALIZING GLFW----------");
		CAPP_PRINT_N("Initializing...");
		if (!glfwInit()) {
			CAPP_PRINT_ERROR("Error initializing GLFW! Exiting...\n");
			SYS_EXIT(-1);
		}

		CAPP_PRINT_N("Setting GLFW window hints...");
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, _contextVersionMajor);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, _contextVersionMinor);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		CAPP_PRINT_N("Creating window...");
		GLFWwindow* window = glfwCreateWindow(_width, _height, _title, NULL, NULL);

		if (window == NULL) {
			glfwTerminate();
			CAPP_PRINT_ERROR("Error creating GLFW window!");

			const char* error;
			glfwGetError(&error);
			CAPP_PRINT_ERROR(error);

			CAPP_PRINT_ERROR("Exiting...\n");
			SYS_EXIT(-2);
		}

		CAPP_PRINT_N("Setting window settings...\n");
		glfwMakeContextCurrent(window);
		glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, GLint width, GLint height) { glViewport(0, 0, width, height); });

#if SCENETEST
		glfwSetCursorPosCallback(window, mouse_callback);
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
#endif

		CAPP_PRINT_N("----------INITIALIZING GLAD----------");
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			glfwTerminate();

			CAPP_PRINT_ERROR("Error initializing GLAD! Exiting...\n");
			SYS_EXIT(-3);
		}

		CAPP_PRINT_N("OpenGL function pointers loaded.\n");

#pragma endregion

		SoundSystem::init(CAPP_PATH + "Assets\\Sounds\\");

#if SOUNDTEST
		auto soundRef = SoundSystem::load2DSound("testSound.mp3");
		auto groupRef = SoundSystem::createChannelGroup("group1");
		auto groupRef2 = SoundSystem::createChannelGroup("group2");

		SoundSystem::playSound2D(soundRef, groupRef);
#endif

#pragma region RENDER LOOP

		CAPP_PRINT_N("----------STARTING RENDER LOOP----------");
		CAPP_PRINT_N("OpenGL version %s", reinterpret_cast<GLchar const*>(glGetString(GL_VERSION)));
		CAPP_PRINT_N("Using %s %s\n", reinterpret_cast<GLchar const*>(glGetString(GL_VENDOR)), reinterpret_cast<GLchar const*>(glGetString(GL_RENDERER)));

#if SCENETEST
		TestScene* testScene = new TestScene(true);
#endif

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_STENCIL_TEST);

		while (!glfwWindowShouldClose(window)) {
			float currentFrame = glfwGetTime();
			dt = currentFrame - lastFrame;
			lastFrame = currentFrame;
			// TODO: PROCESS INPUTS HERE

			// Clear the screen
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

			SceneManager::updateScenes(dt);

			for (auto x : GameObjects)
				x->baseUpdate(dt);

			// TODO: RENDER HERE

			// Swap the buffers and poll events for the next frame
			glfwPollEvents();
			glfwSwapBuffers(window);
		}

#pragma	endregion

#pragma region PROGRAM TERMINATION

		CAPP_PRINT_N("----------CLEANING UP AND EXITING----------");

		glfwTerminate();
		CAPP_PRINT_N("GLFW Terminated.\n");

#pragma endregion
	}

#if SCENETEST
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

		Cappuccino::Scene::defaultCamera->doMouseMovement(xoffset, yoffset);
	}
#endif
}