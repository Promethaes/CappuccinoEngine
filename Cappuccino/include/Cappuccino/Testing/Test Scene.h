#pragma once

#include "Cappuccino/CappMacros.h"
#include "Cappuccino/SceneManager.h"
#include "Cappuccino/Camera.h"
#include "Cappuccino/ShaderProgram.h"
#include "Cappuccino/XInputManager.h"
#include "Cappuccino/SceneManager.h"
#include "Cappuccino/Testing/Cube.h"
#include "Cappuccino/Testing/f16.h"
#include "Cappuccino/CappMacros.h"
#include "Cappuccino/Player.h"
#include "Cappuccino/Ray.h"
#include "Cappuccino/FontManager.h"

#if NETWORKTEST
#include "Cappuccino/Networking.h"
#endif

namespace Cappuccino {
	class TestScene : public Scene {
	public:
		TestScene(bool firstScene);

		bool init() override;
		bool exit() override;

		void childUpdate(float dt) override;

		void mouseFunction(double xpos, double ypos) override;
		
	private:
		Shader _lightingShader{ "lightingShader.vert","lightingShader.frag" };
		F16 _f16{ CAPP_PATH + R"(Assets\Mesh\f16.obj)", _lightingShader, nullptr, 0 };
		F16 _f162{ CAPP_PATH + R"(Assets\Mesh\f16.obj)", _lightingShader, nullptr, 0 };
		Player* testPlayer = new Player(_lightingShader, std::vector<Texture*>{}, std::vector<Mesh*>{},1.0f);
		
		RigidBody testBody = RigidBody( glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0, 0, 0));
		glm::mat4 viewMat = glm::mat4(1.0f);
		glm::mat4 projMat = glm::mat4(1.0f);
		
#if TEXTRENDERTEST
		Shader fontShader{ "font.vert","font.frag" };
		Text testText{"Test",fontShader,glm::vec2(20.0f,20.0f),glm::vec3(1.0f,1.0f,1.0f),0.5f};
#endif

#if CROSSHAIRTEST
		Ray testRay;
		RayBox testSection{ glm::vec3(0.0f,0.0f,0.0f),glm::vec3(1.0f,1.0f,1.0f) };
#endif
		//networking test
#if NETWORKTEST
		//put your IP here
		Network testNetwork{ 54000,"192.168.0.101" };
#endif

		//cube test
#if CUBETEST
		Shader _lightcubeShader{ "lightingShader.vert","lampShader.frag" };
		std::vector<Cube> lightCubes;

		float rotate = 0;
		std::vector<Cube> cubes;
		Texture specularMap;
#endif
		float lastX = 400, lastY = 300;
		float yaw = -90.0f;
		float pitch = 0.0f;
		bool firstMouse = true;
	};
}