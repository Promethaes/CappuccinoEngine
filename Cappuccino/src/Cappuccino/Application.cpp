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
	Shader* Application::_shadowMappingShader = nullptr;
	Shader* Application::_gBufferShader = nullptr;
	Shader* Application::_lightingPassShader = nullptr;
	Shader* Application::_blurPassShader = nullptr;
	Shader* Application::_ppShader = nullptr;
	LUT* Application::_activeLUT = nullptr;
	Application::LightVector Application::allLights;
	bool Application::_useDeferred = true;
	int Application::_numBlurPasses = 10;

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
		glfwWindowHint(GLFW_SAMPLES, 16);

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
		CAPP_GL_CALL(glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS));
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
		glEnable(GL_CULL_FACE);
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
		unsigned gPos, gNormal, gAlbedo, gMetalRoughnessAO, gEmissive, gDepthStencil;

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

		glGenTextures(1, &gDepthStencil);
		glBindTexture(GL_TEXTURE_2D, gDepthStencil);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, _width, _height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, gDepthStencil, 0);


		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			printf("ERROR: FRAMEBUFFER NOT COMPLETE\n");
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		///END OF G BUFFER

		///HDR BUFFER
		//https://learnopengl.com/code_viewer_gh.php?code=src/5.advanced_lighting/7.bloom/bloom.cpp
		unsigned hdrFrameBuffer = 0;
		glGenFramebuffers(1, &hdrFrameBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, hdrFrameBuffer);
		unsigned hdrColourBuffer, hdrBrightBuffer, hdrDepthStencil;
		glEnable(GL_DEPTH_TEST);

		//colour buffer
		glGenTextures(1, &hdrColourBuffer);
		glBindTexture(GL_TEXTURE_2D, hdrColourBuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, _width, _height, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, hdrColourBuffer, 0);

		//bright buffer
		glGenTextures(1, &hdrBrightBuffer);
		glBindTexture(GL_TEXTURE_2D, hdrBrightBuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, _width, _height, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, hdrBrightBuffer, 0);

		unsigned hdrAttachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
		glDrawBuffers(2, hdrAttachments);

		glGenRenderbuffers(1, &hdrDepthStencil);
		glBindRenderbuffer(GL_RENDERBUFFER, hdrDepthStencil);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, _width, _height);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, hdrDepthStencil);


		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			printf("ERROR: FRAMEBUFFER NOT COMPLETE\n");
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		///HDR BUFFER

		///BLUR BUFFERS
		unsigned pingpong[2];
		unsigned pingpongColourBuffers[2];
		glGenFramebuffers(2, pingpong);
		glGenTextures(2, pingpongColourBuffers);
		for (unsigned int i = 0; i < 2; i++)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, pingpong[i]);
			glBindTexture(GL_TEXTURE_2D, pingpongColourBuffers[i]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, _width, _height, 0, GL_RGB, GL_FLOAT, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongColourBuffers[i], 0);

			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
				printf("ERROR: FRAMEBUFFER NOT COMPLETE\n");

		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		///BLUR BUFFERS

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

			if (!_useDeferred) {
				for (auto y : GameObjects) {
					if (y->isActive() && y->isVisible())
						y->draw();
					// this feels like a terrible way to do it
					if (y->getClose())
						glfwSetWindowShouldClose(window, true);
				}
				for (auto x : UserInterface::_allUI)
					x->draw();
				for (auto c : Cubemap::allCubemaps) {
					c->draw();
				}
			}
			else {

				// Clear main buffer
				glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

				// Pre-render settings
				glEnable(GL_DEPTH_TEST);
				glEnable(GL_CULL_FACE);
				glDisable(GL_BLEND);

				// Shadow mapping
				for (auto pLight : allLights) {
					if (!pLight._isActive || !pLight.isShadowCaster) {
						continue;
					}

					auto proj = pLight.projectionMat;
					auto pos = pLight._pos;

					std::vector<glm::mat4> shadowMatrices = {
						proj * glm::lookAt(pos, pos + glm::vec3(1.0f,  0.0f,  0.0f), { 0.0f, -1.0f,  0.0f }),
						proj * glm::lookAt(pos, pos + glm::vec3(-1.0f,  0.0f,  0.0f), { 0.0f, -1.0f,  0.0f }),
						proj * glm::lookAt(pos, pos + glm::vec3(0.0f,  1.0f,  0.0f), { 0.0f,  0.0f,  1.0f }),
						proj * glm::lookAt(pos, pos + glm::vec3(0.0f, -1.0f,  0.0f), { 0.0f,  0.0f, -1.0f }),
						proj * glm::lookAt(pos, pos + glm::vec3(0.0f,  0.0f,  1.0f), { 0.0f, -1.0f,  0.0f }),
						proj * glm::lookAt(pos, pos + glm::vec3(0.0f,  0.0f, -1.0f), { 0.0f, -1.0f,  0.0f })
					};

					glBindFramebuffer(GL_FRAMEBUFFER, pLight.shadowBuffer);
					glViewport(0, 0, pLight.resolution, pLight.resolution);
					glClear(GL_DEPTH_BUFFER_BIT);
					{
						_shadowMappingShader->use();
						_shadowMappingShader->setUniform("lightPosition", pos);
						_shadowMappingShader->setUniform("farPlane", 400.0f);
						for (unsigned i = 0; i < 6; ++i) {
							_shadowMappingShader->setUniform("shadowMatrices[" + std::to_string(i) + "]", shadowMatrices[i]);
						}

						for (auto y : GameObjects) {
							if (y->isActive() && y->isVisible()) {
								y->shadowDraw(_shadowMappingShader);
							}
						}
					}
					glBindFramebuffer(GL_FRAMEBUFFER, 0);
				}




				//geometry pass
				glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
				glViewport(0, 0, _width, _height);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

				for (auto y : GameObjects) {
					if (y->isActive() && y->isVisible())
						y->gBufferDraw(_gBufferShader);
					// this feels like a terrible way to do it (i.e. code smell bad)
					if (y->getClose())
						glfwSetWindowShouldClose(window, true);
				}
				glBindFramebuffer(GL_FRAMEBUFFER, 0);
				glUseProgram(0);

				glDisable(GL_DEPTH_TEST);
				glDisable(GL_CULL_FACE);

				glEnable(GL_BLEND);
				glBlendFunc(GL_ONE, GL_ONE);

				// lighting pass into HDR buffer
				glBindFramebuffer(GL_FRAMEBUFFER, hdrFrameBuffer);
				{
					glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

					_lightingPassShader->use();
					glBindTextureUnit(0, gPos);
					glBindTextureUnit(1, gNormal);
					glBindTextureUnit(2, gAlbedo);
					glBindTextureUnit(3, gMetalRoughnessAO);
					glBindTextureUnit(4, gEmissive);

					for (const auto& light : allLights) {
						if (!light._isActive)
							continue;

						_lightingPassShader->setUniform("light.position", light._pos);
						_lightingPassShader->setUniform("light.colour", light._col);

						if(light.isShadowCaster) {
							_lightingPassShader->setUniform("light.isShadowCaster", true);
							_lightingPassShader->setUniform("light.depthMap", 5);
							glBindTextureUnit(5, light.depthMap);
						}
						else {
							_lightingPassShader->setUniform("light.isShadowCaster", false);
						}

						glBindVertexArray(quadVAO);
						glDrawArrays(GL_TRIANGLES, 0, 6);
					}

					for (unsigned i = 0; i < 6; ++i) {
						glBindTextureUnit(i, 0);
					}
					
				}
				glBindFramebuffer(GL_FRAMEBUFFER, 0);
				glDisable(GL_BLEND);

				// Copy depth info into accumulation buffer
				{
					glBindFramebuffer(GL_READ_FRAMEBUFFER, gBuffer);
					glBindFramebuffer(GL_DRAW_FRAMEBUFFER, hdrFrameBuffer);
					glBlitFramebuffer(0, 0, _width, _height, 0, 0, _width, _height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
					glBindFramebuffer(GL_FRAMEBUFFER, 0);
				}

				// Draw skybox
				glBindFramebuffer(GL_FRAMEBUFFER, hdrFrameBuffer);
				for (auto c : Cubemap::allCubemaps) {
					c->draw();
				}
				glBindFramebuffer(GL_FRAMEBUFFER, 0);

				//https://learnopengl.com/code_viewer_gh.php?code=src/5.advanced_lighting/7.bloom/bloom.cpp
				//blur pass
				static bool firstRenderPass = true;
				bool horizontal = true, first = true;
				_blurPassShader->use();
				if (firstRenderPass)
					_blurPassShader->setUniform("image", 0);
				glActiveTexture(GL_TEXTURE0);

				for (int i = 0; i < _numBlurPasses; i++)
				{
					glBindFramebuffer(GL_FRAMEBUFFER, pingpong[horizontal]);
					_blurPassShader->setUniform("horizontal", horizontal);
					glBindTexture(GL_TEXTURE_2D, first ? hdrBrightBuffer : pingpongColourBuffers[!horizontal]);
					glBindVertexArray(quadVAO);
					glDrawArrays(GL_TRIANGLES, 0, 6);
					horizontal = !horizontal;
					if (first)
						first = false;
				}
				glBindFramebuffer(GL_FRAMEBUFFER, 0);

				//glBindFramebuffer(GL_READ_FRAMEBUFFER, gBuffer);
				//glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

				//
				//glBindFramebuffer(GL_FRAMEBUFFER, 0);

				//post processing
				//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				_ppShader->use();
				if (firstRenderPass) {
					_ppShader->setUniform("screenTexture", 0);
					_ppShader->setUniform("bloomTexture", 1);
					_ppShader->setUniform("lookup.LUT", 2);
				}
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, hdrColourBuffer);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, pingpongColourBuffers[!horizontal]);
				glActiveTexture(GL_TEXTURE2);
				glBindTexture(GL_TEXTURE_3D, _activeLUT->_textureID);

				glBindVertexArray(quadVAO);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				glEnable(GL_BLEND);
				glEnable(GL_SCISSOR_TEST);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, 0);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, 0);
				glActiveTexture(GL_TEXTURE2);
				glBindTexture(GL_TEXTURE_2D, 0);

				for (auto x : UserInterface::_allUI)
					x->draw();

				firstRenderPass = false;

				glDisable(GL_BLEND);
				glDisable(GL_SCISSOR_TEST);
			}

#if _DEBUG
			drawImGui(deltaTime);
#endif

			// Swap the buffers and poll events for the next frame
			CAPP_GL_CALL(glfwPollEvents());
			CAPP_GL_CALL(glfwSwapBuffers(window));

			float delay = turnRate * 1000.0f;
			auto finalDelay = (long long)(currentFrame + delay - glfwGetTime());

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
	Viewport::Viewport(const glm::vec4& borderColour, const glm::vec4& bounds, void(*specialDrawInstructionsCallback)(), const GLenum drawMode) :
		_callback(specialDrawInstructionsCallback), _borderColour(borderColour), _bounds(bounds), _drawMode(drawMode)
	{}
	
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
