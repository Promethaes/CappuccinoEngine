#include "Cappuccino/Application.h"
#include "Cappuccino/Camera.h"
#include "Cappuccino/Game Object.h"
#include "Cappuccino/Scene Manager.h"
#include "Cappuccino/Test Scene.h"

#define GameObjects GameObject::gameObjects
using string = std::string;

namespace Cappuccino {

	#if SCENETEST
	void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	float lastX = 400, lastY = 300;
	float yaw = -90.0f;
	float pitch = 0.0f;
	bool firstMouse = true;

	#endif
	
	bool Application::_instantiated = false;

	Application::Application() : Application(10, 10, "Cappuccino Engine", 4u, 2u) {}

	Application::Application(const GLuint WIDTH, const GLuint HEIGHT, const string& TITLE, const GLuint contextVersionMajor, const GLuint contextVersionMinor) {
		_window = NULL;
		_width = WIDTH; _height = HEIGHT;
		_title = TITLE;
		_contextVersionMajor = contextVersionMajor; _contextVersionMinor = contextVersionMinor;
		
		_clearColour = glm::vec4(0.2f, 0.3f, 0.3f, 1.0f);

		_instantiated = true;
	}

	bool Application::isInstantiated() { return _instantiated; }

	void Application::run() {

		init();

		CAPP_PRINT_N("----------STARTING RENDER LOOP----------");
		CAPP_PRINT_N("OpenGL version %s", reinterpret_cast<GLchar const*>(glGetString(GL_VERSION)));
		CAPP_PRINT_N("Using %s %s\n", reinterpret_cast<GLchar const*>(glGetString(GL_VENDOR)), reinterpret_cast<GLchar const*>(glGetString(GL_RENDERER)));

		#if SCENETEST

		TestScene* testScene = new TestScene(true);

		#endif

		glEnable(GL_DEPTH_TEST);

		static GLfloat lastFrame;
		
		while (!glfwWindowShouldClose(_window)) {
			const GLfloat currentFrame = glfwGetTime();
			const GLfloat deltaTime = currentFrame - lastFrame;
			
			update(deltaTime);
			draw(deltaTime);

			// Swap the buffers and poll events for the next frame
			lastFrame = currentFrame;
			glfwPollEvents();
			glfwSwapBuffers(_window);
			
		}
	}

	void Application::init() {
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
		_window = glfwCreateWindow(_width, _height, _title.c_str(), NULL, NULL);

		if (_window == NULL) {
			glfwTerminate();
			CAPP_PRINT_ERROR("Error creating GLFW window!");

			const char* error;
			glfwGetError(&error);
			CAPP_PRINT_ERROR(error);

			CAPP_PRINT_ERROR("Exiting...\n");
			SYS_EXIT(-2);
		}

		CAPP_PRINT_N("Setting window settings...\n");
		glfwMakeContextCurrent(_window);
		glfwSetFramebufferSizeCallback(_window, [](GLFWwindow* window, GLint width, GLint height) { glViewport(0, 0, width, height); });


	#if SCENETEST
		glfwSetCursorPosCallback(_window, [](GLFWwindow* window, double xpos, double ypos) {
			if (firstMouse)
			{
				lastX = xpos;
				lastY = ypos;
				firstMouse = false;
			}

			GLfloat xOffset = xpos - lastX;
			GLfloat yOffset = lastY - ypos;
			lastX = xpos;
			lastY = ypos;

			Scene::defaultCamera->doMouseMovement(xOffset, yOffset);
		});
		
		glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	#endif



		CAPP_PRINT_N("----------INITIALIZING GLAD----------");
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			glfwTerminate();

			CAPP_PRINT_ERROR("Error initializing GLAD! Exiting...\n");
			SYS_EXIT(-3);
		}

		CAPP_PRINT_N("OpenGL function pointers loaded.\n");
	}

	void Application::cleanup() {
		CAPP_PRINT_N("----------CLEANING UP AND EXITING----------");

		glfwTerminate();
		CAPP_PRINT_N("GLFW Terminated.\n");

#if _DEBUG
		system("pause");
#endif
	}

	void Application::update(GLfloat dt) {
		SceneManager::updateScenes(dt);

		for (auto x : GameObjects)
			x->baseUpdate(dt);
	}

	void Application::draw(GLfloat dt) {
		glClearColor(_clearColour.x, _clearColour.y, _clearColour.z, _clearColour.w);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// TODO: RENDER HERE
		
	}
}