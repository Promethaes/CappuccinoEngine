#include "Cappuccino/Primitive.h"
#include "Cappuccino/CappMacros.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
namespace Cappuccino {
	namespace Primitives {

		Cube::Cube()
			:Mesh(CAPP_PATH + "Assets/Mesh/Cube.obj")
		{
		}
		void Cube::draw() {
			glBindVertexArray(_VAO);
			glDrawArrays(GL_TRIANGLES, 0, _numVerts);
		}

		Sphere::Sphere()
			: Mesh(CAPP_PATH + "Assets/Mesh/Sphere.obj")
		{
		}

		Cylinder::Cylinder()
			: Mesh(CAPP_PATH + "Assets/Mesh/Cylinder.obj")
		{
		}
		void Cylinder::draw() {
			glBindVertexArray(_VAO);
			glDrawArrays(GL_LINES, 0, _numVerts);
			
		}

	}
}