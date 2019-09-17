#include "Cappuccino/Application.h"

using Application = Cappuccino::Application;

#pragma region PROGRAM SETTINGS

constexpr GLuint  SCR_WIDTH  = 800*2;
constexpr GLuint  SCR_HEIGHT = 600*2;
constexpr GLchar* SCR_TITLE  = static_cast<GLchar*>("Cappuccino Engine v1.0");

#pragma endregion

int main() {

	if (!Application::isInstantiated()) {
		Application* application = new Application(SCR_WIDTH, SCR_HEIGHT, SCR_TITLE);
		application->run();
	}
	
	return 0;

	#pragma	endregion
}
