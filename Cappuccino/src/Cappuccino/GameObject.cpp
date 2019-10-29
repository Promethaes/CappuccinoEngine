#include "Cappuccino/GameObject.h"
namespace Cappuccino {
	std::vector<GameObject*> GameObject::gameObjects = {};
	GameObject::GameObject(const Shader& _shader, const std::vector<Texture*>& _textures, const std::vector<Mesh*>& _meshs, const std::optional<float>& mass)
		:_shader(_shader), _rigidBody(glm::vec3(_transform._translateMat[3].x, _transform._translateMat[3].y, _transform._translateMat[3].z),
			glm::vec3(0, 0, 0), mass.has_value() ? mass.value() : 1)
	{
		//mesh = new Mesh(MESH);

		//set the state manually

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


	}
	void GameObject::baseUpdate(float dt)
	{
		childUpdate(dt);

		_rigidBody.update(dt,_transform._transformMat);
		_transform._translateMat[3].x = _rigidBody._position.x;
		_transform._translateMat[3].y = _rigidBody._position.y;
		_transform._translateMat[3].z = _rigidBody._position.z;
		_transform.update();

		draw();
	}


	void GameObject::setPosition(const glm::vec3& newPos)
	{
		//_rigidBody.updatePosition(position = _transform.translate(newPos));
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
	
	
	bool GameObject::checkCollision(GameObject& other)
	{
		if (!_rigidBody._hitBoxes[0].checkCollision(other._rigidBody._hitBoxes[0], other._rigidBody._position, _rigidBody._position))
			return false;
		for (unsigned i = 1; i < _rigidBody._hitBoxes.size() - 1; i++)
			for (unsigned n = 1; i < other._rigidBody._hitBoxes.size() - 1; n++)
			{
				if (_rigidBody._hitBoxes[i].checkCollision(other._rigidBody._hitBoxes[n], other._rigidBody._position, _rigidBody._position))
					return true;
			}
	}
	
	void GameObject::draw()
	{
		//set active shader
		glUseProgram(_shader.getID());

		//bind the textures to their proper slots
		for (auto x : _textures) {
			if (x->type == TextureType::DiffuseMap) {
				//fix this later!!!!!
				//TODO
				x->bind(0);
			}
			else if (x->type == TextureType::SpecularMap) {
				x->bind(1);
			}
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