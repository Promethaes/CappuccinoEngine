#include "Cappuccino/ResourceManager.h"

namespace Cappuccino {

	std::vector<Shader*>  ResourceManager::_allShaders =  {};
	std::vector<Mesh*>    ResourceManager::_allMeshes =   {};
	std::vector<Texture*> ResourceManager::_allTextures = {};
}