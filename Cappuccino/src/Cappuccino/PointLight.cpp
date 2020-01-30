#include "Cappuccino/PointLight.h"
#include "Cappuccino/ResourceManager.h"

using namespace Cappuccino;

PointLight::PointLight(const glm::vec2& windowSize, const std::vector<glm::vec3>& positions, const glm::vec3& ambientColour, const glm::vec3& diffuseColour, const glm::vec3& specularColour, const float shininess) {

	_pointLightShader = *ShaderLibrary::loadShader("DefaultPointLight", "lightingShader.vert", "pointLight.frag");
	_pointLightShader.use();
	_pointLightShader.loadProjectionMatrix(windowSize.x, windowSize.y);
	_pointLightShader.setUniform("material.diffuse", 0);
	_pointLightShader.setUniform("material.specular", 1);
	_pointLightShader.setUniform("material.normalMap", 2);
	_pointLightShader.setUniform("material.emissionMap", 3);
	_pointLightShader.setUniform("material.heightMap", 4);


	_pointLightShader.setUniform("numLights", (int)positions.size());


	_positions = positions;

	_pointLightShader.use();
	shaderActive = true;
	for (unsigned i = 0; i < _positions.size(); i++) {
		_active.push_back(true);

		setActive(i, _active[i]);
		setPosition(_positions[i], i);
		setAmbient(ambientColour, i);
		setDiffuse(diffuseColour, i);
		setSpecular(specularColour, i);
		_pointLightShader.setUniform("pointLight[" + std::to_string(i) + "].UI", _UI);
		_pointLightShader.setUniform("pointLight[" + std::to_string(i) + "].constant", 1.0f);
		_pointLightShader.setUniform("pointLight[" + std::to_string(i) + "].linear", 0.0001f);
		_pointLightShader.setUniform("pointLight[" + std::to_string(i) + "].quadratic", 0.001f);
	}
	setShininess(shininess);

	shaderActive = false;

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

void PointLight::setPosition(const glm::vec3& pos, unsigned index)
{
	if (!shaderActive)
		_pointLightShader.use();
	_positions[index] = pos;
	_positions[index].z -= 5.0f;
	_pointLightShader.setUniform("pointLight[" + std::to_string(index) + "].position", _positions[index]);
}

void PointLight::setAmbient(const glm::vec3& colour, unsigned index)
{
	if (!shaderActive)
		_pointLightShader.use();
	_ambientColour = colour;
	_pointLightShader.setUniform("pointLight[" + std::to_string(index) + "].ambient", _ambientColour);
}

void PointLight::setDiffuse(const glm::vec3& colour, unsigned index)
{
	if (!shaderActive)
		_pointLightShader.use();
	_diffuseColour = colour;
	_pointLightShader.setUniform("pointLight[" + std::to_string(index) + "].diffuse", _diffuseColour);
}

void PointLight::setSpecular(const glm::vec3& colour, unsigned index)
{
	if (!shaderActive)
		_pointLightShader.use();
	_specularColour = colour;
	_pointLightShader.setUniform("pointLight[" + std::to_string(index) + "].specular", _specularColour);
}

void PointLight::setShininess(const float scalar)
{
	if (!shaderActive)
		_pointLightShader.use();
	_shininess = scalar;
	_pointLightShader.setUniform("material.shininess", _shininess);
}
void PointLight::setActive(const unsigned index, const bool active)
{
	if (!shaderActive)
		_pointLightShader.use();
	if (index > _active.size() - 1) {
		printf("PointLight::Set active index out of range!\n");
		__debugbreak();
	}

	_active[index] = active;
	_pointLightShader.setUniform("pointLight[" + std::to_string(index) + "].active", active);
}
bool PointLight::isActive(unsigned index)
{
	if (index > _active.size() - 1) {
		printf("PointLight::Is active index out of range!\n");
		__debugbreak();
	}
	return _active[index];
}
void PointLight::setActive(const unsigned index, const bool active, const glm::vec3& newPos)
{
	setActive(index, active);
	if (!shaderActive)
		_pointLightShader.use();
	shaderActive = true;
	setPosition(newPos, index);
	shaderActive = false;
}
void PointLight::resendLights()
{
	if (!shaderActive)
		_pointLightShader.use();
	//set the new number of the lights
	_pointLightShader.setUniform("numLights", (int)_positions.size());

	shaderActive = true;

	//make sure that actives and positions are the same size
	while (_active.size() < _positions.size())
		_active.push_back(true);


	for (unsigned i = 0; i < _positions.size(); i++) {

		setActive(i, _active[i]);
		setPosition(_positions[i], i);
		setAmbient(_ambientColour, i);
		setDiffuse(_diffuseColour, i);
		setSpecular(_specularColour, i);
		_pointLightShader.setUniform("pointLight[" + std::to_string(i) + "].UI", _UI);
		_pointLightShader.setUniform("pointLight[" + std::to_string(i) + "].constant", 1.0f);
		_pointLightShader.setUniform("pointLight[" + std::to_string(i) + "].linear", 0.0001f);
		_pointLightShader.setUniform("pointLight[" + std::to_string(i) + "].quadratic", 0.001f);
	}
	setShininess(_shininess);

	shaderActive = false;

}
