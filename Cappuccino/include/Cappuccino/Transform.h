#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <optional>

namespace Cappuccino {
	class Transform {
	public:
		Transform() {
			_position = &_translateMat[3];
		}

		/*
		Purp: do whatever transformations you want...not a good idea to use this
		Req: all params are optional, you can pass in anything you want (just read the variable names and you'll be good)
		returns: the transformation matrix
		*/
		glm::mat4 doTransform(const std::optional<glm::vec3>& translation, const std::optional<glm::vec3>& rotateOn, const std::optional<float> rotationAngle,
			const std::optional<glm::vec3>& scaleVec, const std::optional<float>& sizeScalar);

		/*
		Purp: performs matrix multiplication and assigns the final product to the transformMat
		*/ 
		void update();

		glm::vec3 translate(const glm::vec3& translateBy);

		//using a quaternion
		glm::mat4 rotate(/*TODO*/);

		//using a transformation matrix
		glm::mat4 rotate(const glm::vec3& rotateOn, float rotationAngle);
		glm::mat4 scale(const glm::vec3& scaleVec, float sizeScalar);

		glm::mat4 _transformMat{ 1.0f };

		glm::vec4* _position;

		glm::mat4 _translateMat{ 1.0f };
		glm::mat4 _scaleMat{ 1.0f };
		glm::mat4 _rotateMat{ 1.0f };
	
		glm::vec3 forward{ 0.0f,0.0f,-1.0f };
	private:
	};
}