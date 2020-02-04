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
		Purp: performs matrix multiplication and assigns the final product to the transformMat
		*/ 
		void update();

		glm::vec3 translate(const glm::vec3& translateBy);

		//using a quaternion
		const glm::mat4& rotate(const glm::vec3& eulerRotation);

		//using a transformation matrix
		const glm::mat4& rotate(const glm::vec3& rotateOn, float rotationAngle);
		const glm::mat4& scale(const glm::vec3& scaleVec, float sizeScalar);

		glm::mat4 _transformMat{ 1.0f };

		glm::mat4 _parentTransform { 1.0f };
		glm::mat4 _worldTransformMat { 1.0f };
		
		glm::vec4* _position = nullptr;

		glm::mat4 _translateMat{ 1.0f };
		glm::mat4 _scaleMat{ 1.0f };
		glm::mat4 _rotateMat{ 1.0f };
	
		glm::vec3 forward{ 0.0f,0.0f,-1.0f };
	};
}