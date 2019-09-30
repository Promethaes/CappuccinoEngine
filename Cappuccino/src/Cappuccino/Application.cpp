#include "Cappuccino/Application.h"
#include "Cappuccino/CappMacros.h"

#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Cappuccino/Input.h"

#if SCENETEST

#include "Cappuccino/Test Scene.h"

#endif

#define GameObjects GameObject::gameObjects
using string = std::string;

namespace Cappuccino {

	#if SCENETEST
	
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

	Application::~Application() {}

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
		
		while (!glfwWindowShouldClose(_window)) {
			const GLfloat currentFrame = glfwGetTime();
			const GLfloat deltaTime = currentFrame - lastFrame;
			
			update(deltaTime);
			draw(deltaTime);
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

			SYS_EXIT(-2);
		}
		
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

	void Application::drawImGui(GLfloat dt) {
		// Start new ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::Begin("Imgui window");

		// TODO: DRAW IMGUI STUFF HERE

		
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
