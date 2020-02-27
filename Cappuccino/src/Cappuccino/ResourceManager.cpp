#include "Cappuccino/ResourceManager.h"
#include "Cappuccino/CappMacros.h"

#include <fstream>
#include <sstream>
#include <utility>
#include <stb/stb_image.h>

using namespace Cappuccino;

// ----------------------------------------------------------------------------------------
// ----- Structs --------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------

struct FaceData {
	unsigned vertexData[3];
	unsigned textureData[3];
	unsigned normalData[3];
};

// ----------------------------------------------------------------------------------------
// ----- Shader library -------------------------------------------------------------------
// ----------------------------------------------------------------------------------------

std::unordered_map<std::string, Shader*> ShaderLibrary::_shaderMap;
bool ShaderLibrary::_initialized = false;

void ShaderLibrary::init() {
	if(_initialized) {
		CAPP_PRINT_WARNING("Shader library already initialized!");
		return;
	}

	_shaderMap.reserve(1000);
	_initialized = true;
}

void ShaderLibrary::shutdown() {
	for(const auto& shader : _shaderMap) {
		delete shader.second;
	}
	_shaderMap.clear();
}

void ShaderLibrary::addShader(const std::string& name, Shader* shader) {
	if(hasShader(name)) {
		CAPP_PRINT_WARNING("Shader \"%s\" already exists!", name.c_str());
		return;
	}

	_shaderMap[name] = shader;
}

Shader* ShaderLibrary::loadShader(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath, const std::optional<std::string>& geometryPath) {
	if(hasShader(name)) {
		CAPP_PRINT_WARNING("Shader \"%s\" already exists, returning loaded shader", name.c_str());
		return _shaderMap[name];
	}

	return _shaderMap[name] = new Shader(name, vertexPath, fragmentPath, geometryPath);
}

Shader* ShaderLibrary::getShader(const std::string& name) {
	if(!hasShader(name)) {
		CAPP_ASSERT(hasShader(name), "Shader \"%s\" not found!", name.c_str());
		return nullptr;
	}

	return _shaderMap[name];
}

bool ShaderLibrary::hasShader(const std::string& name) { return _shaderMap.find(name) != _shaderMap.end(); }

// ----------------------------------------------------------------------------------------
// ----- Mesh library ---------------------------------------------------------------------
// ----------------------------------------------------------------------------------------

std::unordered_map<std::string, Mesh*> MeshLibrary::_meshMap;
bool MeshLibrary::_initialized = false;

void MeshLibrary::init() {
	if(_initialized) {
		CAPP_PRINT_WARNING("Mesh library already initialized!");
		return;
	}

	_meshMap.reserve(1000);
	_initialized = true;
}

void MeshLibrary::shutdown() {
	for(const auto& mesh : _meshMap) {
		delete mesh.second;
	}
	_meshMap.clear();
}

void MeshLibrary::addMesh(const std::string& name, Mesh* mesh) {
	if(hasMesh(name)) {
		CAPP_PRINT_WARNING("Mesh \"%s\" already exists!", name.c_str());
		return;
	}

	_meshMap[name] = mesh;
}

Mesh* MeshLibrary::loadMesh(const std::string& name, const std::string& filepath) {
	if(hasMesh(name)) {
		CAPP_PRINT_WARNING("Mesh \"%s\" already exists, returning loaded mesh", name.c_str());
		return _meshMap[name];
	}

	auto mesh = new Mesh(name, filepath);
	mesh->loadMesh();
	return _meshMap[name] = mesh;
}

Mesh* MeshLibrary::getMesh(const std::string& name) {
	if(!hasMesh(name)) {
		CAPP_ASSERT(hasMesh(name), "Mesh \"%s\" not found!", name.c_str());
		return nullptr;
	}

	return _meshMap[name];
}

bool MeshLibrary::hasMesh(const std::string& name) { return _meshMap.find(name) != _meshMap.end(); }


// ----------------------------------------------------------------------------------------
// ----- Texture library ------------------------------------------------------------------
// ----------------------------------------------------------------------------------------

std::unordered_map<std::string, Texture*> TextureLibrary::_textureMap;
bool TextureLibrary::_initialized = false;

void TextureLibrary::init() {
	if(_initialized) {
		CAPP_PRINT_WARNING("Texture library already initialized!");
		return;
	}

	_textureMap.reserve(10000);
	_initialized = true;
}

