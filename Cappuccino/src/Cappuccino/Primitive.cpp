#include "Cappuccino/Primitive.h"
#include "Cappuccino/CappMacros.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
namespace Cappuccino {
	namespace Primitives {

		Cube::Cube()
			:Mesh(CAPP_PATH + "Assets/Mesh/Cube.obj")
		{
			//_body.hitBox.push_back(HitBox(glm::vec3(0,0,0),glm::vec3(1,1,1)));
		}
		void Cube::draw() {
			glBindVertexArray(_VAO);
			glDrawArrays(GL_LINES, 0, _numVerts);
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

		Primitive::Primitive(const std::string& path)
			:Mesh(path)
		{
		}

		void Primitive::draw()
		{
			glBindVertexArray(_VAO);
			glDrawArrays(GL_LINES, 0, _numVerts);
		}

	}
}