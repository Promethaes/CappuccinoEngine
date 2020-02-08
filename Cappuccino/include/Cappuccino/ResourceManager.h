#pragma once

#include "Mesh.h"
#include "ShaderProgram.h"
#include "Texture.h"

#include <future>
#include <string>
#include <unordered_map>

namespace Cappuccino {

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

		static ShaderMap _shaders;
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

		static std::mutex _meshMapMutex;
		static FutureVector _futures;
		static MeshMap _meshes;
		
		static bool _initialized;

	};

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

		static std::unordered_map<std::string, Texture*> _textures;
		static bool _initialized;

	};

	
	
	class ResourceManager {
	public:

		static void init() {
			MeshLibrary::init();
			ShaderLibrary::init();
			TextureLibrary::init();
		}

		static void shutdown() {
			MeshLibrary::shutdown();
			ShaderLibrary::shutdown();
			TextureLibrary::shutdown();
		}
	};
}
