#include "Cappuccino/Game Object.h"
namespace Cappuccino {
	std::vector<GameObject*> GameObject::gameObjects = {};
	GameObject::GameObject(const Shader& _shader, const std::vector<Texture*>& _textures, const std::vector<Mesh*>& _meshs)
		:_shader(_shader)
	{
		//mesh = new Mesh(MESH);

		//set the state manually
		_state = nullptr;
		_tempState = new State();

		this->_textures = _textures;
		this->_meshes = _meshs;

		loadTextures();
		loadMesh();

		gameObjects.push_back(this);
	}
	GameObject::~GameObject()
	{
		for (unsigned i = 0; i < _meshes.size(); i++) {
			_meshes[i]->unload();
			delete _meshes[i];
			i--;
		}
		for (unsigned i = 0; i < _textures.size(); i++) {
			if (_textures[i]->type == TextureType::DiffuseMap)
				_textures[i]->unbind(0);
			else if (_textures[i]->type == TextureType::SpecularMap)
				_textures[i]->unbind(1);

			delete _textures[i];
			i--;
		}

		delete _state;

	}
	void GameObject::baseUpdate(float dt)
	{
		childUpdate(dt);

		checkChangeState(dt, *_tempState);
		_state->update(dt,this);
		_transform.update();
        _rigidBody.update(dt);

		draw();
	}


	void GameObject::setPosition(const glm::vec3& newPos)
	{
		position = _transform.translate(newPos);
	}
	void GameObject::rotateX(const float rotateBy)
	{
		_transform.rotate(glm::vec3(1, 0, 0), rotateBy);
	}
	void GameObject::rotateY(const float rotateBy)
	{
		_transform.rotate(glm::vec3(0, 1, 0), rotateBy);
	}
	void GameObject::rotateZ(const float rotateBy)
	{
		_transform.rotate(glm::vec3(0, 0, 1), rotateBy);
	}
	void GameObject::scaleX(const float sizeScalar)
	{
		_transform.scale(glm::vec3(1, 0, 0), sizeScalar);
	}
	void GameObject::scaleY(const float sizeScalar)
	{
		_transform.scale(glm::vec3(0, 1, 0), sizeScalar);
	}
	void GameObject::scaleZ(const float sizeScalar)
	{
		_transform.scale(glm::vec3(0, 0, 1), sizeScalar);
	}



	void GameObject::setStateChange(const State& newState)
	{
		stateChangeFlag = true;
		if (_tempState != nullptr) {
			delete _tempState;
			_tempState = nullptr;
		}
		*_tempState = newState;
	}
	void GameObject::checkChangeState(float dt, const State& newState)
	{
		if (stateChangeFlag == false)
			return;

		_state->onExit(dt, this);
		delete _state;
		_state = nullptr;
		*_state = newState;
		_state->onEnter(dt, this);
		
		delete _tempState;
		_tempState = nullptr;
		stateChangeFlag = false;
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

		_transform._transformMat = _shader.loadModelMatrix(_transform._transformMat);
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