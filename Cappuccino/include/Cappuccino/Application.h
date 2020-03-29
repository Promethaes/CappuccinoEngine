#pragma once

#include "Cappuccino/CappMacros.h"
#include "Cappuccino/Camera.h"
#include "Cappuccino/FontManager.h"
#include "Cappuccino/GameObject.h"
#include "Cappuccino/Input.h"
#include "Cappuccino/SceneManager.h"
#include "Cappuccino/SoundSystem.h"
#include "Cappuccino/XInputManager.h"
#include "Cappuccino/LUT.h"

#if SCENETEST
#include "Cappuccino/Testing/Test Scene.h"
#endif

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>

#include <string>

#include "PointLight.h"

namespace Cappuccino {

	class Viewport {
	public:
		Viewport(const glm::vec4& borderColour, const glm::vec4& bounds, void(*specialDrawInstructionsCallback)() = nullptr,GLenum drawMode = GL_FILL);
		void use();
	private:
		void(*_callback)();
		glm::vec4 _borderColour;
		glm::vec4 _bounds;
		GLenum _drawMode;
	};

	class Application {

		using LightVector = std::vector<PointLight>;
		
	public:

		Application();
		Application(
			GLuint             WIDTH,
			GLuint             HEIGHT,
			const std::string& TITLE,
			const std::vector<Viewport>& viewports,
			GLuint             contextVersionMajor = 4u,
			GLuint             contextVersionMinor = 6u
		);
		~Application() = default;

		void init();
		void run();

		static bool isInstantiated();

		static GLFWwindow* window;

		glm::vec4 _clearColour;
		static Shader* _shadowMappingShader;
		static Shader* _gBufferShader;
		static Shader* _lightingPassShader;
		static Shader* _blurPassShader;
		static Shader* _ppShader;
		static LUT* _activeLUT;
		static int _numBlurPasses;

		static LightVector allLights;
		
		static bool _useDeferred;
	private:

		void cleanup();

		void update(GLfloat dt);
		void drawImGui(GLfloat dt);

		static void glDebugMessageCallbackFunc(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* msg, const void* data);

		GLuint _width, _height;
		std::string _title;
		GLuint _contextVersionMajor, _contextVersionMinor;
		std::vector<Viewport> _viewports;


		static bool _instantiated;
	};

}
