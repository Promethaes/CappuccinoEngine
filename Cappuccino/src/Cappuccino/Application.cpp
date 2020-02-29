#include "Cappuccino/Application.h"
#include "Cappuccino/UI.h"
#include "Cappuccino/FrameBuffer.h"

#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <thread>
#include <chrono>
#include "Cappuccino/ResourceManager.h"
#include "Cappuccino/Cubemap.h"

#define GameObjects GameObject::gameObjects
using string = std::string;

namespace Cappuccino {

	bool Application::_instantiated = false;
	GLFWwindow* Application::window = nullptr;
	Shader* Application::_gBufferShader = nullptr;
	Shader* Application::_lightingPassShader = nullptr;

	Application::Application() : Application(100, 100, "Failed to load properly!", {}, 4u, 6u) {}

	Application::Application(const GLuint WIDTH, const GLuint HEIGHT, const string& TITLE, const std::vector<Viewport>& viewports, const GLuint contextVersionMajor, const GLuint contextVersionMinor) {
		window = NULL;
		_width = WIDTH; _height = HEIGHT;
		_title = TITLE;
		_contextVersionMajor = contextVersionMajor; _contextVersionMinor = contextVersionMinor;
		_viewports = viewports;

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
		//glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		window = glfwCreateWindow(_width, _height, _title.c_str(), NULL, NULL);

		if (window == NULL) {
			glfwTerminate();
			CAPP_PRINT_ERROR("Error creating GLFW window!");

			const char* error;
			glfwGetError(&error);
			CAPP_PRINT_ERROR("%s", error);

			SYS_EXIT(-2);
		}

		glfwMakeContextCurrent(window);
		glfwSetWindowUserPointer(window, this);
		glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, const GLint width, const GLint height) { glViewport(0, 0, width, height); });


		if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
			glfwTerminate();

			CAPP_PRINT_ERROR("Error initializing GLAD! Exiting...\n");
			SYS_EXIT(-3);
		}

#if _DEBUG

		CAPP_GL_CALL(glEnable(GL_DEBUG_OUTPUT));
		CAPP_GL_CALL(glDebugMessageCallback(glDebugMessageCallbackFunc, NULL));
		CAPP_GL_CALL(glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, 0, GL_FALSE));

		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();

		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
		io.ConfigFlags |= ImGuiConfigFlags_TransparentBackbuffers;

		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 460");

		ImGui::StyleColorsDark();
		ImGuiStyle& style = ImGui::GetStyle();

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 0.8f;
		}

