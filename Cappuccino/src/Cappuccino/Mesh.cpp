#include "Cappuccino/Mesh.h"

#include <glad/glad.h>

#include <algorithm>
#include <iostream>
#include "Cappuccino/ResourceManager.h"

namespace Cappuccino {
	struct FaceData {
		unsigned vertexData[3]{};
		unsigned textureData[3]{};
		unsigned normalData[3]{};
	};


	std::string Mesh::_meshDirectory = "./Assets/Meshes/";
	Mesh::Mesh(const std::string& name, const std::string& path) :
		_properties({ name, path }) {
		glGenVertexArrays(1, &_VAO);
		glGenBuffers(1, &_VBO);
	}

	Mesh::~Mesh() {
		glDeleteVertexArrays(1, &_VAO);
	}

	Mesh::Mesh(const std::vector<float>& VERTS, const std::vector<float>& TEXTS, const std::vector<float>& NORMS, const std::vector<float>& TANGS)
	{
		verts = VERTS;
		texts = TEXTS;
		norms = NORMS;
		tangs = TANGS;

		glGenVertexArrays(1, &_VAO);
		glGenBuffers(1, &_VBO);
	}

	Mesh::Mesh(Mesh& other)
	{
		verts = other.verts;
		texts = other.texts;
		norms = other.norms;
		tangs = other.tangs;
		_numVerts = other._numVerts;
		_numFaces = other._numFaces;
	}

