#include "Cappuccino/Testing/Test Scene.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cstdlib>
#include "Cappuccino/Input.h"
#include "Cappuccino/Events.h"

namespace Cappuccino {
	Primitives::Cube TestScene::testPrim;
	Primitives::Cube TestScene::testPrim2;
	Cappuccino::TestScene::TestScene(bool firstScene)
		:Scene(firstScene), testRay(testPlayer->getCamera()->getFront(), testPlayer->getCamera()->getPosition())
	{
		testPrim.loadMesh();
		//testPrim._transform.scale(glm::vec3(1, 10, 1), 1.0f);
		testPrim._body.hitBox.back()._position = testPrim._transform.translate(glm::vec3(1.0f, 0.0f, 0.0f));
		testPrim._transform.update();

		testPrim2.loadMesh();
		testPrim2._body.hitBox.back()._position = testPrim2._transform.translate(glm::vec3(5, 0, 0));
		testPrim2._transform.update();
		///for (unsigned i = 0; i < GameObject::gameObjects.size(); i++) {
		///	GameObject::gameObjects[i]->setPosition(glm::vec3(i, i, i));
		///}
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
		float vertices3[] = {
			// positions          // normals           // texture coords
			-1.0f, -0.5f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
			 1.0f, -0.5f, -1.0f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
			 1.0f,  0.5f, -1.0f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
			 1.0f,  0.5f, -1.0f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
			-1.0f,  0.5f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
			-1.0f, -0.5f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

			-1.0f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
			 1.0f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
			 1.0f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
			 1.0f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
			-1.0f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
			-1.0f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

			-1.0f,  0.5f,  1.0f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
			-1.0f,  0.5f, -1.0f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
			-1.0f, -0.5f, -1.0f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
			-1.0f, -0.5f, -1.0f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
			-1.0f, -0.5f,  1.0f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
			-1.0f,  0.5f,  1.0f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

			 1.0f,  0.5f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
			 1.0f,  0.5f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
			 1.0f, -0.5f, -1.0f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
			 1.0f, -0.5f, -1.0f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
			 1.0f, -0.5f,  1.0f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
			 1.0f,  0.5f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

			-1.0f, -0.5f, -1.0f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
			 1.0f, -0.5f, -1.0f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
			 1.0f, -0.5f,  1.0f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
			 1.0f, -0.5f,  1.0f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
			-1.0f, -0.5f,  1.0f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
			-1.0f, -0.5f, -1.0f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

			-1.0f,  0.5f, -1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
			 1.0f,  0.5f, -1.0f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
			 1.0f,  0.5f,  1.0f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
			 1.0f,  0.5f,  1.0f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
			-1.0f,  0.5f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
			-1.0f,  0.5f, -1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
		};
		specularMap = Texture(std::string(std::getenv("CappuccinoPath")) + "Assets\\Textures\\Metal_specmap.png", TextureType::SpecularMap);
		for (int i = 0; i < 4; i++) {
			cubes.push_back(Cube(vertices2, 288, new Texture(std::string(std::getenv("CappuccinoPath")) + "Assets\\Textures\\container2.png", TextureType::DiffuseMap), true));
			cubes.back().position = glm::vec3(i, i, i);
		}
		cubes.push_back(Cube(vertices3, 288, new Texture(std::string(std::getenv("CappuccinoPath")) + "Assets\\Textures\\container2.png", TextureType::DiffuseMap), true));

		for (int i = 0; i < 4; i++)
			lightCubes.push_back(Cube(vertices2, 288, new Texture(std::string(std::getenv("CappuccinoPath")) + "Assets\\Textures\\container2.png", TextureType::DiffuseMap), true));

#endif
	}

	bool Cappuccino::TestScene::init()
	{
		_f16.setActive(true);
		testPlayer->setActive(true);

		_shouldExit = false;
		_initialized = true;
		return _initialized;
	}

	bool Cappuccino::TestScene::exit()
	{
		_f16.setActive(false);
		testPlayer->setActive(false);

		_initialized = false;
		_shouldExit = true;
		return _shouldExit;
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
		for (unsigned int i = 0; i < cubes.size() - 1; i++)
		{
			_lightingShader.loadModelMatrix(pointLightPositions[i] + glm::vec3(3, 3, 3), std::nullopt, glm::vec3(i, i, i), rotate);
			cubes[i].draw();
		}
		_lightingShader.loadModelMatrix(glm::vec3(0, -7, 0), 10, std::nullopt, std::nullopt);
		cubes.back().draw();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, 0);

		_lightcubeShader.use();

		for (unsigned i = 0; i < lightCubes.size(); i++) {
			_lightcubeShader.loadModelMatrix(pointLightPositions[i], 0.5f, std::nullopt, std::nullopt);
			lightCubes[i].draw();
		}

		_lightcubeShader.loadViewMatrix(/**Scene::defaultCamera*/*testPlayer->getCamera());
		_lightcubeShader.loadProjectionMatrix(800.0f * 2, 600.0f * 2);

		_lightingShader.use();
#endif
		_lightingShader.loadViewMatrix(/**Scene::defaultCamera*/*testPlayer->getCamera());
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

		_lightingShader.setUniform("viewPos", /*Scene::defaultCamera->getPosition()*/testPlayer->getCamera()->getPosition());

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
		///glm::vec3 camPos = defaultCamera->getPosition();
		///if (isEvent(Events::Up))
		///	camPos += 2.5f * dt * defaultCamera->getFront();
		///if (isEvent(Events::Down))
		///	camPos -= 2.5f * dt * defaultCamera->getFront();B
		///defaultCamera->setPosition(camPos);

		if (testPlayer->_input.keyboard->keyPressed(Events::T))
			SceneManager::changeScene(0);
		if (testPlayer->_input.keyboard->keyPressed(Events::F))
			_f16.setActive(false);

		_lightcubeShader.use();

		testPrim._transform.update();
		testPrim2._transform.update();

		testPrim._transform._transformMat = _lightcubeShader.loadModelMatrix(testPrim._transform._transformMat);
		testPrim.draw();
		testPrim2._transform._transformMat = _lightcubeShader.loadModelMatrix(testPrim2._transform._transformMat);
		testPrim2.draw();
		if (testPrim._body.hitBox.back().checkCollision(testPrim2._body.hitBox.back(), testPrim._body.getPosition(), testPrim2._body.getPosition()))
			CAPP_PRINT("Colliding");

#if CROSSHAIRTEST
		testPlayer->crosshairShader.use();
		if (testSection.intersecting(testRay)) {
			//CAPP_PRINT_N("Intersecting");
			testPlayer->crosshairShader.setUniform("colour", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
		}
		else
			testPlayer->crosshairShader.setUniform("colour", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
#endif
	}
	void TestScene::mouseFunction(double xpos, double ypos)
	{
		if (firstMouse)
		{
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}

		GLfloat xOffset = xpos - lastX;
		GLfloat yOffset = lastY - ypos;
		lastX = xpos;
		lastY = ypos;

		if (testPlayer->_input.keyboard->keyPressed(Events::Alt))
			glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		else {
			/*Scene::defaultCamera*/testPlayer->getCamera()->doMouseMovement(xOffset, yOffset);
			glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
	}
}