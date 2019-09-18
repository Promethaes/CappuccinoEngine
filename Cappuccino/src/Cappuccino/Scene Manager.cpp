#include "Cappuccino/Scene Manager.h"
#include "Cappuccino/CappMacros.h"
#include <string>
#define Scenes Scene::scenes
namespace Cappuccino {

	std::vector<Scene*> Scene::scenes = {};
	Camera* Scene::defaultCamera = new Camera();
	Scene::Scene(bool firstScene){
		_active = firstScene;
		scenes.push_back(this);
	}

	void Scene::baseUpdate(float dt){
		for (int i = 0; i < scenes.size(); i++) {
			if (scenes[i]->isActive()) {
				scenes[i]->childUpdate(dt);
				for (int j = 0; j < scenes.size(); j++) {
					if (i == j)
						continue;
					scenes[j]->_active = false;
				}
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
			if (!Scenes[i]->isInit())
				Scenes[i]->init();
#if NETWORKTEST
			Scenes[i]->sendString(network.listen(true));
#endif

			if (Scenes[i]->isInit())
				Scenes[i]->baseUpdate(dt);
		}
	}
}