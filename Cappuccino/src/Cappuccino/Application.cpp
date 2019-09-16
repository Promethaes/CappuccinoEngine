#include "Cappuccino/Application.h"


namespace Cappuccino {

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



		CAPP_PRINT_N("----------INITIALIZING GLAD----------");
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			glfwTerminate();
						
			CAPP_PRINT_ERROR("Error initializing GLAD! Exiting...\n");
			SYS_EXIT(-3);
		}

		CAPP_PRINT_N("OpenGL function pointers loaded.\n");

		#pragma endregion 

		
		#pragma region RENDER LOOP

		CAPP_PRINT_N("----------STARTING RENDER LOOP----------");
		CAPP_PRINT_N("OpenGL version %s", reinterpret_cast<GLchar const*>(glGetString(GL_VERSION)));
		CAPP_PRINT_N("Using %s %s\n", reinterpret_cast<GLchar const*>(glGetString(GL_VENDOR)), reinterpret_cast<GLchar const*>(glGetString(GL_RENDERER)));

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

		#pragma endregion
	}
}