void TextureLibrary::shutdown() {
	for(const auto& texture : _textureMap) {
		delete texture.second;
	}
	_textureMap.clear();
}

void TextureLibrary::addTexture(const std::string& name, Texture* texture) {
	if(hasTexture(name)) {
		CAPP_PRINT_WARNING("Texture \"%s\" already exists!", name.c_str());
		return;
	}

	_textureMap[name] = texture;
}

Texture* TextureLibrary::loadTexture(const std::string& name, const std::string& filepath, const TextureType type, const unsigned textureIndex) {
	if(hasTexture(name)) {
		CAPP_PRINT_WARNING("Texture \"%s\" already exists, returning loaded texture", name.c_str());
		return _textureMap[name];
	}

	return _textureMap[name] = new Texture(name, filepath, type, textureIndex);
}

Texture* TextureLibrary::getTexture(const std::string& name) {
	if(!hasTexture(name)) {
		CAPP_ASSERT(hasTexture(name), "Texture \"%s\" not found!", name.c_str());
		return nullptr;
	}

	return _textureMap[name];
}

bool TextureLibrary::hasTexture(const std::string& name) { return _textureMap.find(name) != _textureMap.end(); }

// ----------------------------------------------------------------------------------------
// ----- Resource manager -----------------------------------------------------------------
// ----------------------------------------------------------------------------------------

ResourceManager::MeshFutureVector ResourceManager::_meshFutures;
ResourceManager::TextureFutureVector ResourceManager::_textureFutures;

void ResourceManager::init() {
	_meshFutures.reserve(1000);
	_textureFutures.reserve(1000);
	
	MeshLibrary::init();
	ShaderLibrary::init();
	TextureLibrary::init();
}

void ResourceManager::shutdown() {
	_meshFutures.clear();
	_textureFutures.clear();
	
	MeshLibrary::shutdown();
	ShaderLibrary::shutdown();
	TextureLibrary::shutdown();
}

void ResourceManager::loadAll() {
	for(unsigned i = 0; i < _meshFutures.size(); ++i) {
		AssetLoader::MeshData data = _meshFutures[i].get();
		
		auto mesh = new Mesh(data.vertices, data.uvCoords, data.normals, data.tangents);
		mesh->loadFromData(data.numFaces);
		mesh->setName(data.name);
		MeshLibrary::addMesh(data.name, mesh);
	}

	for(unsigned i = 0; i < _textureFutures.size(); ++i) {
		AssetLoader::TextureData data = _textureFutures[i].get();

		const auto texture = new Texture(data.name, data.type, data.width, data.height, data.data, data.channels, data.textureIndex);
		TextureLibrary::addTexture(data.name, texture);
	}
}

