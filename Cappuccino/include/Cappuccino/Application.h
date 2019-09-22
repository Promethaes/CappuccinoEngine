#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Fmod/fmod.hpp"
#include "Fmod/fmod_errors.h"

#include "Cappuccino/CappMacros.h"
#include "Cappuccino/Scene Manager.h"

namespace Cappuccino {

	class Application {
	public:

		Application(
			GLuint  width,
			GLuint  height,
			const GLchar* title,
			GLuint  contextVersionMajor = 4u,
			GLuint  contextVersionMinor = 2u
		);

		static bool isInstantiated();
		void run();


	private:

		static GLuint  _width;
		static GLuint  _height;
		static GLchar* _title;
		static GLuint  _contextVersionMajor;
		static GLuint  _contextVersionMinor;

		FMOD::System* _system = NULL;


		static bool _instantiated;

	};

}