#include "Cappuccino/PointLight.h"

namespace Cappuccino {
	PointLight::PointLight(const glm::vec2& windowSize, const glm::vec3& position, const glm::vec3& ambientColour, const glm::vec3& diffuseColour, const glm::vec3& specularColour, float shininess)
		:_pointLightShader{"lightingShader.vert","pointLight.frag"}
	{
		_pointLightShader.use();
		_pointLightShader.loadProjectionMatrix(windowSize.x, windowSize.y);
		_pointLightShader.setUniform("material.diffuse", 0);
		_pointLightShader.setUniform("material.specular", 1);
		_pointLightShader.setUniform("material.normalMap", 2);

		setPosition(position);
		setAmbient(ambientColour);
		setDiffuse(diffuseColour);
		setSpecular(specularColour);
		setShininess(shininess);
	}

	void PointLight::updateViewPos(const glm::vec3& cameraPos)
	{
		_pointLightShader.use();
		_pointLightShader.setUniform("viewPos", cameraPos);
	}

	void PointLight::setPosition(const glm::vec3& pos)
	{
		_pointLightShader.use();
		_position = pos;
		_pointLightShader.setUniform("pointLight.position", _position);
	}

	void PointLight::setAmbient(const glm::vec3& colour)
	{
		_pointLightShader.use();
		_ambientColour = colour;
		_pointLightShader.setUniform("pointLight.ambient", _ambientColour);
	}

	void PointLight::setDiffuse(const glm::vec3& colour)
	{
		_pointLightShader.use();
		_diffuseColour = colour;
		_pointLightShader.setUniform("pointLight.diffuse", _diffuseColour);
	}

	void PointLight::setSpecular(const glm::vec3& colour)
	{
		_pointLightShader.use();
		_specularColour = colour;
		_pointLightShader.setUniform("pointLight.specular", _specularColour);
	}

	void PointLight::setShininess(float scalar)
	{
		_pointLightShader.use();
		_shininess = scalar;
		_pointLightShader.setUniform("material.shininess", _shininess);
	}
}