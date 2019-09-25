#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
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

		static bool isInstantiated();
		void run();


	private:

		void init();
		void cleanup();

		void update(GLfloat dt);
		void draw(GLfloat dt);


		GLFWwindow* _window;
		GLuint _width, _height;
		std::string _title;
		GLuint _contextVersionMajor, _contextVersionMinor;
		glm::vec4 _clearColour;


		static bool _instantiated;

	};

}
