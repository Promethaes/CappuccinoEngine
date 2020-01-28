#pragma once
#include "ShaderProgram.h"
#include <vector>

namespace Cappuccino {
	/*
	Purp: a point light. be sure to update the view pos every frame
	and also, be sure to load the view matrix every frame (both externally)
	*/
	class PointLight {
	public:
		PointLight(const glm::vec2& windowSize, const std::vector<glm::vec3>& positions, const glm::vec3& ambientColour,
			const glm::vec3& diffuseColour, const glm::vec3& specularColour, float shininess);


		void recompile();


		void setPosition(const glm::vec3& pos, unsigned index);
		void setAmbient(const glm::vec3& colour, unsigned index);
		void setDiffuse(const glm::vec3& colour, unsigned index);
		void setSpecular(const glm::vec3& colour, unsigned index);
		void setShininess(float scalar);

		/*
		Purp: make the light at index active/inactive
		*/
		void setActive(unsigned index, bool active);
		bool isActive(unsigned index);

		/*
		Purp: make the light at index active/inactive, and set its position
		*/
		void setActive(unsigned index, bool active, const glm::vec3& newPos);

		void resendLights();

		//returns a vector of booleans that determine whether or not the light is on
		std::vector <bool>& getActives() { return _active; }
		std::vector <glm::vec3>& getPositions() { return _positions; }
		glm::vec3& getAmbient() { return _ambientColour; }
		glm::vec3& getDiffuse() { return _diffuseColour; }
		glm::vec3& getSpecular() { return _specularColour; }
		float& getShininess() { return _shininess; }

		Shader _pointLightShader;
	protected:
		bool _UI = false;

		//Shader active is used when a lot of uniforms are being sent at once so that the shader doesn't get rebound every iteration
		bool shaderActive = false;
		glm::vec2 _windowSize;
		std::vector<glm::vec3> _positions;
		std::vector<bool> _active;
		glm::vec3 _ambientColour;
		glm::vec3 _diffuseColour;
		glm::vec3 _specularColour;
		float _shininess;
	};
}