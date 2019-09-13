#include "Cappuccino/f16.h"
#include "Cappuccino/Events.h"
#include "Cappuccino/Input.h"
#define _CRT_SECURE_NO_WARNINGS

namespace Cappuccino {
	F16::F16(const Mesh& MESH, const Shader& SHADER, const Sedna::XinputManager* manager, unsigned controllerIndex)
		:GameObject(SHADER,  std::vector<Texture*>{new Texture(std::string(std::getenv("CappuccinoPath")) + "Assets/Textures/Metal_specmap.png", TextureType::SpecularMap), new Texture(std::string(std::getenv("CappuccinoPath")) + "Assets/Textures/container2.png", TextureType::SpecularMap)},
							 std::vector<Mesh*>	 {new Mesh(MESH)})
	{
		id = "F16";
		if (manager != nullptr)
			_controller = manager->getController(controllerIndex);
	}
	void F16::childUpdate(float dt)
	{

		if (isEvent(Events::W))
			_f16Pos += _f16ModelMat[0] * 2.5f * dt;
		if (isEvent(Events::A)) {
			_f16RotationV -= glm::vec3(1, 0, 0);
			_f16RotationFloat -= 0.005f;
		}
		if (isEvent(Events::D)) {
			_f16RotationV += glm::vec3(1, 0, 0);
			_f16RotationFloat += 0.005f;
		}
		if (isEvent(Events::Q)) {
			_f16RotationV -= glm::vec3(0, 1, 0);
			_f16RotationFloat -= 0.005f;
		}
		if (isEvent(Events::E)) {
			_f16RotationV += glm::vec3(0, 1, 0);
			_f16RotationFloat += 0.005f;
		}
		if (isEvent(Events::C)) {
			_f16RotationV -= glm::vec3(0, 0, 1);
			_f16RotationFloat -= 0.005f;
		}
		if (isEvent(Events::Z)) {
			_f16RotationV += glm::vec3(0, 0, 1);
			_f16RotationFloat += 0.005f;
		}
		draw();
	}

	void Cappuccino::F16::draw()
	{
		glUseProgram(_shader.getId());

		for (unsigned i = 0; i < _textures.size(); i++) {
			if (_textures[i]->type == TextureType::DiffuseMap)
				_textures[i]->bind(0);
			else if (_textures[i]->type == TextureType::SpecularMap)
				_textures[i]->bind(1);
		}

		for (auto x : _meshs) {
			_f16ModelMat = x->modelMatrix;
			x->modelMatrix = _shader.loadModelMatrix(_f16Pos, std::nullopt, _f16RotationV, _f16RotationFloat);
			x->draw();
		}

		for (unsigned i = 0; i < _textures.size(); i++) {
			if (_textures[i]->type == TextureType::DiffuseMap)
				_textures[i]->unbind(0);
			else if (_textures[i]->type == TextureType::SpecularMap)
				_textures[i]->unbind(1);
		}
	}
}
