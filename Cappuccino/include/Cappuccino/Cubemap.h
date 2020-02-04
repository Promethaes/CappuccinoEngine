#pragma once

#include "Mesh.h"
#include "ShaderProgram.h"

#include <glm/glm.hpp>

namespace Cappuccino {

	class Cubemap {
	public:

		Cubemap(const glm::vec2& windowSize, const std::vector<std::string>& filepaths);
		~Cubemap();

		void bind(unsigned cubemapSlot) const;
		void unbind(unsigned cubemapSlot) const;

		const Shader& getShader() const;
		void setCubemapTextures(const std::vector<std::string>& filepaths);

		void draw() const;

		static std::vector<Cubemap*> allCubemaps;

	private:

		Shader _shader;
		Mesh* _cubeMesh;

		unsigned _cubemap = 0;
		unsigned _size = 0;

		std::vector<std::string> _filepaths;

	};

}
