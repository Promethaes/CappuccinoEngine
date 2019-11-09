#include <glad/glad.h>
#include <glm/glm.hpp>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <fstream>
#include "Cappuccino/Mesh.h"
#include "Cappuccino/CappMacros.h"
#include "Cappuccino/ResourceManager.h"

using string = std::string;

namespace Cappuccino {
	struct FaceData {
		unsigned vertexData[3]{};
		unsigned textureData[3]{};
		unsigned normalData[3]{};
	};


	std::string Mesh::_meshDirectory = CAPP_PATH + R"(Assets/Meshes/)";
	Mesh::Mesh(const std::string& path)
	{
		this->_path = path;
		ResourceManager::_allMeshes.push_back(this);
	}

	bool Mesh::loadMesh()
	{
		if (loaded)
			return true;
		char inputString[128];

		std::vector<glm::vec3> vertexData{};
		std::vector<glm::vec2> textureData{};
		std::vector<glm::vec3> normalData{};
		std::vector<FaceData> faces{};
		std::vector<float> unPvertexData{};
		std::vector<float> unPtextureData{};
		std::vector<float> unPnormalData{};
		//load the file
		std::ifstream input{};
		input.open(_meshDirectory + _path);

		if (!input.good()) {
			std::cout << "Problem loading file: " << _path << "\n";
			return false;
		}
		//import data
		while (!input.eof()) {
			input.getline(inputString, 128);

			//vertex data
			if (inputString[0] == 'v' && inputString[1] == ' ') {
				glm::vec3 vertData{ 0,0,0 };

				std::sscanf(inputString, "v %f %f %f", &vertData.x, &vertData.y, &vertData.z);
				vertexData.push_back(vertData);
			}//texture data
			else if (inputString[0] == 'v' && inputString[1] == 't') {
				glm::vec2 texCoord{ 0,0 };

				std::sscanf(inputString, "vt %f %f", &texCoord.x, &texCoord.y);
				textureData.push_back(texCoord);
			}//normal data
			else if (inputString[0] == 'v' && inputString[1] == 'n') {
				glm::vec3 normData{ 0,0,0 };

				std::sscanf(inputString, "vn %f %f %f", &normData.x, &normData.y, &normData.z);
				normalData.push_back(normData);
			}//face data
			else if (inputString[0] == 'f' && inputString[1] == ' ') {
				faces.emplace_back();


				std::sscanf(inputString, "f %u/%u/%u %u/%u/%u %u/%u/%u",
				            &faces.back().vertexData[0], &faces.back().textureData[0], &faces.back().normalData[0],
				            &faces.back().vertexData[1], &faces.back().textureData[1], &faces.back().normalData[1],
				            &faces.back().vertexData[2], &faces.back().textureData[2], &faces.back().normalData[2]);
			}
			else
				continue;
		}
		//add the data to the vectors
		for (unsigned i = 0; i < faces.size(); i++) {
			for (unsigned j = 0; j < 3; j++) {
				unPvertexData.push_back(vertexData[faces[i].vertexData[j] - 1].x);
				unPvertexData.push_back(vertexData[faces[i].vertexData[j] - 1].y);
				unPvertexData.push_back(vertexData[faces[i].vertexData[j] - 1].z);

				if (!textureData.empty()) {
					unPtextureData.push_back(textureData[faces[i].textureData[j] - 1].x);
					unPtextureData.push_back(textureData[faces[i].textureData[j] - 1].y);
				}

				if (!normalData.empty()) {
					unPnormalData.push_back(normalData[faces[i].normalData[j] - 1].x);
					unPnormalData.push_back(normalData[faces[i].normalData[j] - 1].y);
					unPnormalData.push_back(normalData[faces[i].normalData[j] - 1].z);
				}
			}
		}

		_numFaces = faces.size();
		_numVerts = _numFaces * 3;

		for (unsigned i = 0; i < unPvertexData.size(); i++) {
			master.push_back(unPvertexData[i]);
			verts.push_back(unPvertexData[i]);
		}
		for (unsigned i = 0; i < unPtextureData.size(); i++) {
			master.push_back(unPtextureData[i]);
			texts.push_back(unPtextureData[i]);
		}
		for (unsigned i = 0; i < unPnormalData.size(); i++) {
			master.push_back(unPnormalData[i]);
			norms.push_back(unPnormalData[i]);
		}

		glGenVertexArrays(1, &_VAO);
		glGenBuffers(1, &_VBO);

		//binding the vao
		glBindVertexArray(_VAO);

		//enable slots
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		glBindBuffer(GL_ARRAY_BUFFER, _VBO);
		//vertex
		glBufferData(GL_ARRAY_BUFFER, master.size() * sizeof(float), &master[0], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(unPvertexData.size() * sizeof(float)));
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)((unPtextureData.size() + unPvertexData.size()) * sizeof(float)));


		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		input.close();
		return loaded = true;
	}

	void Mesh::reload(const std::vector<float>& VERTS, const std::vector<float>& TEXTS, const std::vector<float>& NORMS)
	{
		master.clear();
		verts.clear();
		//	texts.clear();
		norms.clear();

		unload();

		for (unsigned i = 0; i < VERTS.size(); i++) {
			master.push_back(VERTS[i]);
			verts.push_back(VERTS[i]);
		}
		for (unsigned i = 0; i < TEXTS.size(); i++) {
			master.push_back(TEXTS[i]);
		}
		for (unsigned i = 0; i < NORMS.size(); i++) {
			master.push_back(NORMS[i]);
			norms.push_back(NORMS[i]);
		}

		glGenVertexArrays(1, &_VAO);
		glGenBuffers(1, &_VBO);

		glBindVertexArray(_VAO);

		//enable slots
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		glBindBuffer(GL_ARRAY_BUFFER, _VBO);
		//vertex
		glBufferData(GL_ARRAY_BUFFER, master.size() * sizeof(float), &master[0], GL_DYNAMIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(verts.size() * sizeof(float)));
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)((texts.size() + verts.size()) * sizeof(float)));


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
		string dir = directory;
		std::transform(dir.begin(), dir.end(), dir.begin(), ::tolower);

		if (dir == "default")
			_meshDirectory = CAPP_PATH + R"(\Assets\Meshes\)";
		else
			_meshDirectory = directory;
	}

}