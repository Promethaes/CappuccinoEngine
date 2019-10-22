#pragma once
#include "Cappuccino/GameObject.h"
#include "Cappuccino/XInputManager.h"
namespace Cappuccino {
	class F16 :public GameObject {
	public:
		F16(const std::string& path,const Shader& SHADER, const Sedna::XInputManager* manager, unsigned controllerIndex);

		void childUpdate(float dt) override;

		static Texture* text1;
		static Texture* text2;
		static Mesh* mesh;
	private:
		Sedna::XInputController* _controller = nullptr;
	};

}