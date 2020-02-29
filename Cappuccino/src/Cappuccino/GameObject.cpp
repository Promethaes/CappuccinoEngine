#include "Cappuccino/GameObject.h"
#include "Cappuccino/CappMacros.h"

using namespace Cappuccino;

std::vector<GameObject*> GameObject::gameObjects = {};

Texture* GameObject::defaultEmission = nullptr;
Texture* GameObject::defaultNormal = nullptr;

GameObject::GameObject(const Shader& _shader, const std::vector<Texture*>& textures, const std::vector<Mesh*>& _meshs, const std::optional<float>& mass, const unsigned viewportNum) :
	_rigidBody(glm::vec3(_transform._translateMat[3].x, _transform._translateMat[3].y, _transform._translateMat[3].z), mass.has_value() ? mass.value() : 1), _shader(_shader) {
	_viewportNum = viewportNum;
	
	this->_textures = textures;
	this->_meshes = _meshs;

	bool hasNormal = false, hasEmission = false, hasHeight = false;
	for (unsigned i = 0; i < this->_textures.size(); i++) {
		if (_textures[i]->type == TextureType::PBREmission)
			hasEmission = true;
		else if (_textures[i]->type == TextureType::PBRNormal)
			hasNormal = true;
	}

	static bool initDefaultMaps = false;

	if (!initDefaultMaps) {
		defaultEmission = new Texture("DefaultEmission", "defaultEmission.png", TextureType::PBREmission);
		defaultNormal = new Texture("DefaultNormal", "defaultNorm.png", TextureType::PBRNormal);

		//after all maps are init one time, never do it again
		initDefaultMaps = true;
	}

	if (!hasEmission)
		_textures.push_back(defaultEmission);
	if (!hasNormal)
		_textures.push_back(defaultNormal);
	

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


bool GameObject::checkCollision(GameObject* other) { return _rigidBody.checkCollision(other->_rigidBody); }
bool GameObject::willCollide(GameObject* other, const glm::vec3& direction, const float dt) { return _rigidBody.willCollide(other->_rigidBody, direction, dt); }
bool GameObject::checkCollision(const HitBox& other, const glm::vec3& pos) { return _rigidBody.checkCollision(other, pos); }
bool GameObject::willCollide(const HitBox& other, const glm::vec3& direction, const glm::vec3& pos, const float dt) { return _rigidBody.willCollide(other, pos, direction, dt); }

bool Cappuccino::GameObject::intersecting(const Ray& ray)
{
	return _rigidBody.intersecting(ray);
}

void GameObject::baseUpdate(float dt) {
	childUpdate(dt);
	collision(dt);
	_rigidBody.update(dt);

	_transform._position->x = _rigidBody._position.x;
	_transform._position->y = _rigidBody._position.y;
	_transform._position->z = _rigidBody._position.z;
	_transform.update();
	_animator.update(dt);
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
			else if (x->type == TextureType::PBRAlbedo)
				x->bind(5);
			else if (x->type == TextureType::PBRNormal)
				x->bind(6);
			else if (x->type == TextureType::PBRMetallic)
				x->bind(7);
			else if (x->type == TextureType::PBRRoughness)
				x->bind(8);
			else if (x->type == TextureType::PBRAmbientOcc)
				x->bind(9);
			else if (x->type == TextureType::PBREmission)
				x->bind(10);
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
			else if (x->type == TextureType::PBRAlbedo)
				x->unbind(5);
			else if (x->type == TextureType::PBRNormal)
				x->unbind(6);
			else if (x->type == TextureType::PBRMetallic)
				x->unbind(7);
			else if (x->type == TextureType::PBRRoughness)
				x->unbind(8);
			else if (x->type == TextureType::PBRAmbientOcc)
				x->unbind(9);
			else if (x->type == TextureType::PBREmission)
				x->unbind(10);
		}
	}

	_rigidBody.draw();
}
void Cappuccino::GameObject::gBufferDraw(Shader* gBufferShader)
{
	gBufferShader->use();
	for (unsigned i = 0; i < _meshes.size(); i++) {

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
			else if (x->type == TextureType::PBRAlbedo)
				x->bind(5);
			else if (x->type == TextureType::PBRNormal)
				x->bind(6);
			else if (x->type == TextureType::PBRMetallic)
				x->bind(7);
			else if (x->type == TextureType::PBRRoughness)
				x->bind(8);
			else if (x->type == TextureType::PBRAmbientOcc)
				x->bind(9);
			else if (x->type == TextureType::PBREmission)
				x->bind(10);
		}

		_transform._transformMat = gBufferShader->loadModelMatrix(_transform._transformMat);

		_meshes[i]->draw();
	
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
			else if (x->type == TextureType::PBRAlbedo)
				x->unbind(5);
			else if (x->type == TextureType::PBRNormal)
				x->unbind(6);
			else if (x->type == TextureType::PBRMetallic)
				x->unbind(7);
			else if (x->type == TextureType::PBRRoughness)
				x->unbind(8);
			else if (x->type == TextureType::PBRAmbientOcc)
				x->unbind(9);
			else if (x->type == TextureType::PBREmission)
				x->unbind(10);
		}
	}
}
void GameObject::collision(const float dt) {
	if (_rigidBody._moveable) {
		//if this object can move
		for (auto x : gameObjects) {//check the other game objects
			if (x->isActive() && x != this) {//if the object is active and not this
				if (x->_rigidBody._canTouch && _rigidBody._canTouch) { //if the object can be touched
					if (_rigidBody.myType == "" || _rigidBody.myType != x->_rigidBody.myType) {
						//if default type or not the same type as other object
						for (unsigned i = 0; i < 3; i++) {//all three dimensions
							glm::vec3 temp(0, 0, 0);
							temp[i] = 1;
							if (willCollide(x, temp, dt)) {
								if (_rigidBody._bounce) {
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
				else if (_rigidBody._projectile) {
					if (!x->_rigidBody._projectile&&!x->_rigidBody._creature) {
						if (checkCollision(x)) {
							_rigidBody._hitWall = true;
						}
					}
				}
			}
		}
	}

}
void GameObject::loadTextures() {
	if (!_loadedTextures) {
		for (unsigned i = 0; i < _textures.size(); i++) {
			if (!_textures[i]->isLoaded()) {
				if (_textures[i]->load())
					continue;
				return;
			}
		}
	}
}

void GameObject::loadMesh() {
	if (!_loadedMesh) {
		for (unsigned i = 0; i < _meshes.size(); i++) {
			if (!_meshes[i]->loaded) {
				if (_meshes[i]->loadMesh())
					continue;
				return;
			}
			
			return;
		}
		_loadedMesh = true;
	}
}