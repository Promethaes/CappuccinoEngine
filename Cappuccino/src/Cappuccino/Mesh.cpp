#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <sstream>
#include <iostream>
#include <fstream>
#include "Cappuccino/Mesh.h"
#include "Cappuccino/CappMacros.h"

namespace Cappuccino {
	struct FaceData {
		unsigned vertexData[3];
		unsigned textureData[3];
		unsigned normalData[3];
	};


	bool Mesh::loadMesh()
	{
		if (loaded)
			return true;
		char inputString[128];

		std::vector<glm::vec3> vertexData;
		std::vector<glm::vec2> textureData;
		std::vector<glm::vec3> normalData;
		std::vector<FaceData> faces;
		std::vector<float> unPvertexData;
		std::vector<float> unPtextureData;
		std::vector<float> unPnormalData;
		//load the file
		std::ifstream input;
		input.open(_path);

		if (!input.good()) {
			std::cout << "Problem loading file: " << _path << "\n";
			return false;
		}
		//import data
		while (!input.eof()) {
			input.getline(inputString, 128);

			//vertex data
			if (inputString[0] == 'v' && inputString[1] == ' ') {
				glm::vec3 vertData;

				std::sscanf(inputString, "v %f %f %f", &vertData.x, &vertData.y, &vertData.z);
				vertexData.push_back(vertData);
			}//texture data
			else if (inputString[0] == 'v' && inputString[1] == 't') {
				glm::vec2 texCoord;

				std::sscanf(inputString, "vt %f %f", &texCoord.x, &texCoord.y);
				textureData.push_back(texCoord);
			}//normal data
			else if (inputString[0] == 'v' && inputString[1] == 'n') {
				glm::vec3 normData;

				std::sscanf(inputString, "vn %f %f %f", &normData.x, &normData.y, &normData.z);
				normalData.push_back(normData);
			}//face data
			else if (inputString[0] == 'f' && inputString[1] == ' ') {
				FaceData faceData;

				std::sscanf(inputString, "f %u/%u/%u %u/%u/%u %u/%u/%u",
					&faceData.vertexData[0], &faceData.textureData[0], &faceData.normalData[0],
					&faceData.vertexData[1], &faceData.textureData[1], &faceData.normalData[1],
					&faceData.vertexData[2], &faceData.textureData[2], &faceData.normalData[2]);
				faces.push_back(faceData);
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

				unPtextureData.push_back(textureData[faces[i].textureData[j] - 1].x);
				unPtextureData.push_back(textureData[faces[i].textureData[j] - 1].y);
				

				unPnormalData.push_back(normalData[faces[i].normalData[j] - 1].x);
				unPnormalData.push_back(normalData[faces[i].normalData[j] - 1].y);
				unPnormalData.push_back(normalData[faces[i].normalData[j] - 1].z);
			}
		}

		_numFaces = faces.size();
		_numVerts = _numFaces * 3;
		std::vector<float> master;

		for (auto x : unPvertexData)
			master.push_back(x);
		for (auto x : unPtextureData)
			master.push_back(x);
		for (auto x : unPnormalData)
			master.push_back(x);

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

	void Mesh::unload()
	{
		//empty the buffers
		glDeleteBuffers(1, &_VBO);

		glDeleteVertexArrays(1, &_VAO);
		_VBO = 0;
		_VAO = 0;

		_numFaces = 0;//reset all numbers
		_numVerts = 0;
	}

	void Mesh::draw()
	{
		glBindVertexArray(_VAO);
		glDrawArrays(GL_TRIANGLES, 0, _numVerts);
	}
}