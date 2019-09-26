#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <optional>

namespace Cappuccino {
	class Transform {
	public:
		Transform() = default;

		//using rotation matrices to rotate
		glm::mat4 doTransform(const std::optional<glm::vec3>& translation, const std::optional<glm::vec3>& rotateOn, const std::optional<float> rotationAngle,
			const std::optional<glm::vec3>& scaleVec, const std::optional<float>& sizeScalar);

		void update();

		glm::vec3 translate(const glm::vec3& translateBy);
		//using a quaternion
		glm::mat4 rotate(/*TODO*/);
		//using a transformation matrix
		glm::mat4 rotate(const glm::vec3& rotateOn, float rotationAngle);
		glm::mat4 scale(const glm::vec3& scaleVec, float sizeScalar);

		glm::mat4 transformMat{ 1.0f };

		glm::vec3 position{ 1,1,1 };

	private:
		glm::mat4 translateMat{ 1.0f };
		glm::mat4 scaleMat{ 1.0f };
		glm::mat4 rotateMat{ 1.0f };
	};
}