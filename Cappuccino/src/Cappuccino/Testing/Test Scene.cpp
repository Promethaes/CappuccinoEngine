#include "Cappuccino/Testing/Test Scene.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cstdlib>
#include "Cappuccino/Input.h"
#include "Cappuccino/Events.h"
#include "Cappuccino/CappMath.h"

namespace Cappuccino {

	TestScene::TestScene(bool firstScene)
		:Scene(firstScene)
#if CROSSHAIRTEST
		, testRay(testPlayer->getCamera()->getFront(), testPlayer->getCamera()->getPosition())
#endif
	{
		testPlayer->_rigidBody._position = glm::vec3(0, 0, 3);


		_f16._rigidBody._hitBoxes.push_back(HitBox(glm::vec3(0, 0, 0), 1.0f));
		_f16._rigidBody._hitBoxes.push_back(HitBox(glm::vec3(0, 0, 0), 1.0f));

		_f162._transform.rotate(glm::vec3(0, 0, 1), 90);
		_f162._rigidBody._hitBoxes.push_back(HitBox(glm::vec3(0, 0, 0), glm::vec3(2.0f, 2.0f,2.0f)));
		_f162._rigidBody._hitBoxes.push_back(HitBox(glm::vec3(0, 0, 0), glm::vec3(2.0f, 2.0f,2.0f)));
		

		
		
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
		specularMap = Texture(CAPP_PATH + R"(Assets\Textures\Metal_specmap.png)", TextureType::SpecularMap);
		for (int i = 0; i < 4; i++) {
			cubes.push_back(Cube(vertices2, 288, new Texture(CAPP_PATH + R"(Assets\Textures\container2.png)", TextureType::DiffuseMap), true));
			cubes.back().position = glm::vec3(i, i, i);
		}
		cubes.push_back(Cube(vertices3, 288, new Texture(CAPP_PATH + R"(Assets\Textures\container2.png)", TextureType::DiffuseMap), true));

		for (int i = 0; i < 4; i++)
			lightCubes.push_back(Cube(vertices2, 288, new Texture(CAPP_PATH + R"(Assets\Textures\container2.png)", TextureType::DiffuseMap), true));

#endif	
		
	}

	bool Cappuccino::TestScene::init()
	{
		_f16.setActive(true);
		_f162.setActive(true);
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
		
		glm::vec3 lightColor = glm::vec4(2.0f, 2.0f, 2.0f, 1.0f);
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

		_lightcubeShader.loadViewMatrix(*testPlayer->getCamera());
		_lightcubeShader.loadProjectionMatrix(800.0f * 2, 600.0f * 2);

		_lightingShader.use();
#endif
		_lightingShader.loadViewMatrix(*testPlayer->getCamera());
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

		_lightingShader.setUniform("viewPos", testPlayer->getCamera()->getPosition());

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


		viewMat = testPlayer->getCamera()->whereAreWeLooking();
		
		projMat = glm::perspective(glm::radians(45.0f), (800.0f * 2) / (600.0f * 2), 0.1f, 100.0f);
		testBody.setViewProjMat(viewMat, projMat);

#if CROSSHAIRTEST
		static float u = 0.0f;
		static bool reverse = false;

		if (!reverse)
			u += dt;
		else
			u -= dt;

		if (u >= 1.0f) {
			u = 1.0f;
			reverse = true;
		}
		else if (u <= 0.0f) {
			u = 0.0f;
			reverse = false;
		}



		testPlayer->_crosshairShader.use();
		if (testSection.intersecting(testRay)||_f162._rigidBody.intersecting(testRay))
		{
			testPlayer->_crosshairShader.setUniform("colour", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
		}
		else
			testPlayer->_crosshairShader.setUniform("colour", glm::vec4(1.0f, 1.0f, 1.0f, Math::lerp(0.0f, 1.0f, u)));
#endif

		viewMat = testPlayer->getCamera()->whereAreWeLooking();
		_f16._rigidBody._shader.use();
		_f16._rigidBody._shader.loadViewMatrix(*testPlayer->getCamera());
		_f162._rigidBody._shader.use();
		_f162._rigidBody._shader.loadViewMatrix(*testPlayer->getCamera());

		projMat = glm::perspective(glm::radians(45.0f), (800.0f * 2) / (600.0f * 2), 0.1f, 100.0f);
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
			testPlayer->getCamera()->doMouseMovement(xOffset, yOffset);
			glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
	}
}