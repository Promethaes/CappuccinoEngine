#include "Cappuccino/Game Object.h"
namespace Cappuccino {
	std::vector<GameObject*> GameObject::gameObjects = {};
	GameObject::GameObject(const Shader& _shader, const std::vector<Texture*>& _textures, const std::vector<Mesh*>& _meshs)
		:_shader(_shader)
	{
		//mesh = new Mesh(MESH);

		this->_textures = _textures;
		this->_meshes = _meshs;

		loadTextures();
		loadMesh();

		gameObjects.push_back(this);
	}
	void GameObject::baseUpdate(float dt)
	{
		childUpdate(dt);
		draw();
	}
	void GameObject::setPosition(const glm::vec3& newPos)
	{
		for (auto x : _meshes) {
			x->transform.translate(newPos);
		}
	}
	void GameObject::draw()
	{
		//set active shader
		glUseProgram(_shader.getID());

		//bind the textures to their proper slots
		for (auto x : _textures) {
			if (x->type == TextureType::DiffuseMap)
				x->bind(0);
			else if (x->type == TextureType::SpecularMap)
				x->bind(1);
		}

		for (auto x : _meshes) {
			x->transform.transformMat = _shader.loadModelMatrix(x->transform.transformMat);
			x->draw();
		}

		//unloads the textures
		for (auto x : _textures) {
			if (x->type == TextureType::DiffuseMap)
				x->unbind(0);
			else if (x->type == TextureType::SpecularMap)
				x->unbind(1);
		}
	}
	void GameObject::loadTextures()
	{
		if (!_loadedTextures) {
			for (unsigned i = 0; i < _textures.size(); i++) {
				if (_textures[i]->load())
					continue;
				else
					return;
			}
			_loadedTextures = true;
		}
	}
	void GameObject::loadMesh()
	{
		if (!_loadedMesh) {
			for (unsigned i = 0; i < _meshes.size(); i++) {
				if (_meshes[i]->loadMesh())
					continue;
				else
					return;
			}
			_loadedMesh = true;
		}
	}
}