#endif

		SoundSystem::init(R"(.\Assets\Sounds\)");
		StudioSound::init(R"(.\Assets\Sounds\)");
		FontManager::init(R"(.\Assets\Fonts\)");
		ResourceManager::init();
	}

	void Application::run() {

		CAPP_PRINT_N("OpenGL version %s", reinterpret_cast<GLchar const*>(glGetString(GL_VERSION)));
		CAPP_PRINT_N("Using %s %s\n", reinterpret_cast<GLchar const*>(glGetString(GL_VENDOR)), reinterpret_cast<GLchar const*>(glGetString(GL_RENDERER)));


#if SOUNDTEST
		auto soundRef = SoundSystem::load2DSound("testSound.mp3");
		auto groupRef = SoundSystem::createChannelGroup("group1");

		SoundSystem::playSound2D(soundRef, groupRef);
#endif

		glEnable(GL_DEPTH_TEST);
		//glEnable(GL_CULL_FACE);
		//glEnable(GL_BLEND);
		//glEnable(GL_SCISSOR_TEST);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

		GLfloat lastFrame = 0;
		float lag = 0.0f;
		float turnRate = 1000.0f / 120.0f;
		turnRate /= 1000.0f;


		//https://learnopengl.com/code_viewer_gh.php?code=src/4.advanced_opengl/5.1.framebuffers/framebuffers.cpp
		//took these from learnopengl cause i didn't wanna write it all out myself
		float quadVertices[] = {
			// positions   // texCoords
			-1.0f,  1.0f,  0.0f, 1.0f,
			-1.0f, -1.0f,  0.0f, 0.0f,
			 1.0f, -1.0f,  1.0f, 0.0f,

			-1.0f,  1.0f,  0.0f, 1.0f,
			 1.0f, -1.0f,  1.0f, 0.0f,
			 1.0f,  1.0f,  1.0f, 1.0f
		};
		// screen quad VAO
		unsigned int quadVAO, quadVBO;
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));


		//https://learnopengl.com/Advanced-Lighting/Deferred-Shading
		unsigned gBuffer = 0;
		glGenFramebuffers(1, &gBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
		unsigned gPos, gNormal, gAlbedo, gMetalRoughnessAO,gEmissive,gDepthStencil;

		//position
		glGenTextures(1, &gPos);
		glBindTexture(GL_TEXTURE_2D, gPos);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, _width, _height, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPos, 0);

		//normals
		glGenTextures(1, &gNormal);
		glBindTexture(GL_TEXTURE_2D, gNormal);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, _width, _height, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);

		//albedo
		glGenTextures(1, &gAlbedo);
		glBindTexture(GL_TEXTURE_2D, gAlbedo);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedo, 0);

		//metallic, roughness, ambient occlusion
		glGenTextures(1, &gMetalRoughnessAO);
		glBindTexture(GL_TEXTURE_2D, gMetalRoughnessAO);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, _width, _height, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, gMetalRoughnessAO, 0);

		//emissive
		glGenTextures(1, &gEmissive);
		glBindTexture(GL_TEXTURE_2D, gEmissive);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, _width, _height, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_2D, gEmissive, 0);

		unsigned attachments[5] = { GL_COLOR_ATTACHMENT0,GL_COLOR_ATTACHMENT1,GL_COLOR_ATTACHMENT2,GL_COLOR_ATTACHMENT3,GL_COLOR_ATTACHMENT4 };
		glDrawBuffers(5, attachments);

		glGenRenderbuffers(1, &gDepthStencil);
		glBindRenderbuffer(GL_RENDERBUFFER, gDepthStencil);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, _width, _height);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, gDepthStencil);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			printf("ERROR: FRAMEBUFFER NOT COMPLETE\n");
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		
		

		/*
		Render Loop
		*/
		while (!glfwWindowShouldClose(window)) {
			//https://gameprogrammingpatterns.com/game-loop.html

			GLfloat currentFrame = static_cast<GLfloat>(glfwGetTime());
			GLfloat deltaTime = currentFrame - lastFrame;
			lag += deltaTime;

#ifdef _DEBUG
			update(deltaTime);
#else
			while (lag >= turnRate) {
				update(turnRate);
				lag -= turnRate;
			}
#endif
			//need to replace this, causes performance issues for sure.
			//for some reason even if i try to do the gl calls manually, nothing renders
			_viewports[0].use();

			//if there are user defined framebuffers
			if (!Framebuffer::_framebuffers.empty()) {
				if (Framebuffer::_overrideCallback != nullptr)
					Framebuffer::_overrideCallback();
				else {
					for (auto k : Framebuffer::_framebuffers) {

						k->bind();
						glClearColor(_clearColour.x, _clearColour.y, _clearColour.z, _clearColour.w);
						k->_callback != nullptr ? k->_callback() : 0;

						for (auto y : GameObjects) {
							if (y->isActive() && y->isVisible()) {
								y->draw();
							}
						}
						for (auto c : Cubemap::allCubemaps) {
							c->draw();
						}
						k->unbind();
					}

					glClear(GL_COLOR_BUFFER_BIT);
					Framebuffer::_fbShader->use();
					for (unsigned i = 0; i < Framebuffer::_framebuffers.size(); i++) {
						for (unsigned j = 0; j < Framebuffer::_framebuffers[i]->getColourBuffers().size(); j++) {
							glActiveTexture(GL_TEXTURE0 + j + i);
							glBindTexture(GL_TEXTURE_2D, Framebuffer::_framebuffers[i]->getColourBuffers()[j]);
						}
					}

					Framebuffer::_fbShader->setUniform("screenTexture", 0);
					Framebuffer::_fbShader->setUniform("bloom", 1);
					glBindVertexArray(quadVAO);
					glDrawArrays(GL_TRIANGLES, 0, 6);

					for (unsigned i = 0; i < Framebuffer::_framebuffers.size(); i++) {
						for (unsigned j = 0; j < Framebuffer::_framebuffers[i]->getColourBuffers().size(); j++) {
							glActiveTexture(GL_TEXTURE0 + j + i);
							glBindTexture(GL_TEXTURE_2D, 0);
						}
					}
					for (auto x : UserInterface::_allUI)
						x->draw();
				}
			}
			else {
				
				//geometry pass
				glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				for (auto y : GameObjects)
					if (y->isActive() && y->isVisible()) 
						y->gBufferDraw(_gBufferShader);

				glBindFramebuffer(GL_FRAMEBUFFER, 0);

				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				_lightingPassShader->use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, gPos);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, gNormal);
				glActiveTexture(GL_TEXTURE2);
				glBindTexture(GL_TEXTURE_2D, gAlbedo);
				glActiveTexture(GL_TEXTURE3);
				glBindTexture(GL_TEXTURE_2D, gMetalRoughnessAO);
				glActiveTexture(GL_TEXTURE4);
				glBindTexture(GL_TEXTURE_2D, gEmissive);

				glBindVertexArray(quadVAO);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, gPos);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, gNormal);
				glActiveTexture(GL_TEXTURE2);
				glBindTexture(GL_TEXTURE_2D, gAlbedo);
				glActiveTexture(GL_TEXTURE3);
				glBindTexture(GL_TEXTURE_2D, gMetalRoughnessAO);
				glActiveTexture(GL_TEXTURE4);
				glBindTexture(GL_TEXTURE_2D, gEmissive);

				glBindFramebuffer(GL_READ_FRAMEBUFFER, gBuffer);
				glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

				glBlitFramebuffer(0, 0, _width, _height, 0, 0, _width, _height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
				glBindFramebuffer(GL_FRAMEBUFFER, 0);

				for (auto x : UserInterface::_allUI)
					x->draw();
				for (auto c : Cubemap::allCubemaps) {
					c->draw();
				}
			}