void ResourceManager::loadAssetFile(const std::string& filepath, const ResourceType type) {
	std::ifstream assetFile(filepath);
	CAPP_ASSERT(assetFile.good(), "Could not open file!\nFilepath: %s", filepath.c_str());

	switch(type) {
		
		case ResourceType::Mesh: {
			std::unordered_map<std::string, std::string> meshFilepaths;
			while(!assetFile.eof()) {
				std::string line;
				std::getline(assetFile, line);

				if(line.empty() || line.compare(0, 2, "//") == 0) {
					continue;
				}

				std::string name, path, token;
				std::istringstream sin(line);

				for(unsigned i = 0; i < 2; ++i) {
					std::getline(sin, token, ',');
					if(i == 0) {
						name = token;
					}
					else if(i == 1) {
						token.erase(0, 1);
						path = token;
					}
				}

				if(meshFilepaths.find(name) != meshFilepaths.end()) {
					CAPP_PRINT_WARNING(R"(Mesh "%s" with filepath "%s" already loaded, skipping...)", name.c_str(), path.c_str());
					continue;
				}

				meshFilepaths[name] = path;
			}

			for(const auto& properties : meshFilepaths) {
				_meshFutures.push_back(std::async(std::launch::async, AssetLoader::loadOBJ, properties.first, properties.second));
			}

			break;
		}

		case ResourceType::Texture: {
			std::vector<std::tuple<std::string, std::string, std::string, std::string>> textureFilepaths;

			while(!assetFile.eof()) {
				std::string line;
				std::getline(assetFile, line);

				if(line.empty() || line.compare(0, 2, "//") == 0) {
					continue;
				}

				std::string token, name, path, tp, index = "0";
				std::istringstream sin(line);

				for(unsigned i = 0; i < 3; ++i) {
					std::getline(sin, token, ',');
					if(i == 0) {
						name = token;
					}
					else if(i == 1) {
						token.erase(0, 1);
						path = token;
					}
					else if(i == 2) {
						token.erase(0, 1);
						tp = token;
					}
				}

				if(std::getline(sin, token, ',')) {
					token.erase(0, 1);
					index = token;
				}

				auto texture = std::make_tuple(name, path, tp, index);
				
				if(std::find(textureFilepaths.begin(), textureFilepaths.end(), texture) != textureFilepaths.end()) {
					CAPP_PRINT_WARNING(R"(Texture "%s" of type "%s" with filepath "%s" already loaded, skipping...)", name.c_str(), tp.c_str(), path.c_str());
					continue;
				}
				
				textureFilepaths.push_back(texture);
			}

			for(const auto& properties : textureFilepaths) {
				const auto& [name, path, tp, index] = properties;

				TextureType tt = TextureType::None;

				if(tp == "DiffuseMap")			tt = TextureType::DiffuseMap;
				else if(tp == "SpecularMap")	tt = TextureType::SpecularMap;
				else if(tp == "NormalMap")		tt = TextureType::NormalMap;
				else if(tp == "EmissionMap")	tt = TextureType::EmissionMap;
				else if(tp == "HeightMap")		tt = TextureType::HeightMap;
				else if(tp == "PBRAlbedo")		tt = TextureType::PBRAlbedo;
				else if(tp == "PBRMetallic")	tt = TextureType::PBRMetallic;
				else if(tp == "PBRNormal")		tt = TextureType::PBRNormal;
				else if(tp == "PBRRoughness")	tt = TextureType::PBRRoughness;
				else if(tp == "PBRAmbientOcc")	tt = TextureType::PBRAmbientOcc;
				else {
					CAPP_ASSERT(false, "Unknown texture type \"%s\"!", tp.c_str());
				}
				
				int i = std::stoi(index);
				_textureFutures.push_back(std::async(std::launch::async, AssetLoader::loadIMG, name, path, tt, i));
			}

			break;
		}
		
		default: break;
	}
}


// ----------------------------------------------------------------------------------------
// ----- Asset loader ---------------------------------------------------------------------
// ----------------------------------------------------------------------------------------

std::mutex AssetLoader::_textureMutex;

