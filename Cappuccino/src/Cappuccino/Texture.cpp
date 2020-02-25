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

Texture::Texture(const std::string& name, const std::string& path, const TextureType type, const unsigned textureIndex) :
	_data(nullptr), _properties({ name, path, type, textureIndex }), _texture(0)
{
	glGenTextures(1, &_texture);

	const AssetLoader::TextureData data = AssetLoader::loadIMG(name, _textureDirectory + path, type);
	
	_width = data.width;
	_height = data.height;
	_channels = data.channels;

	load();
}

Texture::Texture(const std::string& name, const TextureType type, const unsigned width, const unsigned height, void* data, const unsigned numChannels, const unsigned textureIndex) :
	_width(width), _height(height), _channels(numChannels), _data(static_cast<unsigned char*>(data)), _texture(0)
{
	glGenTextures(1, &_texture);

	_properties.name = name;
	_properties.type = type;
	_properties.textureIndex = textureIndex;

	load();
}

bool Texture::load() {
	// Again, 100,000,000% not mine
	// I got this code from learnopengl.com: https://learnopengl.com/code_viewer_gh.php?code=src/2.lighting/5.3.light_casters_spot/light_casters_spot.cpp
	
	if (_data == nullptr) {
		stbi_image_free(_data);
		CAPP_ASSERT(_data == nullptr, "Problem loading texture \"%s\"\nTexture path: %s", _properties.name.c_str(), _properties.filepath.c_str());
		return false;
	}

	GLenum format = 0;
	switch(_channels) {
		case 1:
			format = GL_RED;
			break;
		case 2:
			format = GL_RG;
			break;
		case 3:
			format = GL_RGB;
			break;
		case 4:
			format = GL_RGBA;
			break;
		default:
			CAPP_ASSERT(format == 0, "Unsupported image format!\nTexture: \"%s\" (%s)", _properties.name.c_str(), _properties.filepath.c_str());
			break;
	}
		
	glBindTexture(GL_TEXTURE_2D, _texture);
	glTexImage2D(GL_TEXTURE_2D, 0, format, _width, _height, 0, format, GL_UNSIGNED_BYTE, _data);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTextureParameterf(_texture, GL_TEXTURE_MAX_ANISOTROPY, 16.0f);

	//delete the stored data
	stbi_image_free(_data);
	return _loaded = true;
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