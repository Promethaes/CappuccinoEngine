#include "Cappuccino/Texture.h"

#include <iostream>
namespace Cappuccino {
	Texture::Texture(std::string PATH, const TextureType& ID)
	{
		_path = PATH;
		type = ID;
		load();
		/////<if it wasn't obvious, a lot of this code isn't mine, but this entire section is ACTUALLY copy and pasted to shit so...not like other sections aren't but yeah>
		//glGenTextures(1, &texture);
		//glBindTexture(GL_TEXTURE_2D, texture);
		//
		//// set the texture wrapping/filtering options (on the currently bound texture object)
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//
		//
		//// load and generate the texture
		//int width;
		//int height;
		//int nrChannels;
		//auto data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
		//if (data) {
		//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB , GL_UNSIGNED_BYTE, data);
		//	glGenerateMipmap(GL_TEXTURE_2D);
		//}
		//else
		//	std::cout << "Failed to load texture" << std::endl;
		//
		//stbi_image_free(data);
	}
	bool Texture::load() {
		//again, one hundred million % not mine
		//i got this code from learn opengl.com https://learnopengl.com/code_viewer_gh.php?code=src/2.lighting/5.3.light_casters_spot/light_casters_spot.cpp
		unsigned int textureID;
		glGenTextures(1, &textureID);

		//load the file into memory
		int width, height, nrComponents;
		unsigned char* data = stbi_load(_path.c_str(), &width, &height, &nrComponents, 0);
		if (data)
		{
			GLenum format;
			if (nrComponents == 1)
				format = GL_RED;
			else if (nrComponents == 3)
				format = GL_RGB;
			else if (nrComponents == 4)
				format = GL_RGBA;

			//set up the proper openGL settings
			glBindTexture(GL_TEXTURE_2D, textureID);
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			//delete the stored data
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Texture failed to load at path: " << _path << std::endl;
			stbi_image_free(data);
			return false;
		}

		_texture = textureID;
		return true;
	}
}