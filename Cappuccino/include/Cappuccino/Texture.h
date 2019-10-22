#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb/stb_image.h"
#include <string>
namespace Cappuccino {
	enum class TextureType {
		DiffuseMap = 0,
		SpecularMap
	};

	/*
	The texture class is used to abstract the process of creating a texture
	*/
	class Texture {
	public:
		Texture(std::string& PATH, const TextureType& type);
		Texture() = default;
		/*
		Purp: loads the texture into memory
		returns: bool if it was successful or not
		*/
		bool load();

		bool unload();

		unsigned getTextureId() const { return _texture; }

		/*
		Purp: binds the texture in the proper slot
		*/
		void bind(unsigned textureSlot) {
			glActiveTexture(GL_TEXTURE0 + textureSlot);
			glBindTexture(GL_TEXTURE_2D, _texture);
		}
		/*
		Purp: unbinds the texture in the proper slot
		*/
		void unbind(unsigned textureSlot) {
			glActiveTexture(GL_TEXTURE + textureSlot);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		TextureType type;
	private:
		unsigned char* _data;

		std::string _path;
		unsigned _texture;
	};
}