#pragma once
#include "Cappuccino/Scene Manager.h"
#include "Cappuccino/Camera.h"
#include "Cappuccino/ShaderProgram.h"
#include "Cappuccino/XinputManager.h"
#include "Cappuccino/Scene Manager.h"
#include "Cappuccino/Cube.h"
#include "Cappuccino/f16.h"
#include "Cappuccino/CappMacros.h"
#include "Cappuccino/Networking.h"


namespace Cappuccino {

	class TestScene : public Scene {
	public:
		TestScene(bool firstScene);

		bool init() override;
		bool exit() override;
		
		void childUpdate(float dt) override;

	private:
		Shader _lightingShader{"lightingShader.vert","lightingShader.frag"};
		F16 _f16{ Mesh(std::string(std::getenv("CappuccinoPath")) + "Assets\\Mesh\\f16.obj"), _lightingShader,nullptr,0 };
#if CUBETEST
		float rotate = 0;
		std::vector<Cube> cubes;
		Texture specularMap;
#endif
	};
}