#pragma once
#include "Cappuccino/Scene Manager.h"
#include "Cappuccino/Camera.h"
#include "Cappuccino/ShaderProgram.h"
#include "Cappuccino/XinputManager.h"
#include "Cappuccino/Scene Manager.h"
#include "Cappuccino/Testing/Cube.h"
#include "Cappuccino/Testing/f16.h"
#include "Cappuccino/CappMacros.h"
#include "Cappuccino/Networking.h"
#include "Cappuccino/Player.h"
#include "Cappuccino/Primitive.h"
#include "Cappuccino/Ray.h"

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

		static Primitives::Cube testPrim;
		static Primitives::Cube testPrim2;
	private:
		Shader _lightingShader{ "lightingShader.vert","lightingShader.frag" };
		F16 _f16{ std::string(std::getenv("CappuccinoPath")) + "Assets\\Mesh\\f16.obj", _lightingShader,nullptr,0 };
		///F16 _f162{std::string(std::getenv("CappuccinoPath")) + "Assets\\Mesh\\f16.obj", _lightingShader,nullptr,0 };
		Player* testPlayer = new Player(_lightingShader, std::vector<Texture*>{}, std::vector<Mesh*>{});
		Ray testRay;
		RayBox testSection{ glm::vec3(0.0f,0.0f,0.0f),glm::vec3(1.0f,1.0f,1.0f) };
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