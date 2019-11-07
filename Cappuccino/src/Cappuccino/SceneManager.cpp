#include "Cappuccino/SceneManager.h"

#include "Cappuccino/CappMacros.h"
#include <glfw/glfw3.h>
#include <string>

#define Scenes Cappuccino::Scene::scenes

using namespace Cappuccino;

std::vector<Scene*> Scene::scenes = {};
Camera* Scene::defaultCamera = new Camera();


void internalMouseCallback(GLFWwindow* window, double xpos, double ypos) {
	for (auto x : Scenes)
		if (x->isActive())
			x->mouseFunction(xpos, ypos);
}
void internalClickCallback(GLFWwindow* window, int button, int action, int mods) {
	for (auto x : Scenes)
		if (x->isActive())
			x->clickFunction(button,action,mods);
}


Scene::Scene(bool firstScene) {
	_active = firstScene;
	scenes.push_back(this);
}

void Scene::baseUpdate(float dt) {
	for (int i = 0; i < scenes.size(); i++) {
		if (scenes[i]->isActive()) {
			for (int j = 0; j < scenes.size(); j++) {
				if (i == j)
					continue;
				scenes[j]->_active = false;
			}
			glfwSetCursorPosCallback(glfwGetCurrentContext(), internalMouseCallback);
			glfwSetMouseButtonCallback(glfwGetCurrentContext(), internalClickCallback);
			scenes[i]->childUpdate(dt);
			break;
		}
	}
}

void Scene::sendString(const std::string& info)
{
	this->info = info;
}

void Scene::clickFunction(int button, int action, int mods) {}

void SceneManager::updateScenes(float dt)
{
	for (unsigned i = 0; i < Scenes.size(); i++) {
		if (Scenes[i]->isActive())
			Scenes[i]->baseUpdate(dt);
	}
}

void SceneManager::changeScene(unsigned changeToIndex)
{
	for (unsigned i = 0; i < Scene::scenes.size(); i++) {
		if (Scenes[i]->isActive()) {
			Scenes[i]->setActive(false);
			Scenes[i]->exit();
			break;
		}
	}

	Scenes[changeToIndex]->init();

	Scenes[changeToIndex]->setActive(true);
}