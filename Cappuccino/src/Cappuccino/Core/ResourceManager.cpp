#include "Cappuccino/Core/ResourceManager.h"
#include "Cappuccino/Core/CappMacros.h"

using namespace Cappuccino;

// ----------------------------------------------------------------------------------------
// ----- Shader library -------------------------------------------------------------------
// ----------------------------------------------------------------------------------------

ShaderLibrary::ShaderMap ShaderLibrary::_shaders;
bool ShaderLibrary::_initialized = false;

void ShaderLibrary::init() {
	if(_initialized) {
		CAPP_PRINT_WARNING("Shader library already initialized!");
		return;
	}

	_shaders.reserve(1000);
	_initialized = true;
}

void ShaderLibrary::shutdown() {
	for(const auto& shader : _shaders) {
		delete shader.second;
	}
	_shaders.clear();
}

void ShaderLibrary::addShader(const std::string& name, Shader* shader) {
	if(hasShader(name)) {
		CAPP_PRINT_WARNING("Shader \"%s\" already exists!", name.c_str());
		return;
	}

	_shaders[name] = shader;
}

Shader* ShaderLibrary::loadShader(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath, const std::optional<std::string>& geometryPath) {
	if(hasShader(name)) {
		CAPP_PRINT_WARNING("Shader \"%s\" already exists, returning loaded shader", name.c_str());
		return _shaders[name];
	}

	return _shaders[name] = new Shader(name, vertexPath, fragmentPath, geometryPath);
}

Shader* ShaderLibrary::getShader(const std::string& name) {
	if(!hasShader(name)) {
		CAPP_ASSERT(hasShader(name), "Shader \"%s\" not found!", name.c_str());
		return nullptr;
	}

	return _shaders[name];
}

bool ShaderLibrary::hasShader(const std::string& name) { return _shaders.find(name) != _shaders.end(); }

// ----------------------------------------------------------------------------------------
// ----- Mesh library ---------------------------------------------------------------------
// ----------------------------------------------------------------------------------------

std::mutex MeshLibrary::_meshMapMutex;
MeshLibrary::FutureVector MeshLibrary::_futures;
MeshLibrary::MeshMap MeshLibrary::_meshes;

bool MeshLibrary::_initialized = false;

void MeshLibrary::init() {
	if(_initialized) {
		CAPP_PRINT_WARNING("Mesh library already initialized!");
		return;
	}

	_futures.reserve(1000);
	_meshes.reserve(1000);
	_initialized = true;
}

void MeshLibrary::shutdown() {
	for(const auto& mesh : _meshes) {
		delete mesh.second;
	}
	_meshes.clear();
}

void MeshLibrary::addMesh(const std::string& name, Mesh* mesh) {
	if(hasMesh(name)) {
		CAPP_PRINT_WARNING("Mesh \"%s\" already exists!", name.c_str());
		return;
	}

	_meshes[name] = mesh;
}

Mesh* MeshLibrary::loadMesh(const std::string& name, const std::string& filepath) {
	if(hasMesh(name)) {
		CAPP_PRINT_WARNING("Mesh \"%s\" already exists, returning loaded mesh", name.c_str());
		return _meshes[name];
	}

	_futures.push_back(std::async(std::launch::async, loadMeshImpl, &_meshes, name, filepath));
	return _meshes[name];
}

Mesh* MeshLibrary::getMesh(const std::string& name) {
	if(!hasMesh(name)) {
		CAPP_ASSERT(hasMesh(name), "Mesh \"%s\" not found!", name.c_str());
		return nullptr;
	}

	return _meshes[name];
}

void MeshLibrary::loadMeshImpl(std::unordered_map<std::string, Mesh*>* map, const std::string& name, const std::string& filepath) {
	CAPP_PRINT_N("Loading mesh \"%s\" from filepath \"%s\"...", name.c_str(), filepath.c_str());
	auto mesh = new Mesh(name, filepath);
	
	std::lock_guard<std::mutex> lock(_meshMapMutex);
	map->insert({ name, mesh });
}


bool MeshLibrary::hasMesh(const std::string& name) { return _meshes.find(name) != _meshes.end(); }


// ----------------------------------------------------------------------------------------
// ----- Texture library ------------------------------------------------------------------
// ----------------------------------------------------------------------------------------

std::unordered_map<std::string, Texture*> TextureLibrary::_textures;
bool TextureLibrary::_initialized = false;

void TextureLibrary::init() {
	if(_initialized) {
		CAPP_PRINT_WARNING("Texture library already initialized!");
		return;
	}

	_textures.reserve(100000);
	_initialized = true;
}

void TextureLibrary::shutdown() {
	for(const auto& texture : _textures) {
		delete texture.second;
	}
	_textures.clear();
}

void TextureLibrary::addTexture(const std::string& name, Texture* texture) {
	if(hasTexture(name)) {
		CAPP_PRINT_WARNING("Texture \"%s\" already exists!", name.c_str());
		return;
	}

	_textures[name] = texture;
}

Texture* TextureLibrary::loadTexture(const std::string& name, const std::string& filepath, const TextureType type, const unsigned textureIndex) {
	if(hasTexture(name)) {
		CAPP_PRINT_WARNING("Texture \"%s\" already exists, returning loaded texture", name.c_str());
		return _textures[name];
	}

	return _textures[name] = new Texture(name, filepath, type, textureIndex);
}

Texture* TextureLibrary::getTexture(const std::string& name) {
	if(!hasTexture(name)) {
		CAPP_ASSERT(hasTexture(name), "Texture \"%s\" not found!", name.c_str());
		return nullptr;
	}

	return _textures[name];
}

bool TextureLibrary::hasTexture(const std::string& name) { return _textures.find(name) != _textures.end(); }