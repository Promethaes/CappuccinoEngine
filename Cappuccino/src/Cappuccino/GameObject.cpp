#include "Cappuccino/GameObject.h"
#include "Cappuccino/CappMacros.h"
#include "Cappuccino/ResourceManager.h"

using namespace Cappuccino;

std::vector<GameObject*> GameObject::gameObjects;

Texture* GameObject::defaultEmission = nullptr;
Texture* GameObject::defaultNormal = nullptr;
Texture* GameObject::defaultHeight = nullptr;

GameObject::GameObject(const Shader& shader, const std::vector<TextureProperties>& textures, const std::vector<MeshProperties>& meshes, const std::optional<float>& mass, const unsigned viewportNum) :
	_rigidBody(glm::vec3(_transform._translateMat[3].x, _transform._translateMat[3].y, _transform._translateMat[3].z), mass.has_value() ? mass.value() : 1),
	_viewportNum(viewportNum), _textures(textures), _meshes(meshes), _shader(shader)
{
	bool hasNormal = false, hasEmission = false, hasHeight = false;
	for(const auto& texture : _textures) {
		if (texture.type == TextureType::EmissionMap)
			hasEmission = true;
		else if (texture.type == TextureType::NormalMap)
			hasNormal = true;
		else if (texture.type == TextureType::HeightMap)
			hasHeight = true;
	}

	static bool initDefaultMaps = false;

	if (!initDefaultMaps) {
		defaultEmission = TextureLibrary::loadTexture("DefaultEmission", "defaultEmission.png", TextureType::EmissionMap);
		defaultNormal = TextureLibrary::loadTexture("DefaultNormal", "defaultNorm.png", TextureType::NormalMap);
		defaultHeight = TextureLibrary::loadTexture("DefaultHeight", "defaultHeight.png", TextureType::HeightMap);

		//after all maps are init one time, never do it again
		initDefaultMaps = true;
	}

	if (!hasEmission)
		_textures.push_back(defaultEmission->getProperties());
	if (!hasNormal)
		_textures.push_back(defaultNormal->getProperties());
	if (!hasHeight)
		_textures.push_back(defaultHeight->getProperties());

	//loadTextures();
	//loadMesh();

	gameObjects.push_back(this);
}

GameObject::~GameObject() {
	_meshes.clear();
	_textures.clear();
}


bool GameObject::checkCollision(GameObject* other) { return _rigidBody.checkCollision(other->_rigidBody); }
bool GameObject::willCollide(GameObject* other, const glm::vec3& direction, const float dt) { return _rigidBody.willCollide(other->_rigidBody, direction, dt); }
bool GameObject::checkCollision(const HitBox& other, const glm::vec3& pos) { return _rigidBody.checkCollision(other, pos); }
bool GameObject::willCollide(const HitBox& other, const glm::vec3& direction, const glm::vec3& pos, const float dt) { return _rigidBody.willCollide(other, pos, direction, dt); }

bool GameObject::intersecting(const Ray& ray)
{
	return _rigidBody.intersecting(ray);
}

GameObject* Cappuccino::GameObject::getFirstIntersect(const Ray& ray)
{
	std::vector <GameObject*> touched;
	std::vector <glm::vec3> locations;
	std::vector <float> distances;
	int correctBoi = 0;
	for (auto x : gameObjects)
		if (x->intersecting(ray) && x->isActive()&&x!=this) {
			touched.push_back(x);
			locations.push_back(x->_rigidBody.getFirstInteresect(ray));
		}
	for (auto x : locations) 
		distances.push_back(x.length());

	float min = distances[0];
	for (unsigned i = 0; i < touched.size(); i++) {
		if (distances[i] < min) {
			min = distances[i];
			correctBoi = i;
		}
	}
			

	return touched[correctBoi];
}

bool GameObject::intersecting(const Ray& ray) {
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
}

void GameObject::draw()
{
	//set active shader
	glUseProgram(_shader.getID());

	for(unsigned i = 0; i < _meshes.size(); i++) {

		//bind the textures to their proper slots
		for (const auto& t : _textures) {
			const auto texture = TextureLibrary::getTexture(t.name);
			
			if (t.textureIndex != i)
				continue;

			if (t.type == TextureType::DiffuseMap)
				texture->bind(0);
			else if (t.type == TextureType::SpecularMap)
				texture->bind(1);
			else if (t.type == TextureType::NormalMap)
				texture->bind(2);
			else if (t.type == TextureType::EmissionMap)
				texture->bind(3);
			else if (t.type == TextureType::HeightMap)
				texture->bind(4);
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
		}

		_transform._transformMat = _shader.loadModelMatrix(_transform._transformMat);

		auto mesh = MeshLibrary::getMesh(_meshes[i].name);
		mesh->draw();
		
		//unloads the textures
		const auto texture = TextureLibrary::getTexture(defaultEmission->getName());
		for(unsigned j = 0; j < 24; ++j) {
			texture->unbind(j);
		}
	}

	_rigidBody.draw();
}
void GameObject::collision(const float dt) {
	if (_rigidBody._moveable) {
		//if this object can move
		for (auto x : gameObjects) {//check the other game objects
			if (x->isActive() && x != this) {//if the object is active and not this
				if (x->_rigidBody._canTouch && _rigidBody._canTouch) { //if the object can be touched
					if (_rigidBody.myType.empty() || _rigidBody.myType != x->_rigidBody.myType) {
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
//void GameObject::loadTextures() {
//	if (!_loadedTextures) {
//		for (unsigned i = 0; i < _textures.size(); i++) {
//			if (!_textures[i]->isLoaded()) {
//				if (_textures[i]->load())
//					continue;
//				return;
//			}
//		}
//	}
//}
//
//void GameObject::loadMesh() {
//	if (!_loadedMesh) {
//		for (unsigned i = 0; i < _meshes.size(); i++) {
//			if (!_meshes[i]->loaded) {
//				if (_meshes[i]->loadMesh())
//					continue;
//				return;
//			}
//			
//			return;
//		}
//		_loadedMesh = true;
//	}
//}