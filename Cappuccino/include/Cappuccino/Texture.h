#pragma once

#include <string>

namespace Cappuccino {
	enum class TextureType {
		DiffuseMap = 0,
		SpecularMap,
		NormalMap,
		EmissionMap,
		HeightMap
	};

	/*
	The texture class is used to abstract the process of creating a texture
	*/
	class Texture {
	public:
		//texture index (for game objects) applies this texture to the mesh at that index
		//eg GameObject has 2 meshes, texture index is 1, apply this texture to the second mesh
		Texture(const std::string& name, const std::string& path, const TextureType& type, unsigned textureIndex = 0);
		/*
		Purp: loads the texture into memory
		returns: bool if it was successful or not
		*/
		bool load();
		void setLoaded(bool yn) { _loaded = yn; }
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

		unsigned getTextureIndex() const { return _textureIndex; }

		const std::string& getName() const { return _name; }

	private:
		unsigned _textureIndex = 6969;

		static std::string _textureDirectory;

		unsigned char* _data;

		std::string _path;
		std::string _name;
		unsigned _texture;
		bool _loaded = false;
	};
}