#pragma once
#include "ShaderProgram.h"

namespace Cappuccino {
	class DirLight {
	public:
		DirLight(const glm::vec2& windowSize, const glm::vec3& direction, const glm::vec3& ambientColour, const glm::vec3& diffuseColour,
			const glm::vec3& specularColour, float shininess);

		/*
		Purp: update the view position, should be called every frame
		Req: the camera position
		*/
		void updateViewPos(const glm::vec3& cameraPos);

		void setDirection(const glm::vec3& dir);
		void setAmbient(const glm::vec3& colour);
		void setDiffuse(const glm::vec3& colour);
		void setSpecular(const glm::vec3& colour);
		void setShininess(float scalar);

		glm::vec3& getDirection() { return _direction; }
		glm::vec3& getAmbient() { return _ambientColour; }
		glm::vec3& getDiffuse() { return _diffuseColour; }
		glm::vec3& getSpecular() { return _specularColour; }
		float& getShininess() { return _shininess; }

	private:
		glm::vec3 viewPosition;
		glm::vec3 _direction;
		glm::vec3 _ambientColour;
		glm::vec3 _diffuseColour;
		glm::vec3 _specularColour;
		float _shininess;
		static Shader _dirLightShader;
	};
}