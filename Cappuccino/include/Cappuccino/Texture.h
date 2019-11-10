#pragma once

#include <string>

namespace Cappuccino {
	enum class TextureType {
		DiffuseMap = 0,
		SpecularMap,
		NormalMap
	};

	/*
	The texture class is used to abstract the process of creating a texture
	*/
	class Texture {
	public:
		Texture(const std::string& PATH, const TextureType& type);
		Texture() = default;
		/*
		Purp: loads the texture into memory
		returns: bool if it was successful or not
		*/
		bool load();
		bool isLoaded() const { return _loaded; }

		bool unload();

		unsigned getTextureId() const { return _texture; }

		/*
		 * Purp.: Changes the directory in which the engine looks for the texture files
		 * Req.:
		 *     directory: the directory in which to look for, use "default" to look in default directory ( %CappuccinoPath%\Assets\Textures\ )
		 * Returns: Nothing
		 */
		static void setDefaultPath(const std::string& directory);
		
		/*
		Purp: binds the texture in the proper slot
		*/
		void bind(unsigned textureSlot) const;
		
		/*
		Purp: unbinds the texture in the proper slot
		*/
		void unbind(unsigned textureSlot) const;
		
		TextureType type;
		
	private:
		static std::string _textureDirectory;
		
		unsigned char* _data;

		std::string _path;
		unsigned _texture;
		bool _loaded = false;
	};
}