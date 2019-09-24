#pragma once
#include "Cappuccino/Game Object.h"
#include "Cappuccino/XinputManager.h"
namespace Cappuccino {
	class F16 :public GameObject {
	public:
		F16(const Mesh& MESH, const Shader& SHADER, const Sedna::XinputManager* manager,unsigned controllerIndex);

		void childUpdate(float dt) override;
		glm::mat4 _f16ModelMat = glm::mat4(1.0f);
		glm::vec4 _f16Pos = glm::vec4(0,0,0,1);
	private:
		Sedna::XinputController* _controller = nullptr;
		glm::vec3 _f16RotationV = glm::vec3(1,1,1);
		float _f16RotationFloat = 0.0f;

	};
}