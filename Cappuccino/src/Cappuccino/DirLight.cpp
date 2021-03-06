#include "Cappuccino/DirLight.h"
#include "Cappuccino/ResourceManager.h"

namespace Cappuccino {

	DirLight::DirLight(const glm::vec2& windowSize, const glm::vec3& direction, const glm::vec3& ambientColour, const glm::vec3& diffuseColour, const glm::vec3& specularColour, const float shininess)
	{
		_dirLightShader = *ShaderLibrary::loadShader("DefaultDirectionalLight", "lightingShader.vert", "dirLight.frag");
		_dirLightShader.use();
		_dirLightShader.loadProjectionMatrix(windowSize.x, windowSize.y);
		_dirLightShader.setUniform("material.diffuse", 0);
		_dirLightShader.setUniform("material.specular", 1);
		_dirLightShader.setUniform("material.normalMap", 2);

		setDirection(direction);
		setAmbient(ambientColour);
		setDiffuse(diffuseColour);
		setSpecular(specularColour);
		setShininess(shininess);
	}

	void DirLight::updateViewPos(const glm::vec3& cameraPos)
	{
		_dirLightShader.use();
		_dirLightShader.setUniform("viewPos", cameraPos);
	}

	void DirLight::setDirection(const glm::vec3& dir)
	{
		_dirLightShader.use();
		_direction = dir;
		_dirLightShader.setUniform("dirLight.direction", _direction);
	}

	void DirLight::setAmbient(const glm::vec3& colour)
	{
		_dirLightShader.use();
		_ambientColour = colour;
		_dirLightShader.setUniform("dirLight.ambient", _ambientColour);
	}

	void DirLight::setDiffuse(const glm::vec3& colour)
	{
		_dirLightShader.use();
		_diffuseColour = colour;
		_dirLightShader.setUniform("dirLight.diffuse", _diffuseColour);
	}

	void DirLight::setSpecular(const glm::vec3& colour)
	{
		_dirLightShader.use();
		_specularColour = colour;
		_dirLightShader.setUniform("dirLight.specular", _specularColour);
	}

	void DirLight::setShininess(const float scalar)
	{
		_dirLightShader.use();
		_shininess = scalar;
		_dirLightShader.setUniform("material.shininess", _shininess);
	}

}
