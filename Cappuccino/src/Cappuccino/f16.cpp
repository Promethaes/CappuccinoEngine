#include "Cappuccino/f16.h"
#include "Cappuccino/Events.h"
#include "Cappuccino/Input.h"

namespace Cappuccino {
	F16::F16(const Mesh& MESH, const Shader& SHADER, const Sedna::XinputManager* manager, unsigned controllerIndex)
		:GameObject(SHADER, std::vector<Texture*>{new Texture(std::string(std::getenv("CappuccinoPath")) + "Assets/Textures/Metal_specmap.png", TextureType::SpecularMap), new Texture(std::string(std::getenv("CappuccinoPath")) + "Assets/Textures/container2.png", TextureType::SpecularMap)},
			std::vector<Mesh*>	 {new Mesh(MESH)})
	{
		id = "F16";
		if (manager != nullptr)
			_controller = manager->getController(controllerIndex);
	}
	void F16::childUpdate(float dt)
	{
		if (isEvent(Events::Alt))
			for (auto x : _meshes)
				position = x->transform.translate(x->transform.transformMat[0] * 2.5f * dt);

		if (isEvent(Events::A)) {
			for (auto x : _meshes)
				x->transform.rotate(x->transform.transformMat[0], -dt);
		}
		if (isEvent(Events::D)) {
			for (auto x : _meshes)
				x->transform.rotate(x->transform.transformMat[0], dt);
		}
		if (isEvent(Events::Q)) {
			for (auto x : _meshes)
				x->transform.rotate(x->transform.transformMat[1], -dt);
		}
		if (isEvent(Events::E)) {
			for (auto x : _meshes)
				x->transform.rotate(x->transform.transformMat[1], dt);
		}
		if (isEvent(Events::C)) {
			_f16RotationV -= glm::vec3(0, 0, 1);
			_f16RotationFloat -= 4 * dt;
		}
		if (isEvent(Events::Z)) {
			_f16RotationV += glm::vec3(0, 0, 1);
			_f16RotationFloat += 4 * dt;
		}
	}
}