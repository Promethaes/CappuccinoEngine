#include "Cappuccino/Cube.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
namespace Cappuccino {

	//unsigned Cube::VAO = 0;
	//bool Cube::VAOExist = false;
	Cappuccino::Cube::Cube(float* verts, const unsigned sizeOfArray, Texture* TEXTURERECT, bool irregularVertData)
		:textureRect(*TEXTURERECT)
	{
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		this->irregularVertData = irregularVertData;
		bindBuffers(verts, sizeOfArray);
		bindVertPointers();
	}
	void Cube::bindBuffers(float* verts, const unsigned sizeOfArray)
	{
		//bind the vertex array and the buffer to the context
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * sizeOfArray, verts, GL_STATIC_DRAW);
	}
	void Cube::bindVertPointers()
	{
		//point the attrivute pointer in the right direction based on the size of our vector of floats
		//this will determine how many spaces in memory opengl should leave inbetween checkinf for data

		glBindVertexArray(VAO);
		if (!irregularVertData) {

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);
		}
		else {
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
			glEnableVertexAttribArray(2);
		}
	}
	void Cube::draw()
	{
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
}
