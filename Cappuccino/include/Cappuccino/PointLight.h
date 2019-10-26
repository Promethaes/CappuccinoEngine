#pragma once
#include "ShaderProgram.h"

namespace Cappuccino {
	class PointLight {
	public:
		PointLight(const glm::vec2& windowSize, const glm::vec3& position, const glm::vec3& ambientColour,
			const glm::vec3& diffuseColour, const glm::vec3& specularColour, float shininess);


		void updateViewPos(const glm::vec3& cameraPos);
		void setPosition(const glm::vec3& pos);
		void setAmbient(const glm::vec3& colour);
		void setDiffuse(const glm::vec3& colour);
		void setSpecular(const glm::vec3& colour);
		void setShininess(float scalar);

		glm::vec3& getPosition() { return _position; }
		glm::vec3& getAmbient() { return _ambientColour; }
		glm::vec3& getDiffuse() { return _diffuseColour; }
		glm::vec3& getSpecular() { return _specularColour; }
		float& getShininess() { return _shininess; }

		Shader _pointLightShader;
	private:
		glm::vec3 _position;
		glm::vec3 _ambientColour;
		glm::vec3 _diffuseColour;
		glm::vec3 _specularColour;
		float _shininess;
	};
}