	bool Mesh::loadMesh()
	{
		if (loaded)
			return true;
		//char inputString[128];

		//std::vector<glm::vec3> vertexData{};
		//std::vector<glm::vec2> textureData{};
		//std::vector<glm::vec3> normalData{};
		//std::vector<FaceData> faces{};
		//std::vector<float> unPvertexData{};
		//std::vector<float> unPtextureData{};
		//std::vector<float> unPnormalData{};
		//load the file
		//std::ifstream input(_meshDirectory + _properties.filepath);

		//if (!input.good()) {
		//	CAPP_ASSERT(false, "Problem loading mesh \"%s\"\nMesh path: %s", _properties.name.c_str(), _properties.filepath.c_str());
		//	return false;
		//}
		//import data
		//while (!input.eof()) {
		//	input.getline(inputString, 128);

		//	vertex data
		//	if (inputString[0] == 'v' && inputString[1] == ' ') {
		//		glm::vec3 vertData { 0, 0, 0 };

		//		std::sscanf(inputString, "v %f %f %f", &vertData.x, &vertData.y, &vertData.z);
		//		vertexData.push_back(vertData);
		//	}//texture data
		//	else if (inputString[0] == 'v' && inputString[1] == 't') {
		//		glm::vec2 texCoord { 0, 0 };

		//		std::sscanf(inputString, "vt %f %f", &texCoord.x, &texCoord.y);
		//		textureData.push_back(texCoord);
		//	}//normal data
		//	else if (inputString[0] == 'v' && inputString[1] == 'n') {
		//		glm::vec3 normData { 0, 0, 0 };

		//		std::sscanf(inputString, "vn %f %f %f", &normData.x, &normData.y, &normData.z);
		//		normalData.push_back(normData);
		//	}//face data
		//	else if (inputString[0] == 'f' && inputString[1] == ' ') {
		//		faces.emplace_back();


		//		std::sscanf(inputString, "f %u/%u/%u %u/%u/%u %u/%u/%u",
		//			&faces.back().vertexData[0], &faces.back().textureData[0], &faces.back().normalData[0],
		//			&faces.back().vertexData[1], &faces.back().textureData[1], &faces.back().normalData[1],
		//			&faces.back().vertexData[2], &faces.back().textureData[2], &faces.back().normalData[2]);
		//	}
		//	else
		//		continue;
		//}



		//add the data to the vectors
		//for (unsigned i = 0; i < faces.size(); i++) {
		//	for (unsigned j = 0; j < 3; j++) {
		//		unPvertexData.push_back(vertexData[faces[i].vertexData[j] - 1].x);
		//		unPvertexData.push_back(vertexData[faces[i].vertexData[j] - 1].y);
		//		unPvertexData.push_back(vertexData[faces[i].vertexData[j] - 1].z);

		//		if (!textureData.empty()) {
		//			unPtextureData.push_back(textureData[faces[i].textureData[j] - 1].x);
		//			unPtextureData.push_back(textureData[faces[i].textureData[j] - 1].y);
		//		}

		//		if (!normalData.empty()) {
		//			unPnormalData.push_back(normalData[faces[i].normalData[j] - 1].x);
		//			unPnormalData.push_back(normalData[faces[i].normalData[j] - 1].y);
		//			unPnormalData.push_back(normalData[faces[i].normalData[j] - 1].z);
		//		}
		//	}
		//}

		//_numFaces = faces.size();
		//_numVerts = _numFaces * 3;

		//https://learnopengl.com/Advanced-Lighting/Normal-Mapping
		//https://gitlab.com/ShawnM427/florp/blob/master/src/florp/graphics/MeshBuilder.cpp

		//it works!
		//std::vector<glm::vec3> tangs;

		//for (unsigned i = 0; i < _numFaces; i++) {
		//	std::vector<glm::vec3> tCalcPos;
		//	std::vector<glm::vec2> tCalcUV;

		//	for (unsigned j = 0; j < 3; j++) {
		//		tCalcPos.push_back(vertexData[faces[i].vertexData[j] - 1]);
		//		tCalcUV.push_back(textureData[faces[i].textureData[j] - 1]);

		//	}
		//	glm::vec3 deltaPos = tCalcPos[1] - tCalcPos[0];
		//	glm::vec3 deltaPos2 = tCalcPos[2] - tCalcPos[0];

		//	glm::vec2 deltaUV = tCalcUV[1] - tCalcUV[0];
		//	glm::vec2 deltaUV2 = tCalcUV[2] - tCalcUV[0];

		//	float f = 1.0f / (deltaUV.x * deltaUV2.y - deltaUV.y * deltaUV2.x);
		//	glm::vec3 tang = (deltaPos * deltaUV2.y - deltaPos2 * deltaUV.y) * f;

		//	for (unsigned j = 0; j < 3; j++) {
		//		tangs.push_back(tang);
		//	}

		//}

		AssetLoader::MeshData data = AssetLoader::loadOBJ(_properties.name, _meshDirectory + _properties.filepath);
		_numVerts = data.vertices.size();
		_numFaces = _numVerts / 3;

		for(float vert : data.vertices)
			master.push_back(vert);
		for(float uv : data.uvCoords)
			master.push_back(uv);
		for(float norm : data.normals)
			master.push_back(norm);
		for(float tang : data.tangents)
			master.push_back(tang);

		//binding the vao
		glBindVertexArray(_VAO);

		//enable slots
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);
		glEnableVertexAttribArray(4);
		glEnableVertexAttribArray(5);
		glEnableVertexAttribArray(6);

