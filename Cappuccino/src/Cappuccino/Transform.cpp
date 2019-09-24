#include "Cappuccino/Transform.h"

namespace Cappuccino {
	glm::mat4 Transform::doTransform(const std::optional<glm::vec3>& translation, const std::optional<glm::vec3>& rotateOn, const std::optional<float> rotationAngle, const std::optional<glm::vec3>& scaleVec, const std::optional<float>& sizeScalar)
	{
		if (translation.has_value())
			translate(translation.value());
		if (rotateOn.has_value())
			rotate(rotateOn.value(), rotationAngle.value());
		if (scaleVec.has_value())
			scale(scaleVec.value(), (sizeScalar.has_value() ? sizeScalar.value() : 1));

		return transformMat;
	}
	glm::vec3 Transform::translate(const glm::vec3& translateBy)
	{
		transformMat = glm::translate(transformMat, translateBy);
		//position *= transformMat;
		return translateBy;
	}

	glm::mat4 Transform::rotate(const glm::vec3& rotateOn, float rotationAngle)
	{
		return transformMat = glm::rotate(transformMat, rotationAngle, rotateOn);
		//	position *= transformMat;
	}

	glm::mat4 Transform::scale(const glm::vec3& scaleVec, float sizeScalar)
	{
		return transformMat = glm::scale(transformMat, scaleVec * sizeScalar);
	}
}