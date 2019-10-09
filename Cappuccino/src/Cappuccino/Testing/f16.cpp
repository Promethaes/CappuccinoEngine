#include "Cappuccino/Testing/f16.h"
#include "Cappuccino/Events.h"
#include "Cappuccino/Input.h"
#include "Cappuccino/CappMacros.h"

namespace Cappuccino {

	Texture* F16::text1 = nullptr;
	Texture* F16::text2 = nullptr;
	Mesh* F16::mesh = nullptr;
	F16::F16(const std::string& path, const Shader& SHADER, const Sedna::XinputManager* manager, unsigned controllerIndex)
		:GameObject(SHADER, std::vector<Texture*>{text1 == nullptr ? text1 = new Texture(std::string(std::getenv("CappuccinoPath")) + "Assets/Textures/Metal_specmap.png", TextureType::SpecularMap) : text1, text2 == nullptr ? text2 = new Texture(std::string(std::getenv("CappuccinoPath")) + "Assets/Textures/container2.png", TextureType::SpecularMap) : text2},
			std::vector<Mesh*>{mesh == nullptr ? mesh = new Mesh(path) : mesh},std::nullopt,CAPP_PATH + "Assets/Mesh/Cube.obj",10)
	{
		_state = new State();
		id = "F16";
		if (manager != nullptr)
			_controller = manager->getController(controllerIndex);
	}
	void F16::childUpdate(float dt)
	{
		if (isEvent(Events::Alt))
			_rigidBody.setAccel(_transform._transformMat[0] * 2.5f,dt);

		if (isEvent(Events::A))
			_transform.rotate(glm::vec3(0, 1, 0), -dt);

		if (isEvent(Events::D))

			_transform.rotate(glm::vec3(0, 1, 0), dt);

		if (isEvent(Events::Q))

			_transform.rotate(glm::vec3(1, 0, 0), -dt);

		if (isEvent(Events::E))

			_transform.rotate(glm::vec3(1, 0, 0), dt);

		if (isEvent(Events::C))

			_transform.rotate(glm::vec3(0, 0, 1), -dt);

		if (isEvent(Events::Z))

			_transform.rotate(glm::vec3(0, 0, 1), dt);

	}
}