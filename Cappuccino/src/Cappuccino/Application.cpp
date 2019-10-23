#include "Cappuccino/Application.h"
#include "Cappuccino/CappMacros.h"
#include "Cappuccino/Camera.h"
#include "Cappuccino/FontManager.h"
#include "Cappuccino/GameObject.h"
#include "Cappuccino/Input.h"
#include "Cappuccino/SceneManager.h"
#include "Cappuccino/SoundSystem.h"
#include "Cappuccino/XInputManager.h"

#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#if SCENETEST
#include "Cappuccino/Testing/Test Scene.h"
#endif

#define GameObjects GameObject::gameObjects
using string = std::string;

namespace Cappuccino {

	bool Application::_instantiated = false;
	GLFWwindow* Application::window = nullptr;

	Application::Application() : Application(100, 100, "Failed to load properly!", 4u, 2u) {}

	Application::Application(const GLuint WIDTH, const GLuint HEIGHT, const string& TITLE, const GLuint contextVersionMajor, const GLuint contextVersionMinor) {
		window = NULL;
		_width = WIDTH; _height = HEIGHT;
		_title = TITLE;
		_contextVersionMajor = contextVersionMajor; _contextVersionMinor = contextVersionMinor;

		_clearColour = glm::vec4(0.2f, 0.3f, 0.3f, 1.0f);

		_instantiated = true;
	}

	bool Application::isInstantiated() { return _instantiated; }

	void Application::init() {
		if (!glfwInit()) {
			CAPP_PRINT_ERROR("Error initializing GLFW! Exiting...\n");
			SYS_EXIT(-1);
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, _contextVersionMajor);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, _contextVersionMinor);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		window = glfwCreateWindow(_width, _height, _title.c_str(), NULL, NULL);

		if (window == NULL) {
			glfwTerminate();
			CAPP_PRINT_ERROR("Error creating GLFW window!");

			const char* error;
			glfwGetError(&error);
			CAPP_PRINT_ERROR(error);

			SYS_EXIT(-2);
		}

		glfwMakeContextCurrent(window);
		glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, GLint width, GLint height) { glViewport(0, 0, width, height); });


		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			glfwTerminate();

			CAPP_PRINT_ERROR("Error initializing GLAD! Exiting...\n");
			SYS_EXIT(-3);
		}

#if _DEBUG
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();

		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
		io.ConfigFlags |= ImGuiConfigFlags_TransparentBackbuffers;

		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 420");

		ImGui::StyleColorsDark();
		ImGuiStyle& style = ImGui::GetStyle();

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 0.8f;
		}
#endif
	}
	
	void Application::run() {
		
		CAPP_PRINT_N("OpenGL version %s", reinterpret_cast<GLchar const*>(glGetString(GL_VERSION)));
		CAPP_PRINT_N("Using %s %s\n", reinterpret_cast<GLchar const*>(glGetString(GL_VENDOR)), reinterpret_cast<GLchar const*>(glGetString(GL_RENDERER)));

		SoundSystem::init(CAPP_PATH + "Assets\\Sounds\\");

#if SOUNDTEST
		auto soundRef = SoundSystem::load2DSound("testSound.mp3");
		auto groupRef = SoundSystem::createChannelGroup("group1");

		SoundSystem::playSound2D(soundRef, groupRef);
#endif

		FontManager::init(CAPP_PATH + "Assets\\Fonts\\");

	//	FontManager::loadTypeFace("arial.ttf");

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		static GLfloat lastFrame;

		/*
		Render Loop
		*/
		while (!glfwWindowShouldClose(window)) {
			const GLfloat currentFrame = glfwGetTime();
			const GLfloat deltaTime = currentFrame - lastFrame;

			glClearColor(_clearColour.x, _clearColour.y, _clearColour.z, _clearColour.w);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			
			update(deltaTime);
			drawImGui(deltaTime);

			// Swap the buffers and poll events for the next frame
			lastFrame = currentFrame;
			glfwPollEvents();
			glfwSwapBuffers(window);

		}

		cleanup();
	}

	void Application::cleanup() {
		// Shutdown imGui
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		// Shutdown GLFW
		glfwTerminate();
	}


	/*
	GAME LOOP
	*/
	void Application::update(GLfloat dt) {
#ifdef _DEBUG
		if (isEvent(Events::Escape))
			exit(0);
#endif

		if (Sedna::XInputManager::controllerConnected(0) || Sedna::XInputManager::controllerConnected(1) ||
			Sedna::XInputManager::controllerConnected(2) || Sedna::XInputManager::controllerConnected(3)) {
			Sedna::XInputManager::update();
		}

		SceneManager::updateScenes(dt);
		for (auto x : GameObjects)
			if (x->isActive())
				x->baseUpdate(dt);

	}

	void Application::drawImGui(GLfloat dt) {
		// Start new ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::Begin("Imgui window");

		// TODO: RENDER IMGUI HERE
		
		// End the ImGui frame
		ImGui::End();
		ImGuiIO& io = ImGui::GetIO();

		io.DisplaySize = ImVec2(_width, _height);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {

			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();

			glfwMakeContextCurrent(window);
		}
	}
}
