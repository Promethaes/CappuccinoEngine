#pragma once

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>

#include <string>

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


		GLuint _width, _height;
		std::string _title;
		GLuint _contextVersionMajor, _contextVersionMinor;


		static bool _instantiated;
	};
	
}
