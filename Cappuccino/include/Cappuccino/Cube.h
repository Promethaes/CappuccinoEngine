#pragma once
#include "Cappuccino/Texture.h"
#include <glm/glm.hpp>
#include <vector>
namespace Cappuccino {
	/*
	Desc.: a 3D cube here for visual representation with functions pertaining to that purpose
	*/

	class Cube {
	public:
		Cube(float* verts, const unsigned sizeOfArray, Texture* TEXTURERECT, bool irregularVertData = false);

		/*
		Purp.: Draws the cube in the 3D plane
		Req.: Vertices of the cube, the size of the array, the texture used on the cube, set true if the vertex data is irregular
		Returns: void
		*/
		void draw();
		/*
		Purp.: Get the texture of the cube
		Req.: none
		Returns: Returns the texture of the cube
		*/
		Texture getTexture() const { return textureRect; }

		//The cube's position in the 3D plane
		glm::vec3 position;

	private:
		/*
		Purp.: Binds the buffers
		Req.: Vertices, and the size of the array called
		Returns: void
		*/
		void bindBuffers(float* verts, const unsigned sizeOfArray);
		/*
		Purp.: Binds vertex pointers
		Req.: none
		Returns: void
		*/
		void bindVertPointers();

		//Rectangle texture
		Texture textureRect;

		//Vertex Array Object
		unsigned VAO;
		//Vertex Buffer Object and Element Buffer Object
		unsigned VBO, EBO;
		//Irregular Vertex Data, either a true or false
		bool irregularVertData;
	};
}