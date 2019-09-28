#include "Cappuccino/Game Object.h"
namespace Cappuccino {
	std::vector<GameObject*> GameObject::gameObjects = {};
	GameObject::GameObject(const Shader& _shader, const std::vector<Texture*>& _textures, const std::vector<Mesh*>& _meshs)
		:_shader(_shader)
	{
		//mesh = new Mesh(MESH);

		_state = new State();

		this->_textures = _textures;
		this->_meshes = _meshs;

		loadTextures();
		loadMesh();

		gameObjects.push_back(this);
	}
	void GameObject::baseUpdate(float dt)
	{
		childUpdate(dt);
		
			transform.update();
		
		draw();
	}
	void GameObject::setPosition(const glm::vec3& newPos)
	{
		
			position = transform.translate(newPos);
	}
	void GameObject::scaleX(const float sizeScalar)
	{
		
			transform.scale(glm::vec3(1, 0, 0), sizeScalar);
	}
	void GameObject::scaleY(const float sizeScalar)
	{
		
			transform.scale(glm::vec3(0, 1, 0), sizeScalar);
	}
	void GameObject::scaleZ(const float sizeScalar)
	{
		
			transform.scale(glm::vec3(0, 0, 1), sizeScalar);
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

			transform._transformMat = _shader.loadModelMatrix(transform._transformMat);
		for (auto x : _meshes) {
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