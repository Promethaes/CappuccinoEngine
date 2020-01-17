#include "Cappuccino/GameObject.h"
#include "Cappuccino/CappMacros.h"
using namespace Cappuccino;

std::vector<GameObject*> GameObject::gameObjects = {};

Texture* GameObject::defaultEmission = nullptr;
Texture* GameObject::defaultNormal = nullptr;
Texture* GameObject::defaultHeight = nullptr;

GameObject::GameObject(const Shader& _shader, const std::vector<Texture*>& textures, const std::vector<Mesh*>& _meshs, const std::optional<float>& mass, unsigned viewportNum)
	:_shader(_shader), _rigidBody(glm::vec3(_transform._translateMat[3].x, _transform._translateMat[3].y, _transform._translateMat[3].z),
		glm::vec3(0, 0, 0),
		mass.has_value() ? mass.value() : 1)
{
	_viewportNum = viewportNum;
	//mesh = new Mesh(MESH);

	//set the state manually

	this->_textures = textures;
	this->_meshes = _meshs;

	bool hasNormal = false, hasEmission = false, hasHeight = false;
	for (unsigned i = 0; i < this->_textures.size(); i++) {
		if (_textures[i]->type == TextureType::EmissionMap)
			hasEmission = true;
		else if (_textures[i]->type == TextureType::NormalMap)
			hasNormal = true;
		else if (_textures[i]->type == TextureType::HeightMap)
			hasHeight = true;

	}

	static bool initDefaultMaps = false;

	if (!initDefaultMaps) {
		defaultEmission = new Texture("defaultEmission.png", TextureType::EmissionMap);
		defaultNormal = new Texture("defaultNorm.png", TextureType::NormalMap);
		defaultHeight = new Texture("defaultHeight.png", TextureType::HeightMap);

		//after all maps are init one time, never do it again
		initDefaultMaps = true;
	}

	if (!hasEmission)
		_textures.push_back(defaultEmission);
	if (!hasNormal)
		_textures.push_back(defaultNormal);
	if (!hasHeight)
		_textures.push_back(defaultHeight);

	loadTextures();
	loadMesh();

	gameObjects.push_back(this);
}

GameObject::~GameObject() {

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


bool GameObject::checkCollision(GameObject* other) {
	return _rigidBody.checkCollision(other->_rigidBody);
}

bool Cappuccino::GameObject::willCollide(GameObject* other, const glm::vec3& direction, float dt)
{
	return _rigidBody.willCollide(other->_rigidBody, direction, dt);
}

bool GameObject::checkCollision(const HitBox& other, const glm::vec3& pos) {
	return _rigidBody.checkCollision(other, pos);
}

bool Cappuccino::GameObject::willCollide(const HitBox& other, const glm::vec3& direction, const glm::vec3& pos, float dt)
{
	return _rigidBody.willCollide(other, pos, direction, dt);
}

void GameObject::baseUpdate(float dt) {
	childUpdate(dt);
	collision(dt);
	_rigidBody.update(dt);

	_transform._position->x = _rigidBody._position.x;
	_transform._position->y = _rigidBody._position.y;
	_transform._position->z = _rigidBody._position.z;
	_transform.update();


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


void GameObject::draw()
{
	//set active shader
	glUseProgram(_shader.getID());

	for (unsigned i = 0; i < _meshes.size();i++) {

		//bind the textures to their proper slots
		for (auto x : _textures) {
			if (x->getTextureIndex() != i)
				continue;

			if (x->type == TextureType::DiffuseMap)
				x->bind(0);
			else if (x->type == TextureType::SpecularMap)
				x->bind(1);
			else if (x->type == TextureType::NormalMap)
				x->bind(2);
			else if (x->type == TextureType::EmissionMap)
				x->bind(3);
			else if (x->type == TextureType::HeightMap)
				x->bind(4);
		}

		_transform._transformMat = _shader.loadModelMatrix(_transform._transformMat);


		_meshes[i]->draw();


		//unloads the textures
		for (auto x : _textures) {
			if (x->type == TextureType::DiffuseMap)
				x->unbind(0);
			else if (x->type == TextureType::SpecularMap)
				x->unbind(1);
			else if (x->type == TextureType::NormalMap)
				x->unbind(2);
			else if (x->type == TextureType::EmissionMap)
				x->unbind(3);
			else if (x->type == TextureType::HeightMap)
				x->unbind(4);
		}
	}

	_rigidBody.draw();
}
void Cappuccino::GameObject::collision(float dt)
{
	if (_rigidBody._moveable)//if this object can move
		for (auto x : gameObjects) {//check the other game objects
			if (x->isActive() && x != this && x->_rigidBody._canTouch) //if the object is active, not this, and can be touched
				for (unsigned i = 0; i < 3; i++) {//all three dimensions
					glm::vec3 temp(0, 0, 0);
					temp[i] = 1;
					if (willCollide(x, temp, dt) && !x->_rigidBody._creature) {
						if (_rigidBody.bounce) {
							_rigidBody._vel[i] *= -1.0f;
							_rigidBody._accel[i] *= -1.0f;
						}
						else {
							_rigidBody._vel[i] = 0.0f;
							_rigidBody._accel[i] = 0.0f;
						}
					}

				}

		}

}
void GameObject::loadTextures()
{
	if (!_loadedTextures) {
		for (unsigned i = 0; i < _textures.size(); i++) {
			if (!_textures[i]->isLoaded()) {
				if (_textures[i]->load())
					continue;
				else
					return;
			}
		}
	}
}

void GameObject::loadMesh()
{
	if (!_loadedMesh) {
		for (unsigned i = 0; i < _meshes.size(); i++) {
			if (!_meshes[i]->loaded) {
				if (_meshes[i]->loadMesh())
					continue;
				else
					return;
			}
			else
				return;
		}
		_loadedMesh = true;
	}
}