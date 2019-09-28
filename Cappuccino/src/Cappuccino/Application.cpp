#include "Cappuccino/Application.h"
#include "Cappuccino/Camera.h"
#include "Cappuccino/Game Object.h"
#include "Cappuccino/Test Scene.h"
#include "Cappuccino/Input.h"
#include "Cappuccino/Events.h"

#define GameObjects GameObject::gameObjects
using string = std::string;

namespace Cappuccino {

	
	bool Application::_instantiated = false;
	GLFWwindow* Application::_window = nullptr;

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

		CAPP_PRINT_N("OpenGL version %s", reinterpret_cast<GLchar const*>(glGetString(GL_VERSION)));
		CAPP_PRINT_N("Using %s %s\n", reinterpret_cast<GLchar const*>(glGetString(GL_VENDOR)), reinterpret_cast<GLchar const*>(glGetString(GL_RENDERER)));

		#if SCENETEST

		TestScene* testScene = new TestScene(true);

		#endif

		glEnable(GL_DEPTH_TEST);

		static GLfloat lastFrame;

		/*
		Render Loop
		*/
		while (!glfwWindowShouldClose(_window)) {
			const GLfloat currentFrame = glfwGetTime();
			const GLfloat deltaTime = currentFrame - lastFrame;
			
			draw(deltaTime);
			update(deltaTime);

			// Swap the buffers and poll events for the next frame
			lastFrame = currentFrame;
			glfwPollEvents();
			glfwSwapBuffers(_window);
			
		}


	}

	void Application::init() {
		if (!glfwInit()) {
			CAPP_PRINT_ERROR("Error initializing GLFW! Exiting...\n");
			SYS_EXIT(-1);
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, _contextVersionMajor);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, _contextVersionMinor);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		_window = glfwCreateWindow(_width, _height, _title.c_str(), NULL, NULL);

		if (_window == NULL) {
			glfwTerminate();
			CAPP_PRINT_ERROR("Error creating GLFW window!");

			const char* error;
			glfwGetError(&error);
			CAPP_PRINT_ERROR(error);

			CAPP_PRINT_ERROR("Exiting...\n");
			std::cin.get();
			SYS_EXIT(-2);
		}

		glfwMakeContextCurrent(_window);
		glfwSetFramebufferSizeCallback(_window, [](GLFWwindow* window, GLint width, GLint height) { glViewport(0, 0, width, height); });


		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			glfwTerminate();

			CAPP_PRINT_ERROR("Error initializing GLAD! Exiting...\n");
			SYS_EXIT(-3);
		}

	}

	void Application::cleanup() {

		glfwTerminate();
	}

	void Application::update(GLfloat dt) {
#ifdef _DEBUG
		if (isEvent(Events::Escape))
			exit(0);
#endif

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