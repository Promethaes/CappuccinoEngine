#pragma once

#include <glad/glad.h>;
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <string>

#include "Cappuccino/XinputManager.h"

namespace Cappuccino {
	class Application {
	public:

		Application();
		Application(
			GLuint       WIDTH,
			GLuint       HEIGHT,
	  const std::string& TITLE,
			GLuint       contextVersionMajor = 4u,
			GLuint       contextVersionMinor = 2u
		);
		~Application();

		static bool isInstantiated();
		void run();

		static GLFWwindow* _window;
		static Sedna::XinputManager* _xinputManager;
	private:

		void init();
		void cleanup();

		void update(GLfloat dt);
		void draw(GLfloat dt);
		void drawImGui(GLfloat dt);


		GLuint _width, _height;
		std::string _title;
		GLuint _contextVersionMajor, _contextVersionMinor;
		glm::vec4 _clearColour;


		static bool _instantiated;
	};
}
