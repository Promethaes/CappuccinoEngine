#include "Cappuccino/Test Scene.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cstdlib>
#include "Cappuccino/Input.h"
#include "Cappuccino/Events.h"


namespace Cappuccino {
	Cappuccino::TestScene::TestScene(bool firstScene)
		:Scene(firstScene)
	{
		_f16._f16Pos.z = 1;
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
		specularMap = Texture(std::string(std::getenv("CappuccinoPath")) + "Assets\\Textures\\Metal_specmap.png", TextureType::SpecularMap);
		for (int i = 0; i < 4; i++) {
			cubes.push_back(Cube(vertices2, 288, new Texture(std::string(std::getenv("CappuccinoPath")) + "Assets\\Textures\\container2.png", TextureType::DiffuseMap), true));
			cubes.back().position = glm::vec3(i, i, i);
		}
		_f16._f16Pos = glm::vec4(4, 4, 4, _f16._f16Pos.w);

		for (int i = 0; i < 4; i++)
			lightCubes.push_back(Cube(vertices2, 288, new Texture(std::string(std::getenv("CappuccinoPath")) + "Assets\\Textures\\container2.png", TextureType::DiffuseMap), true));


#endif
	}

	bool Cappuccino::TestScene::init()
	{

		return _initialized = true;
	}

	bool Cappuccino::TestScene::exit()
	{
		return _shouldExit = true;
	}

	void Cappuccino::TestScene::childUpdate(float dt)
	{
		//centre cube


		glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f,  0.2f,  2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3(0.0f,  0.0f, -3.0f)
		};

		glm::vec3 lightColor = glm::vec4(2.0f, 2.0f, 2.0f, 1);
		//cubeeeee
#if CUBETEST
		rotate += dt;
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, cubes.back().getTexture().getTextureId());
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap.getTextureId());
		for (unsigned int i = 0; i < cubes.size(); i++)
		{
			_lightingShader.loadModelMatrix(pointLightPositions[i] + glm::vec3(3,3,3), std::nullopt, glm::vec3(i, i, i), rotate);
			cubes[i].draw();
		}
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, 0);

		_lightcubeShader.use();

		_lightcubeShader.setUniform("Colour", lightColor);
		for (unsigned i = 0; i < lightCubes.size(); i++) {
			_lightcubeShader.loadModelMatrix(pointLightPositions[i], 0.5f, std::nullopt, std::nullopt);
			lightCubes[i].draw();
		}

		_lightcubeShader.loadViewMatrix(*Scene::defaultCamera);
		_lightcubeShader.loadProjectionMatrix(800.0f * 2, 600.0f * 2);

		_lightingShader.use();
#endif
		_lightingShader.loadViewMatrix(*Scene::defaultCamera);
		_lightingShader.loadProjectionMatrix(800.0f * 2, 600.0f * 2);
		_lightingShader.setUniform("material.diffuse", (int)0);
		_lightingShader.setUniform("material.specular", (int)1);
		_lightingShader.setUniform("material.shininess", (float)32.0f);

		// directional light
		_lightingShader.setUniform("dirLight.direction", -0.2f, -1.0f, -0.3f);
		_lightingShader.setUniform("dirLight.ambient", 0.05f, 0.05f, 0.05f);
		_lightingShader.setUniform("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
		_lightingShader.setUniform("dirLight.specular", 0.5f, 0.5f, 0.5f);

		for (unsigned i = 0; i < 4; i++) {

			_lightingShader.setUniform("pointLights[" + std::to_string(i) + "].position", pointLightPositions[i]);
			_lightingShader.setUniform("pointLights[" + std::to_string(i) + "].ambient", 0.05f * 3, 0.05f * 3, 0.05f * 3);
			_lightingShader.setUniform("pointLights[" + std::to_string(i) + "].diffuse", 0.8f * 3, 0.8f * 3, 0.8f * 3);
			_lightingShader.setUniform("pointLights[" + std::to_string(i) + "].specular", 1.0f * 3, 1.0f * 3, 1.0f * 3);
			_lightingShader.setUniform("pointLights[" + std::to_string(i) + "].constant", 1.0f * 3);
			_lightingShader.setUniform("pointLights[" + std::to_string(i) + "].linear", 0.09f * 3);
			_lightingShader.setUniform("pointLights[" + std::to_string(i) + "].quadratic", 0.032f * 3);
		}

		glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f); // decrease the influence
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); // low influence

		_lightingShader.setUniform("light.ambient", ambientColor);
		_lightingShader.setUniform("light.diffuse", diffuseColor);
		_lightingShader.setUniform("light.specular", 1.0f, 1.0f, 1.0f);


		_lightingShader.setUniform("viewPos", Scene::defaultCamera->getPosition());





#if NETWORKTEST
		if (isEvent(Events::Up)) {
			testNetwork.sendMessage("1", "192.168.0.101");
		}
		else
			testNetwork.sendMessage("0", "192.168.0.101");

		sendString(testNetwork.listen(true));
		if (std::stoi(info) == 1)
			_f16._f16Pos += _f16._f16ModelMat[0] * 2.5f * dt;
#endif
		glm::vec3 camPos = defaultCamera->getPosition();
		if (isEvent(Events::Up))
			camPos += 2.5f * dt * defaultCamera->getFront();
		if (isEvent(Events::Down))
			camPos -= 2.5f * dt * defaultCamera->getFront();

		defaultCamera->setPosition(camPos);
		//defaultCamera->lookAt(_f16._f16Pos);
		//defaultCamera->setPosition(glm::vec3(_f16._f16Pos) + glm::vec3(-5, 5, 0));
	}

}