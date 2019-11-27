#include "Cappuccino/PointLight.h"

namespace Cappuccino {
	PointLight::PointLight(const glm::vec2& windowSize, const std::vector<glm::vec3>& positions, const glm::vec3& ambientColour, const glm::vec3& diffuseColour, const glm::vec3& specularColour, float shininess)
		:_pointLightShader{ "lightingShader.vert","pointLight.frag" }
	{
		_pointLightShader.use();
		_pointLightShader.loadProjectionMatrix(windowSize.x, windowSize.y);
		_pointLightShader.setUniform("material.diffuse", 0);
		_pointLightShader.setUniform("material.specular", 1);
		_pointLightShader.setUniform("material.normalMap", 2);
		_pointLightShader.setUniform("material.emissionMap", 3);
		_pointLightShader.setUniform("material.heightMap", 4);


		_pointLightShader.setUniform("numLights", (int)positions.size());


		_positions = positions;

		for (unsigned i = 0; i < _positions.size(); i++) {

			setPosition(_positions[i], i);
			setAmbient(ambientColour, i);
			setDiffuse(diffuseColour, i);
			setSpecular(specularColour, i);
			_pointLightShader.setUniform("pointLight[" + std::to_string(i) + "].constant", 1.0f);
			_pointLightShader.setUniform("pointLight[" + std::to_string(i) + "].linear", 0.0001f);
			_pointLightShader.setUniform("pointLight[" + std::to_string(i) + "].quadratic", 0.001f);
		}
			setShininess(shininess);

		_windowSize = windowSize;


	}

	void PointLight::recompile()
	{
		_pointLightShader.createShader();
		_pointLightShader.use();
		_pointLightShader.loadProjectionMatrix(_windowSize.x, _windowSize.y);
		_pointLightShader.setUniform("material.diffuse", 0);
		_pointLightShader.setUniform("material.specular", 1);
		_pointLightShader.setUniform("material.normalMap", 2);
		_pointLightShader.setUniform("material.emissionMap", 3);
		_pointLightShader.setUniform("material.heightMap", 4);

		for (unsigned i = 0; i < _positions.size(); i++) {

			setPosition(_positions[i], i);
			setAmbient(_ambientColour, i);
			setDiffuse(_diffuseColour, i);
			setSpecular(_specularColour, i);
			setShininess(_shininess);
		}
	}

	void PointLight::updateViewPos(const glm::vec3& cameraPos)
	{
		_pointLightShader.use();
		_pointLightShader.setUniform("viewPos", cameraPos);
	}

	void PointLight::setPosition(const glm::vec3& pos, unsigned index)
	{
		_pointLightShader.use();
		_positions[index] = pos;
		_pointLightShader.setUniform("pointLight[" + std::to_string(index) + "].position", _positions[index]);
	}

	void PointLight::setAmbient(const glm::vec3& colour, unsigned index)
	{
		_pointLightShader.use();
		_ambientColour = colour;
		_pointLightShader.setUniform("pointLight[" + std::to_string(index) + "].ambient", _ambientColour);
	}

	void PointLight::setDiffuse(const glm::vec3& colour, unsigned index)
	{
		_pointLightShader.use();
		_diffuseColour = colour;
		_pointLightShader.setUniform("pointLight[" + std::to_string(index) + "].diffuse", _diffuseColour);
	}

	void PointLight::setSpecular(const glm::vec3& colour, unsigned index)
	{
		_pointLightShader.use();
		_specularColour = colour;
		_pointLightShader.setUniform("pointLight[" + std::to_string(index) + "].specular", _specularColour);
	}

	void PointLight::setShininess(float scalar)
	{
		_pointLightShader.use();
		_shininess = scalar;
		_pointLightShader.setUniform("material.shininess", _shininess);
	}
}