#include "Cappuccino/Test Scene.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Cappuccino::TestScene::TestScene(bool firstScene)
	:Scene(firstScene), specularMap("", TextureType::DiffuseMap)
{
#if CUBETEST
	float vertices2[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
	};
	specularMap = Texture(std::string(std::getenv("CappuccinoShotPath")) + "Assets/Textures/Metal_specmap.png",TextureType::SpecularMap);
	for (int i = 0; i < 10; i++) {
		cubes.push_back(Cube(vertices2, 288, new Texture(std::string(std::getenv("CappuccinoShotPath")) + "Assets/Textures/container2.png",TextureType::DiffuseMap), true));
		cubes.back().position = (glm::vec3(i + rand() % 5, rand() % 5 + 1, i + rand() % 5) *= -1);
	}
	specularMap.load();
#endif
}

bool Cappuccino::TestScene::init()
{
	
	return initialized = true;
}

bool Cappuccino::TestScene::exit()
{
	return shouldExit = true;
}

void Cappuccino::TestScene::childUpdate(float dt,Camera& defaultCamera)
{
	//centre cube
	

	glm::vec3 pointLightPositions[] = {
	glm::vec3(0.7f,  0.2f,  2.0f),
	glm::vec3(2.3f, -3.3f, -4.0f),
	glm::vec3(-4.0f,  2.0f, -12.0f),
	glm::vec3(0.0f,  0.0f, -3.0f)
	};
	//cubeeeee
#if CUBETEST
	rotate += dt;
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, cubes.back().getTexture().getTextureId());
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, specularMap.getTextureId());
	for (unsigned int i = 0; i < cubes.size(); i++)
	{
		lightingShader.loadModelMatrix(glm::vec3(i*2 + 1,i*2 + 1,i*2 + 1),std::nullopt, glm::vec3(i,i,i), rotate);
		cubes.back().draw();
	}
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 0);
#endif
	lightingShader.loadViewMatrix(defaultCamera);
	lightingShader.loadProjectionMatrix(800.0f * 2, 600.0f * 2);
	lightingShader.setInt("material.diffuse", 0);
	lightingShader.setInt("material.specular", 1);
	lightingShader.setFloat("material.shininess", 32.0f);

	// directional light
	lightingShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
	lightingShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
	lightingShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
	lightingShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);

	for (unsigned i = 0; i < 4; i++) {

		lightingShader.setVec3 ("pointLights[" + std::to_string(i) + "].position", pointLightPositions[i]);
		lightingShader.setVec3 ("pointLights[" + std::to_string(i) + "].ambient", 0.05f * 2, 0.05f * 2, 0.05f * 2);
		lightingShader.setVec3 ("pointLights[" + std::to_string(i) + "].diffuse", 0.8f * 2, 0.8f * 2, 0.8f * 2);
		lightingShader.setVec3 ("pointLights[" + std::to_string(i) + "].specular", 1.0f * 2, 1.0f * 2, 1.0f * 2);
		lightingShader.setFloat("pointLights[" + std::to_string(i) + "].constant", 1.0f * 2);
		lightingShader.setFloat("pointLights[" + std::to_string(i) + "].linear", 0.09 * 2);
		lightingShader.setFloat("pointLights[" + std::to_string(i) + "].quadratic", 0.032 * 2);
	}

	glm::vec3 lightColor = glm::vec4(2.0f, 2.0f, 2.0f, 1);
	glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f); // decrease the influence
	glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); // low influence

	lightingShader.setVec3("light.ambient", ambientColor);
	lightingShader.setVec3("light.diffuse", diffuseColor);
	lightingShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);


	lightingShader.setVec3("viewPos", defaultCamera.getPosition());
}
