#include "Cappuccino/Primitive.h"
#include "Cappuccino/CappMacros.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
namespace Cappuccino {
	namespace Primitives {
		Cube::Cube()
			:Mesh(CAPP_PATH + "Assets/Mesh/Cube.obj")
		{
			_body.hitBox.push_back(HitBox(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)));
		}
		void Cube::draw() {
			glBindVertexArray(_VAO);
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glDrawArrays(GL_TRIANGLES, 0, _numVerts);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
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