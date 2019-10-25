#pragma once
#include "Cappuccino/ShaderProgram.h"
#include "Cappuccino/Mesh.h"
#include "Cappuccino/Texture.h"

#include <vector>


namespace Cappuccino {
	class ResourceManager {
	public:

		static std::vector<Shader*>_allShaders;
		static std::vector<Mesh*>_allMeshes;
		static std::vector<Texture*>_allTextures;

	};
}