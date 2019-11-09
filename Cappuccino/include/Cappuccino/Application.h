#pragma once

#include "Cappuccino/CappMacros.h"
#include "Cappuccino/Camera.h"
#include "Cappuccino/FontManager.h"
#include "Cappuccino/GameObject.h"
#include "Cappuccino/Input.h"
#include "Cappuccino/SceneManager.h"
#include "Cappuccino/SoundSystem.h"
#include "Cappuccino/XInputManager.h"

#if SCENETEST
#include "Cappuccino/Testing/Test Scene.h"
#endif

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>

#include <string>

namespace Cappuccino {
	
	class Application {
	public:

		Application();
		Application(
			GLuint             WIDTH,
			GLuint             HEIGHT,
			const std::string& TITLE,
			GLuint             contextVersionMajor = 4u,
			GLuint             contextVersionMinor = 6u
		);
		~Application() = default;

		void init();
		void run();

		static bool isInstantiated();

		static GLFWwindow* window;

		glm::vec4 _clearColour;
	private:

		void cleanup();

		void update(GLfloat dt);
		void drawImGui(GLfloat dt);

		static void glDebugMessageCallbackFunc(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* msg, const void* data);
		
		GLuint _width, _height;
		std::string _title;
		GLuint _contextVersionMajor, _contextVersionMinor;


		static bool _instantiated;
	};
	
}