		glBindBuffer(GL_ARRAY_BUFFER, _VBO);
		//vertex
		glBufferData(GL_ARRAY_BUFFER, master.size() * sizeof(float), &master[0], GL_STATIC_DRAW);
		//verts
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		//texts
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(data.vertices.size() * sizeof(float)));
		//norms
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)((data.vertices.size() + data.uvCoords.size()) * sizeof(float)));
		//tangents
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)((data.vertices.size() + data.uvCoords.size() + data.normals.size()) * sizeof(float)));

		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)((data.vertices.size() + data.uvCoords.size()) * sizeof(float)));
		glVertexAttribPointer(6, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)((data.vertices.size() + data.uvCoords.size() + data.normals.size()) * sizeof(float)));


		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		return loaded = true;
	}

	void Mesh::loadFromData(const unsigned numFaces) {
		_numFaces = numFaces;
		_numVerts = _numFaces * 3;
		loadFromData();
	}

	void Mesh::loadFromData()
	{
		master.clear();
		
		for(float vert : verts)
			master.push_back(vert);
		for(float uv : texts)
			master.push_back(uv);
		for(float norm : norms)
			master.push_back(norm);
		for(float tang : tangs)
			master.push_back(tang);

		glGenVertexArrays(1, &_VAO);
		glGenBuffers(1, &_VBO);
		glBindVertexArray(_VAO);

		//enable slots
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);
		glEnableVertexAttribArray(4);
		glEnableVertexAttribArray(5);
		glEnableVertexAttribArray(6);

		glBindBuffer(GL_ARRAY_BUFFER, _VBO);
		//vertex
		glBufferData(GL_ARRAY_BUFFER, master.size() * sizeof(float), &master[0], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(verts.size() * sizeof(float)));
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)((texts.size() + verts.size()) * sizeof(float)));
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)((texts.size() + verts.size() + norms.size()) * sizeof(float)));

		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)((texts.size() + verts.size()) * sizeof(float)));
		glVertexAttribPointer(6, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)((texts.size() + verts.size() + norms.size()) * sizeof(float)));


		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		loaded = true;
	}
	void Mesh::animationFunction(Mesh& other)
	{

		glBindVertexArray(_VAO);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);
		glEnableVertexAttribArray(4);
		glEnableVertexAttribArray(5);
		glEnableVertexAttribArray(6);

		glBindBuffer(GL_ARRAY_BUFFER, _VBO);
		//vertex
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(verts.size() * sizeof(float)));
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)((texts.size() + verts.size()) * sizeof(float)));
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)((texts.size() + verts.size() + norms.size()) * sizeof(float)));

		glBindBuffer(GL_ARRAY_BUFFER, other._VBO);

		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)((texts.size() + verts.size()) * sizeof(float)));
		glVertexAttribPointer(6, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)((texts.size() + verts.size() + norms.size()) * sizeof(float)));


		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	void Mesh::reload(const std::vector<float>& VERTS, const std::vector<float>& NORMS, const std::vector<float>& TANGS)
	{
		master.clear();
		verts.clear();
		norms.clear();
		tangs.clear();

		for(float i : VERTS) {
			master.push_back(i);
			verts.push_back(i);
		}
		for(float i : texts) {
			master.push_back(i);
		}
		for(float i : NORMS) {
			master.push_back(i);
			norms.push_back(i);
		}
		for(float i : TANGS) {
			master.push_back(i);
			tangs.push_back(i);
		}


		glBindVertexArray(_VAO);

		//enable slots
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);

		glBindBuffer(GL_ARRAY_BUFFER, _VBO);
		//vertex
		glBufferSubData(GL_ARRAY_BUFFER, 0, master.size() * sizeof(float), &master[0]);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(verts.size() * sizeof(float)));
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)((texts.size() + verts.size()) * sizeof(float)));
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)((texts.size() + verts.size() + norms.size()) * sizeof(float)));


		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}


	void Mesh::unload()
	{
		//empty the buffers
		glDeleteBuffers(1, &_VBO);

		glDeleteVertexArrays(1, &_VAO);
		_VBO = 0;
		_VAO = 0;

		//_numFaces = 0;//reset all numbers
		//_numVerts = 0;
	}

	void Mesh::draw()
	{
		glBindVertexArray(_VAO);
		glDrawArrays(GL_TRIANGLES, 0, _numVerts);
	}

	void Mesh::setDefaultPath(const std::string& directory) {
		std::string dir = directory;
		std::transform(dir.begin(), dir.end(), dir.begin(), ::tolower);

		if (dir == "default")
			_meshDirectory = "./Assets/Meshes/";
		else
			_meshDirectory = directory;
	}

}
