#pragma once
#include "Cappuccino/Game Object.h"
#include "Cappuccino/XinputManager.h"
namespace Cappuccino {
	class F16 :public GameObject {
	public:
		F16(const std::string& path,const Shader& SHADER, const Sedna::XinputManager* manager, unsigned controllerIndex);

		void childUpdate(float dt) override;

		
	private:
		Sedna::XinputController* _controller = nullptr;
	};

}