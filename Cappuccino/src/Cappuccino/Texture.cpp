#include "Cappuccino/Texture.h"
#include "Cappuccino/CappMacros.h"
#include "Cappuccino/ResourceManager.h"

#include <iostream>
namespace Cappuccino {
	Texture::Texture(std::string& PATH, const TextureType& ID)
	{
		_path = PATH;
		type = ID;
		load();
		ResourceManager::_allTextures.push_back(this);
	}
	bool Texture::load() {
		//again, one hundred million % not mine
		//i got this code from learn opengl.com https://learnopengl.com/code_viewer_gh.php?code=src/2.lighting/5.3.light_casters_spot/light_casters_spot.cpp
		unsigned textureID;
		glGenTextures(1, &textureID);

		int width, height, nrComponents;
		_data = stbi_load(_path.c_str(), &width, &height, &nrComponents, 0);
		//stbi_set_flip_vertically_on_load(true);
		if (_data)
		{
			GLenum format;
			if (nrComponents == 1)
				format = GL_RED;
			else if (nrComponents == 3)
				format = GL_RGB;
			else if (nrComponents == 4)
				format = GL_RGBA;

			glBindTexture(GL_TEXTURE_2D, textureID);
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, _data);
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			//delete the stored _data
			stbi_image_free(_data);
		}
		else
		{
			CAPP_PRINT_ERROR("Problem loading Texture at %s", _path.c_str());
			stbi_image_free(_data);
			SYS_EXIT(0);
			return false;
		}

		_texture = textureID;
		loaded = true;
		return true;
	}
	bool Texture::unload()
	{
		stbi_image_free(_data);

		return true;
	}
}