AssetLoader::MeshData AssetLoader::loadOBJ(std::string name, std::string filepath) {
	std::vector<glm::vec3> vertexData; vertexData.reserve(1000000);
	std::vector<glm::vec2> textureData; textureData.reserve(1000000);
	std::vector<glm::vec3> normalData; normalData.reserve(1000000);
	std::vector<glm::vec3> tangentData; tangentData.reserve(1000000);
	std::vector<FaceData> faces; faces.reserve(1000000);

	std::vector<float> unpackedVertexData; unpackedVertexData.reserve(3000000);
	std::vector<float> unpackedTextureData; unpackedTextureData.reserve(2000000);
	std::vector<float> unpackedNormalData; unpackedNormalData.reserve(3000000);
	std::vector<float> unpackedTangentData; unpackedTangentData.reserve(3000000);
	
	CAPP_PRINT_N(R"(Loading mesh "%s" (%s))", name.c_str(), filepath.c_str());

	//load the file
	std::ifstream input;
	input.open(filepath);

	if(!input.good()) {
		CAPP_ASSERT(input.good(), "Problem loading OBJ file: \"%s\"", filepath.c_str());
		return { "", {}, {}, {}, {}, {} };
	}

	
	// import data
	while(!input.eof()) {
		char inputString[128];
		input.getline(inputString, 128);

		//vertex data
		if(inputString[0] == 'v' && inputString[1] == ' ') {
			glm::vec3 vertData { 0,0,0 };

			std::sscanf(inputString, "v %f %f %f", &vertData.x, &vertData.y, &vertData.z);
			vertexData.push_back(vertData);
		}
			//texture data
		else if(inputString[0] == 'v' && inputString[1] == 't') {
			glm::vec2 texCoord { 0,0 };

			std::sscanf(inputString, "vt %f %f", &texCoord.x, &texCoord.y);
			textureData.push_back(texCoord);
		}
			//normal data
		else if(inputString[0] == 'v' && inputString[1] == 'n') {
			glm::vec3 normData { 0,0,0 };

			std::sscanf(inputString, "vn %f %f %f", &normData.x, &normData.y, &normData.z);
			normalData.push_back(normData);
		}
			//face data
		else if(inputString[0] == 'f' && inputString[1] == ' ') {
			faces.emplace_back();
			std::sscanf(inputString, "f %u/%u/%u %u/%u/%u %u/%u/%u",
				&faces.back().vertexData[0], &faces.back().textureData[0], &faces.back().normalData[0],
				&faces.back().vertexData[1], &faces.back().textureData[1], &faces.back().normalData[1],
				&faces.back().vertexData[2], &faces.back().textureData[2], &faces.back().normalData[2]);
		}
	}

	//add the data to the vectors
	for(auto& face : faces) {
		for(unsigned j = 0; j < 3; j++) {
			unpackedVertexData.push_back(vertexData[face.vertexData[j] - 1].x);
			unpackedVertexData.push_back(vertexData[face.vertexData[j] - 1].y);
			unpackedVertexData.push_back(vertexData[face.vertexData[j] - 1].z);

			if(!textureData.empty()) {
				unpackedTextureData.push_back(textureData[face.textureData[j] - 1].x);
				unpackedTextureData.push_back(textureData[face.textureData[j] - 1].y);
			}

			if(!normalData.empty()) {
				unpackedNormalData.push_back(normalData[face.normalData[j] - 1].x);
				unpackedNormalData.push_back(normalData[face.normalData[j] - 1].y);
				unpackedNormalData.push_back(normalData[face.normalData[j] - 1].z);
			}
		}
	}

	unsigned numFaces = faces.size();

	// https://learnopengl.com/Advanced-Lighting/Normal-Mapping
	// https://gitlab.com/ShawnM427/florp/blob/master/src/florp/graphics/MeshBuilder.cpp

	// it works!
	for(unsigned i = 0; i < numFaces; ++i) {
		std::vector<glm::vec3> tCalcPos;
		std::vector<glm::vec2> tCalcUV;

		for(unsigned j = 0; j < 3; ++j) {
			tCalcPos.push_back(vertexData[faces[i].vertexData[j] - 1]);
			tCalcUV.push_back(textureData[faces[i].textureData[j] - 1]);

		}

		glm::vec3 deltaPos = tCalcPos[1] - tCalcPos[0];
		glm::vec3 deltaPos2 = tCalcPos[2] - tCalcPos[0];

		glm::vec2 deltaUV = tCalcUV[1] - tCalcUV[0];
		glm::vec2 deltaUV2 = tCalcUV[2] - tCalcUV[0];

		float f = 1.0f / (deltaUV.x * deltaUV2.y - deltaUV.y * deltaUV2.x);
		glm::vec3 tang = (deltaPos * deltaUV2.y - deltaPos2 * deltaUV.y) * f;

		for(unsigned j = 0; j < 3; ++j) {
			tangentData.push_back(tang);
		}

	}

	for(auto tang : tangentData) {
		unpackedTangentData.push_back(tang.x);
		unpackedTangentData.push_back(tang.y);
		unpackedTangentData.push_back(tang.z);
	}

	input.close();
	return { std::move(name), numFaces, unpackedVertexData, unpackedTextureData, unpackedNormalData, unpackedTangentData };
}

AssetLoader::TextureData AssetLoader::loadIMG(std::string name, std::string filepath, const TextureType textureType, const unsigned textureIndex) {

	CAPP_PRINT_N("Loading texture \"%s\" (%s)", name.c_str(), filepath.c_str());
	
	stbi_set_flip_vertically_on_load(true);
	int width, height, channels;

	std::lock_guard<std::mutex> lock(_textureMutex);
	stbi_uc* data = stbi_load(filepath.c_str(), &width, &height, &channels, 0);
	
	CAPP_ASSERT(data != nullptr, "Failed to load texture \"%s\" (%s)", name.c_str(), filepath.c_str());
	CAPP_ASSERT(width > 0 && height > 0, "Image \"%s\" must have dimensions greater than 0!\nTexture path: %s", name.c_str(), filepath.c_str());
	
	return { std::move(name), textureType, static_cast<unsigned>(width), static_cast<unsigned>(height), static_cast<unsigned>(channels), data, textureIndex };
}