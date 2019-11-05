#include "Cappuccino/Transform.h"

using namespace Cappuccino;

glm::mat4 Transform::doTransform(const std::optional<glm::vec3>& translation, const std::optional<glm::vec3>& rotateOn, const std::optional<float> rotationAngle, const std::optional<glm::vec3>& scaleVec, const std::optional<float>& sizeScalar)
{
	if (translation.has_value())
		_transformMat = glm::translate(_transformMat, translation.value());
	if (rotateOn.has_value())
		_transformMat = glm::rotate(_transformMat, rotationAngle.value(), rotateOn.value());
	if (scaleVec.has_value())
		_transformMat = glm::scale(_transformMat, scaleVec.value() * sizeScalar.value());
	return _transformMat;
}


void Transform::update()
{
	_transformMat = _translateMat * _rotateMat * _scaleMat;
}

glm::vec3 Transform::translate(const glm::vec3& translateBy)
{
	_translateMat = glm::translate(_translateMat, translateBy);
	return _translateMat[3];
}

glm::mat4 Transform::rotate(const glm::vec3& rotateOn, float rotationAngle)
{
	return _rotateMat = glm::rotate(_rotateMat, glm::radians(rotationAngle), glm::radians(rotateOn)*rotationAngle);
}

glm::mat4 Transform::scale(const glm::vec3& scaleVec, float sizeScalar)
{
	return _scaleMat = glm::scale(_scaleMat, scaleVec*sizeScalar);
}