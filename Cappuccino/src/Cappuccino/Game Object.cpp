#include "Cappuccino/Game Object.h"
namespace Cappuccino {

	std::vector<GameObject*> GameObject::gameObjects = {};
	GameObject::GameObject(const Shader& _shader, const std::vector<Texture*>& _textures, const std::vector<Mesh*>& _meshs)
		:_shader(_shader)
	{
		//mesh = new Mesh(MESH);

		this->_textures = _textures;
		this->_meshs = _meshs;

		loadTextures();
		loadMesh();

		gameObjects.push_back(this);
	}
	void GameObject::baseUpdate(float dt)
	{
		childUpdate(dt);
		_pos += _acceleration * dt;
	}
	void GameObject::draw()
	{
		//set active shader
		glUseProgram(_shader.getID());

		//bind the textures to their proper slots
		for (unsigned i = 0; i < _textures.size(); i++) {
			if (_textures[i]->type == TextureType::DiffuseMap)
				_textures[i]->bind(0);
			else if (_textures[i]->type == TextureType::SpecularMap)
				_textures[i]->bind(1);
		}
		//sets the model matrix variable to the fully transformed matrix, after loading the model matrix into the GPU
		for (unsigned i = 0; i < _meshs.size(); i++) {
			_modelMat = _meshs[i]->modelMatrix;
			_meshs[i]->modelMatrix = _shader.loadModelMatrix(_pos, std::nullopt, _rotationV, _rotationFloat);
			_meshs[i]->draw();
		}


		//unloads the textures
		for (unsigned i = 0; i < _textures.size(); i++) {
			if (_textures[i]->type == TextureType::DiffuseMap)
				_textures[i]->unbind(0);
			else if (_textures[i]->type == TextureType::SpecularMap)
				_textures[i]->unbind(1);
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

			for (unsigned i = 0; i < _meshs.size(); i++) {
				if (_meshs[i]->loadMesh())
					continue;
				else
					return;
			}
			_loadedMesh = true;
		}
	}
}