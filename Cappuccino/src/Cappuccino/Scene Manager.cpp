#include "Cappuccino/Scene Manager.h"
#include "Cappuccino/CappMacros.h"
#include "Cappuccino/Application.h"
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define Scenes Cappuccino::Scene::scenes


namespace Cappuccino {
	void internalMouseCallback(GLFWwindow* window, double xpos, double ypos) {
		for (auto x : Scenes)
			if (x->isActive())
				x->mouseFunction(xpos, ypos);
	}
	std::vector<Scene*> Scene::scenes = {};
	Camera* Scene::defaultCamera = new Camera();
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
				scenes[i]->childUpdate(dt);
				break;
			}
		}
	}
	void Scene::sendString(const std::string& info)
	{
		this->info = info;
	}
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
}