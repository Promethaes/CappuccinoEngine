#pragma once

#include <string>

namespace Cappuccino {
	enum class TextureType : unsigned int {
		DiffuseMap = 0,
		SpecularMap,
		NormalMap,
		EmissionMap,
		HeightMap,
		PBRAlbedo,
		PBRNormal,
		PBRMetallic,
		PBRRoughness,
		PBRAmbientOcc,
		None
	};

	struct TextureProperties {
		std::string name;
		std::string filepath;
		TextureType type = TextureType::None;
		unsigned textureIndex = 0;
	};
	
	/*
	The texture class is used to abstract the process of creating a texture
	*/
	class Texture {
	public:
		//texture index (for game objects) applies this texture to the mesh at that index
		//eg GameObject has 2 meshes, texture index is 1, apply this texture to the second mesh
		Texture(const std::string& name, const std::string& path, TextureType type, unsigned textureIndex = 0);
		Texture(const std::string& name, TextureType type, unsigned width, unsigned height, void* data, unsigned numChannels = 3, unsigned textureIndex = 0);

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

		TextureProperties getProperties() const { return _properties; }
		
		const std::string& getName() const { return _properties.name; }
		TextureType getType() const { return _properties.type; }
		unsigned getTextureIndex() const { return _properties.textureIndex; }

		void setName(const std::string& name) { _properties.name = name; }

	private:

		static std::string _textureDirectory;

		unsigned _width, _height, _channels;
		unsigned char* _data;

		TextureProperties _properties;
		unsigned _texture;
		
		bool _loaded = false;
	};
}