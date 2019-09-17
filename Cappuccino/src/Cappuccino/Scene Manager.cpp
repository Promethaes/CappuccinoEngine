#include "Cappuccino/Scene Manager.h"
#include <string>
namespace Cappuccino {
	std::vector<Scene*> Scene::scenes = {};
	Scene::Scene(bool firstScene){
		_active = firstScene;
		scenes.push_back(this);
	}

	void Scene::baseUpdate(float dt, Camera& defaultCamera){
		for (int i = 0; i < scenes.size(); i++) {
			if (scenes[i]->isActive()) {
				scenes[i]->childUpdate(dt,defaultCamera);
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
}