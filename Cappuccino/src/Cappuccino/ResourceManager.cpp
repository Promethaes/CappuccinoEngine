#include "Cappuccino/ResourceManager.h"

#include "Cappuccino/CappMacros.h"

using namespace Cappuccino;

// ----------------------------------------------------------------------------------------
// ----- Shader library -------------------------------------------------------------------
// ----------------------------------------------------------------------------------------

std::unordered_map<std::string, Shader*> ShaderLibrary::_shaders;
bool ShaderLibrary::_initialized = false;

void ShaderLibrary::init() {
	if(_initialized) {
		CAPP_ASSERT(!_initialized, "Shader library already initialized!");
		return;
	}

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

std::unordered_map<std::string, Mesh*> MeshLibrary::_meshes;
bool MeshLibrary::_initialized = false;

void MeshLibrary::init() {
	if(_initialized) {
		CAPP_ASSERT(!_initialized, "Mesh library already initialized!");
		return;
	}

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

	return _meshes[name] = new Mesh(name, filepath);
}

Mesh* MeshLibrary::getMesh(const std::string& name) {
	if(!hasMesh(name)) {
		CAPP_ASSERT(hasMesh(name), "Mesh \"%s\" not found!", name.c_str());
		return nullptr;
	}

	return _meshes[name];
}

bool MeshLibrary::hasMesh(const std::string& name) { return _meshes.find(name) != _meshes.end(); }


// ----------------------------------------------------------------------------------------
// ----- Texture library ------------------------------------------------------------------
// ----------------------------------------------------------------------------------------

std::unordered_map<std::string, Texture*> TextureLibrary::_textures;
bool TextureLibrary::_initialized = false;

void TextureLibrary::init() {
	if(_initialized) {
		CAPP_ASSERT(!_initialized, "Texture library already initialized!");
		return;
	}

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