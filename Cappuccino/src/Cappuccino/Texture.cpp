#include "Cappuccino/Texture.h"

#include "Cappuccino/CappMacros.h"
#include "Cappuccino/ResourceManager.h"

#include <glad/glad.h>
#include <stb/stb_image.h>

#include <iostream>
#include <algorithm>

using string = std::string;
using namespace Cappuccino;

string Texture::_textureDirectory = "./Assets/Textures/";

Texture::Texture(const std::string& name, const std::string& path, const TextureType& type, unsigned textureIndex) :
	type(type), _textureIndex(textureIndex), _data(nullptr), _path(path), _name(name), _texture(0) {
	load();
}

bool Texture::load() {
	// Again, 100,000,000% not mine
	// I got this code from learnopengl.com: https://learnopengl.com/code_viewer_gh.php?code=src/2.lighting/5.3.light_casters_spot/light_casters_spot.cpp
	glGenTextures(1, &_texture);	
	stbi_set_flip_vertically_on_load(true);

	int width, height, nrComponents;
	_data = stbi_load(std::string(_textureDirectory + _path).c_str(), &width, &height, &nrComponents, 0);
	
	if (_data) {
		GLenum format = 0;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, _texture);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, _data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTextureParameterf(_texture, GL_TEXTURE_MAX_ANISOTROPY, 16.0f);

		//delete the stored data
		stbi_image_free(_data);
	}
	else {
		CAPP_PRINT_ERROR("Problem loading texture at %s", string(_textureDirectory + _path).c_str());
		stbi_image_free(_data);
		SYS_EXIT(0);
	}

	_loaded = true;
	return true;
}

bool Texture::unload() {
	stbi_image_free(_data);

	return true;
}

void Texture::setDefaultPath(const string& directory) {
	string dir = directory;
	std::transform(dir.begin(), dir.end(), dir.begin(), ::tolower);

	if(dir == "default")
		_textureDirectory = "./Assets/Textures/";
	else
		_textureDirectory = directory;
}

void Texture::bind(const unsigned textureSlot) const {
	glActiveTexture(GL_TEXTURE0 + textureSlot);
	glBindTexture(GL_TEXTURE_2D, _texture);
}

void Texture::unbind(const unsigned textureSlot) const {
	glActiveTexture(GL_TEXTURE0 + textureSlot);
	glBindTexture(GL_TEXTURE_2D, 0);
}