#include "Cappuccino/Cubemap.h"

#include "Cappuccino/Camera.h"
#include "Cappuccino/CappMacros.h"
#include "Cappuccino/ResourceManager.h"

#include <glad/glad.h>
#include <stb/stb_image.h>

using namespace Cappuccino;

std::vector<Cubemap*> Cubemap::allCubemaps = {};

Cubemap::Cubemap(const glm::vec2& windowSize, const std::vector<std::string>& filepaths) :
	_filepaths(filepaths) {
	glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &_cubemap);
	setCubemapTextures(filepaths);

	_cubeMesh = new Mesh("Skybox", "Skybox.obj");
	_cubeMesh->loadMesh();
	
	_shader = *ShaderLibrary::loadShader("Skybox", "cubemap.vert", "cubemap.frag");
	_shader.use();
	_shader.loadProjectionMatrix(windowSize.x, windowSize.y);
	allCubemaps.push_back(this);
}

Cubemap::~Cubemap() {
	glDeleteTextures(1, &_cubemap);
	_cubeMesh->unload();
}

void Cubemap::bind(const unsigned cubemapSlot) const {
	glBindTextureUnit(cubemapSlot, _cubemap);
}
void Cubemap::unbind(const unsigned cubemapSlot) const {
	glBindTextureUnit(cubemapSlot, 0);
}

void Cubemap::setCubemapTextures(const std::vector<std::string>& filepaths) {
	CAPP_ASSERT(filepaths.size() == 6, "Cubemap must load in 6 faces!");

	stbi_set_flip_vertically_on_load(true);
	for(unsigned i = 0; i < filepaths.size(); ++i) {
		int width, height, channels;
		unsigned char* data = stbi_load(filepaths[i].c_str(), &width, &height, &channels, 0);

		if(!data) {
			CAPP_ASSERT(data != nullptr, "Failed to load texture for texture cube!\nTexture path: %s", filepaths[i].c_str());
			stbi_image_free(data);
		}

		if(_size != 0 && (static_cast<unsigned>(width) != _size || static_cast<unsigned>(height) != _size)) {
			CAPP_ASSERT(_size == 0 || static_cast<unsigned>(width) == _size && static_cast<unsigned>(height) == _size, "Texture image file dimensions do not match the size of this cubemap!\nTexture path: %s", filepaths[i].c_str());
			stbi_image_free(data);
		}

		if(width != height) {
			CAPP_ASSERT(width == height, "Texture image for cubemap must be square!\nTexture path: %s", filepaths[i].c_str());
			stbi_image_free(data);
		}

		if(i == 0) {
			_size = width;

			glTextureParameteri(_cubemap, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTextureParameteri(_cubemap, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTextureParameteri(_cubemap, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTextureParameteri(_cubemap, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTextureParameteri(_cubemap, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTextureStorage2D(_cubemap, 1, GL_RGB8, _size, _size);
		}
		
		glTextureSubImage3D(_cubemap, 0, 0, 0,
		                    static_cast<unsigned>(i),
		                    _size, _size, 1,
		                    channels == 1 ? GL_RED : channels == 3 ? GL_RGB : GL_RGBA,
		                    GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}
}


const Shader& Cubemap::getShader() const { return _shader; }

void Cubemap::draw() const {
	glDisable(GL_CULL_FACE);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_FALSE);

	bind(25);
	_shader.use();
	_shader.setUniform("skybox", 25);

	_cubeMesh->draw();

	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
}

