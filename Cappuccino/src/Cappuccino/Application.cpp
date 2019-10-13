#include "Cappuccino/Application.h"
#include "Cappuccino/CappMacros.h"
#include "Cappuccino/Camera.h"
#include "Cappuccino/Game Object.h"
#include "Cappuccino/SoundSystem.h"

#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Cappuccino/Input.h"

#if SCENETEST
#include "Cappuccino/Testing/Test Scene.h"
#endif

#define GameObjects GameObject::gameObjects
using string = std::string;

namespace Cappuccino {


	Sedna::XinputManager* Application::_xinputManager = nullptr;
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

		if (_xinputManager == nullptr)
			_xinputManager = new Sedna::XinputManager();
	}

	Application::~Application() {}

	bool Application::isInstantiated() { return _instantiated; }

	void Application::run() {

		//init();

		CAPP_PRINT_N("OpenGL version %s", reinterpret_cast<GLchar const*>(glGetString(GL_VERSION)));
		CAPP_PRINT_N("Using %s %s\n", reinterpret_cast<GLchar const*>(glGetString(GL_VENDOR)), reinterpret_cast<GLchar const*>(glGetString(GL_RENDERER)));

		SoundSystem::init(CAPP_PATH + "Assets\\Sounds\\");

#if SOUNDTEST
		auto soundRef = SoundSystem::load2DSound("testSound.mp3");
		auto groupRef = SoundSystem::createChannelGroup("group1");

		SoundSystem::playSound2D(soundRef, groupRef);
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
			drawImGui(deltaTime);

			// Swap the buffers and poll events for the next frame
			lastFrame = currentFrame;
			glfwPollEvents();
			glfwSwapBuffers(_window);

		}

		cleanup();
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

#if _DEBUG
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();

		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
		io.ConfigFlags |= ImGuiConfigFlags_TransparentBackbuffers;

		ImGui_ImplGlfw_InitForOpenGL(_window, true);
		ImGui_ImplOpenGL3_Init("#version 420");

		ImGui::StyleColorsDark();
		ImGuiStyle& style = ImGui::GetStyle();

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 0.8f;
		}
#endif
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

		if (_xinputManager->controllerConnected(0) || _xinputManager->controllerConnected(1)
			|| _xinputManager->controllerConnected(2) || _xinputManager->controllerConnected(3))
			_xinputManager->update();

		SceneManager::updateScenes(dt);
		for (auto x : GameObjects)
			if (x->isActive())
				x->baseUpdate(dt);

	}

	void Application::draw(GLfloat dt) {
		glClearColor(_clearColour.x, _clearColour.y, _clearColour.z, _clearColour.w);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// TODO: RENDER HERE

	}

	void Application::drawImGui(GLfloat dt) {
		// Start new ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::Begin("Imgui window");

		// TODO: DRAW IMGUI STUFF HERE
		ImGui::DragFloat("Primitive 1 Position X", &(TestScene::testPrim._transform._translateMat[3].x));
		ImGui::DragFloat("Primitive 1 Position Y", &(TestScene::testPrim._transform._translateMat[3].y));
		ImGui::DragFloat("Primitive 1 Position Z", &(TestScene::testPrim._transform._translateMat[3].z));

		TestScene::testPrim._body.hitBox.back()._position.x = (TestScene::testPrim._transform._translateMat[3].x);
		TestScene::testPrim._body.hitBox.back()._position.y = (TestScene::testPrim._transform._translateMat[3].y);
		TestScene::testPrim._body.hitBox.back()._position.z = (TestScene::testPrim._transform._translateMat[3].z);

		ImGui::NewLine();

		ImGui::DragFloat("Primitive 2 Position X", &(TestScene::testPrim2._transform._translateMat[3].x));
		ImGui::DragFloat("Primitive 2 Position Y", &(TestScene::testPrim2._transform._translateMat[3].y));
		ImGui::DragFloat("Primitive 2 Position Z", &(TestScene::testPrim2._transform._translateMat[3].z));

		TestScene::testPrim2._body.hitBox.back()._position.x = (TestScene::testPrim2._transform._translateMat[3].x);
		TestScene::testPrim2._body.hitBox.back()._position.y = (TestScene::testPrim2._transform._translateMat[3].y);
		TestScene::testPrim2._body.hitBox.back()._position.z = (TestScene::testPrim2._transform._translateMat[3].z);

		// End the ImGui frame
		ImGui::End();
		ImGuiIO& io = ImGui::GetIO();

		io.DisplaySize = ImVec2(_width, _height);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {

			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();

			glfwMakeContextCurrent(_window);
		}
	}
}
