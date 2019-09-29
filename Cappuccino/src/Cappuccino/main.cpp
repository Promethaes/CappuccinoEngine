#include "Cappuccino/Application.h"
#include "Cappuccino/Scene Manager.h"
#include "Cappuccino/CappMacros.h"

#if SCENETEST
#include "Cappuccino/Testing/Test Scene.h"
#endif

using Application = Cappuccino::Application;
using SceneManager = Cappuccino::SceneManager;

#pragma region PROGRAM SETTINGS

constexpr GLuint  SCR_WIDTH = 1600;
constexpr GLuint  SCR_HEIGHT = 1200;
constexpr GLchar* SCR_TITLE = "Cappuccino Engine v1.0";

#pragma endregion

// This piece of code was given to us by Shawn Matthews
// Auto-magically changes OpenGL to use the high performance GPU to render rather than the iGPU
// (for dual GPU systems)
extern "C" {
	__declspec(dllexport) unsigned long NvOptimusEnablement = 0x01;
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 0x01;
}

int main() {
	if (!Application::isInstantiated()) {
		Application* application = new Application(SCR_WIDTH, SCR_HEIGHT, SCR_TITLE);
		application->run();
		delete application;
	}

	return 0;

}