#if _DEBUG
			drawImGui(deltaTime);
#endif

			// Swap the buffers and poll events for the next frame
			CAPP_GL_CALL(glfwPollEvents());
			CAPP_GL_CALL(glfwSwapBuffers(window));

			float delay = turnRate * 1000.0f;
			long long finalDelay = (long long)(currentFrame + delay - glfwGetTime());

			std::this_thread::sleep_for(std::chrono::milliseconds(finalDelay));

			lastFrame = currentFrame;
		}

		cleanup();
	}

	void Application::cleanup() {

#if _DEBUG
		// Shutdown imGui
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
#endif

		// Shutdown GLFW
		glfwTerminate();

		ResourceManager::shutdown();
	}


	/*
	GAME LOOP
	*/
	void Application::update(GLfloat dt) {
#if _DEBUG
		if (isEvent(Events::Escape)) {
			glfwSetWindowShouldClose(window, true);
		}
#endif

		if (Sedna::XInputManager::controllerConnected(0) || Sedna::XInputManager::controllerConnected(1) ||
			Sedna::XInputManager::controllerConnected(2) || Sedna::XInputManager::controllerConnected(3)) {
			Sedna::XInputManager::update();
		}

		SoundSystem::update();
		StudioSound::update();

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

		io.DisplaySize = ImVec2(static_cast<GLfloat>(_width), static_cast<GLfloat>(_height));

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {

			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();

			glfwMakeContextCurrent(window);
		}
	}

	void Application::glDebugMessageCallbackFunc(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* msg, const void* data) {

		char buffer[9] = { '\0' };
		sprintf(buffer, "%.8x", id);

		std::string message = "OpenGL(0x" + std::string(buffer) + "): ";

		switch (type) {
		case GL_DEBUG_TYPE_ERROR:
			message += "ERROR";
			break;
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
			message += "DEPRECATED BEHAVIOUR";
			break;
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
			message += "UNDEFINED BEHAVIOUR";
			break;
		case GL_DEBUG_TYPE_PORTABILITY:
			message += "PORTABILITY ISSUE";
			break;
		case GL_DEBUG_TYPE_PERFORMANCE:
			message += "PERFORMANCE ISSUE";
			break;
		case GL_DEBUG_TYPE_MARKER:
			message += "TYPE MARKER";
			break;
		case GL_DEBUG_TYPE_OTHER:
			message += "OTHER";
			break;
		default: break;
		}

		message += "\nSOURCE: ";
		switch (source) {
		case GL_DEBUG_SOURCE_API:
			message += "API";
			break;
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
			message += "Window system";
			break;
		case GL_DEBUG_SOURCE_SHADER_COMPILER:
			message += "Shader compiler";
			break;
		case GL_DEBUG_SOURCE_THIRD_PARTY:
			message += "Third party";
			break;
		case GL_DEBUG_SOURCE_APPLICATION:
			message += "Application";
			break;
		case GL_DEBUG_SOURCE_OTHER:
			message += "Other";
		default: break;
		}

		message += " \nSEVERITY: ";
		switch (severity) {
		case GL_DEBUG_SEVERITY_HIGH:
			message += "HIGH";
			break;
		case GL_DEBUG_SEVERITY_MEDIUM:
			message += "Medium";
			break;
		case GL_DEBUG_SEVERITY_LOW:
			message += "Low";
			break;
		case GL_DEBUG_SEVERITY_NOTIFICATION:
			message += "Notification";
		default: break;
		}

		message += "\n" + std::string(msg) + "\n";

		if (type == GL_DEBUG_TYPE_ERROR || severity == GL_DEBUG_SEVERITY_HIGH) {
			CAPP_PRINT_ERROR("%s", message.c_str());
			__debugbreak();
		}
		else {
			CAPP_PRINT_WARNING("%s", message.c_str());
		}

	}
	Viewport::Viewport(const glm::vec4& borderColour, const glm::vec4& bounds, void(*specialDrawInstructionsCallback)(), GLenum drawMode)
		:_borderColour(borderColour), _bounds(bounds), _drawMode(drawMode), _callback(specialDrawInstructionsCallback)
	{
	}
	void Viewport::use()
	{
		CAPP_GL_CALL(glViewport(_bounds.x, _bounds.y, _bounds.z, _bounds.w));
		CAPP_GL_CALL(glScissor(_bounds.x, _bounds.y, _bounds.z, _bounds.w));
		CAPP_GL_CALL(glClearColor(_borderColour.x, _borderColour.y, _borderColour.z, 0.0f));
		CAPP_GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
		CAPP_GL_CALL(glPolygonMode(GL_FRONT_AND_BACK, _drawMode));
		_callback != nullptr ? _callback() : 0;
	}
}
