#pragma once

#include "Mesh.h"
#include "ShaderProgram.h"
#include "Texture.h"

#include <future>
#include <string>
#include <unordered_map>

namespace Cappuccino {

	class ResourceManager;

	// ----------------------------------------------------------------------------------------
	// ----- Shader library -------------------------------------------------------------------
	// ----------------------------------------------------------------------------------------
	
	class ShaderLibrary {

		using ShaderMap = std::unordered_map<std::string, Shader*>;
		
	public:

		static void init();
		static void shutdown();

		static void addShader(const std::string& name, Shader* shader);
		static Shader* loadShader(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath, const std::optional<std::string>& geometryPath = std::nullopt);
		static Shader* getShader(const std::string& name);

	private:

		static bool hasShader(const std::string& name);

		static std::unordered_map<std::string, Shader*> _shaderMap;
		static bool _initialized;
	};

	// ----------------------------------------------------------------------------------------
	// ----- Mesh library ---------------------------------------------------------------------
	// ----------------------------------------------------------------------------------------
	
	class MeshLibrary {

		using FutureVector = std::vector<std::future<void>>;
		using MeshMap = std::unordered_map<std::string, Mesh*>;
		
	public:

		static void init();
		static void shutdown();

		static void addMesh(const std::string& name, Mesh* mesh);
		static Mesh* loadMesh(const std::string& name, const std::string& filepath);
		static Mesh* getMesh(const std::string& name);

	private:

		static void loadMeshImpl(std::unordered_map<std::string, Mesh*>* map, const std::string& name, const std::string& filepath);
		static bool hasMesh(const std::string& name);

		static std::unordered_map<std::string, Mesh*> _meshMap;
		static bool _initialized;

	};
	
	// ----------------------------------------------------------------------------------------
	// ----- Texture library ------------------------------------------------------------------
	// ----------------------------------------------------------------------------------------

	// ----------------------------------------------------------------------------------------
	// ----- Texture library ------------------------------------------------------------------
	// ----------------------------------------------------------------------------------------
	
	class TextureLibrary {
	public:

		static void init();
		static void shutdown();

		static void addTexture(const std::string& name, Texture* texture);
		static Texture* loadTexture(const std::string& name, const std::string& filepath, TextureType type, unsigned textureIndex = 0);
		static Texture* getTexture(const std::string& name);

	private:

		static bool hasTexture(const std::string& name);

		static std::unordered_map<std::string, Texture*> _textureMap;
		static bool _initialized;

	};

	// ----------------------------------------------------------------------------------------
	// ----- Asset loader ---------------------------------------------------------------------
	// ----------------------------------------------------------------------------------------	
	
	class AssetLoader {

		friend ResourceManager;

	public:

		struct MeshData {
			std::string name;
			unsigned numFaces = 0;
			std::vector<float> vertices;
			std::vector<float> uvCoords;
			std::vector<float> normals;
			std::vector<float> tangents;
		};
		
		static MeshData loadOBJ(std::string name, std::string filepath);

		struct TextureData {
			std::string name;
			TextureType type = TextureType::None;
			unsigned width = 0;
			unsigned height = 0;
			unsigned channels = 0;
			unsigned char* data = nullptr;
			unsigned textureIndex = 0;
		};
		
		static TextureData loadIMG(std::string name, std::string filepath, TextureType textureType, unsigned textureIndex = 0);

		//struct ShaderData {
		//	std::string name;
		//	std::string vert;
		//	std::string frag;
		//	std::optional<std::string> geom;
		//};
		
	private:
		static std::mutex _textureMutex;
		
	};

	// ----------------------------------------------------------------------------------------
	// ----- Resource manager -----------------------------------------------------------------
	// ----------------------------------------------------------------------------------------

	enum class ResourceType : unsigned int {
		Mesh = 0,
		Texture
	};

	class ResourceManager {

		using MeshFutureVector = std::vector<std::future<AssetLoader::MeshData>>;
		using TextureFutureVector = std::vector<std::future<AssetLoader::TextureData>>;
		//using ShaderFutureVector = std::vector<std::future<AssetLoader::ShaderData>>;

	public:

		static void init();
		static void shutdown();

		static void loadAll();
		static void loadAssetFile(const std::string& filepath, ResourceType type);

	private:

		static MeshFutureVector _meshFutures;
		static TextureFutureVector _textureFutures;
		//static ShaderFutureVector _shaderFutures;

	};
	
}
