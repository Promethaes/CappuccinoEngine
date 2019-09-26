#include "Cappuccino/Transform.h"

namespace Cappuccino {
	glm::mat4 Transform::doTransform(const std::optional<glm::vec3>& translation, const std::optional<glm::vec3>& rotateOn, const std::optional<float> rotationAngle, const std::optional<glm::vec3>& scaleVec, const std::optional<float>& sizeScalar)
	{
		if (translation.has_value())
			transformMat = glm::translate(transformMat, translation.value());
		if (rotateOn.has_value())
			transformMat = glm::rotate(transformMat,rotationAngle.value(),rotateOn.value());
		if (scaleVec.has_value())
			transformMat = glm::scale(transformMat, scaleVec.value() * sizeScalar.value());

		return transformMat;
	}
	void Transform::update()
	{
		transformMat = scaleMat * rotateMat * translateMat;
	}
	glm::vec3 Transform::translate(const glm::vec3& translateBy)
	{
		translateMat = glm::translate(translateMat, translateBy);
		return glm::vec3(translateMat[3].x, translateMat[3].y, translateMat[3].z);
		//position *= transformMat;
	}

	glm::mat4 Transform::rotate(const glm::vec3& rotateOn, float rotationAngle)
	{
		return rotateMat = glm::rotate(rotateMat, rotationAngle, rotateOn);
	}

	glm::mat4 Transform::scale(const glm::vec3& scaleVec, float sizeScalar)
	{
		return scaleMat = glm::scale(glm::mat4(sizeScalar), scaleVec);
	}
}