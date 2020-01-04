#include "Cappuccino/GameObject.h"
#include "Cappuccino/CappMacros.h"
using namespace Cappuccino;

std::vector<GameObject*> GameObject::gameObjects = {};

Texture* GameObject::defaultEmission = nullptr;
Texture* GameObject::defaultNormal   = nullptr;
Texture* GameObject::defaultHeight   = nullptr;

GameObject::GameObject(const Shader& _shader, const std::vector<Texture*>& textures, const std::vector<Mesh*>& _meshs, const std::optional<float>& mass,unsigned viewportNum)
	:_shader(_shader), _rigidBody(glm::vec3(_transform._translateMat[3].x, _transform._translateMat[3].y, _transform._translateMat[3].z),
		glm::vec3(0, 0, 0),
		mass.has_value() ? mass.value() : 1)
{
	_viewportNum = viewportNum;
	//mesh = new Mesh(MESH);

	//set the state manually

	this->_textures = textures;
	this->_meshes = _meshs;

	bool hasNormal = false, hasEmission = false,hasHeight = false;
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

bool GameObject::checkCollision(const HitBox& other, const glm::vec3& pos) {
	return _rigidBody.checkCollision(other, pos);
}

void GameObject::baseUpdate(float dt) {
	childUpdate(dt);

	_rigidBody.update(dt, _transform._transformMat);
	_transform._position->x = _rigidBody._position.x;
	_transform._position->y = _rigidBody._position.y;
	_transform._position->z = _rigidBody._position.z;
	_transform.update();

	collision();
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

	//bind the textures to their proper slots
	for (auto x : _textures) {
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


	for (auto x : _meshes)
		x->draw();


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
void Cappuccino::GameObject::collision()
{
	collisionData newData;//what hitboxes are colliding
	if (_rigidBody._moveable)//if this object can move
		for (auto x : gameObjects){//check the other game objects
			if (x->isActive() && checkCollision(x) && x != this&&x->_rigidBody._canTouch) {//if the object is active and we are colliding
				newData = _rigidBody.getData(x->_rigidBody);//get the hitboxes
				//glm::vec3 vectorData = glm::vec3((_rigidBody._position+newData.one._position)-(x->_rigidBody._position+newData.two._position));
				//glm::vec3 normalizedData = glm::normalize(vectorData);//old code but may be useful if system is changed
				/*HitBox ourMiniBoxes[6];//will not work but not deleted just yet
				HitBox otherMiniBoxes[6];
				for (unsigned i = 0; i < 6; i++) {//create the miniBoxes for the colliding hitboxes
					int mult = 1;//this is the multiplier for which side the mini box will be on one axis
					for (unsigned n = 0; n < i; n++)//for the current iteration we invert the multiplier //does it need to run once more
						mult *= -1;

					glm::vec3 ourTempSize = newData.one._size;
					ourTempSize[(i) / 2] /= 2;

					glm::vec3 otherTempSize = newData.two._size;
					otherTempSize[(i) / 2] /= 2;

					glm::vec3 ourTempPos = newData.one._position;//create temp position data for each minibox
					ourTempPos[(i) / 2] += (ourTempSize[(i) / 2] * mult);

					glm::vec3 otherTempPos = newData.two._position;
					otherTempPos[(i) / 2] += (otherTempSize[(i) / 2] * mult);

					ourMiniBoxes[i] = HitBox(ourTempPos,ourTempSize);
					otherMiniBoxes[i] = HitBox(otherTempPos,otherTempSize);
				}
				bool ourDirection[6];
				bool otherDirection[6];
				for (unsigned i = 0; i < 6; i++) {
					ourDirection[i] = x->_rigidBody.checkCollision(ourMiniBoxes[i], _rigidBody._position);
				}
				for (unsigned i = 0; i < 6; i++) {
					otherDirection[i] = _rigidBody.checkCollision(otherMiniBoxes[i], x->_rigidBody._position);
				}*/

				std::vector<glm::vec3> directions
				{
					glm::vec3( 0.0f, 0.0f,  1.0f),
					glm::vec3( 0.0f, 0.0f, -1.0f),
					glm::vec3(-1.0f, 0.0f,  0.0f),
					glm::vec3( 1.0f, 0.0f,  0.0f)
				};
				enum Direction : unsigned int
				{
					forward = 0,
					backward,
					left,
					right
				};
				for (unsigned i = 0; i < newData.collisions; i++)
				{

				
					unsigned bestmatch = 0;
					glm::vec3 displacement((_rigidBody._position + newData.ourBox._position) - (x->_rigidBody._position + newData.otherBox[i]._position));
					//glm::vec3 displacement = _rigidBody._position - x->_rigidBody._position;
					float max = 0.0f;
					for (unsigned i = 0; i < 4; i++) {
						const float dotProduct = glm::dot(glm::normalize(displacement), directions[i]);
						if (dotProduct > max)
						{
							//std::cout << max << std::endl;
							max = dotProduct;
							bestmatch = i;
						}
					}
					const Direction dir = static_cast<Direction>(bestmatch);
					if (dir == left) {
						//x-
						_rigidBody._vel.x = 0.0f;
						_rigidBody._position.x = x->_rigidBody._position.x + newData.otherBox[i]._position.x - newData.otherBox[i]._size.x / 2  - newData.ourBox._size.x / 2 + newData.ourBox._position.x-0.01;
					}
					else if (dir == right) {
						//x+
						_rigidBody._vel.x = 0.0f;
						_rigidBody._position.x = x->_rigidBody._position.x + newData.otherBox[i]._position.x + newData.otherBox[i]._size.x / 2 + newData.ourBox._size.x / 2 + newData.ourBox._position.x + 0.01;
					}
					else if (dir == forward) {
						//z+
						_rigidBody._vel.z = 0.0f;
						_rigidBody._position.z = x->_rigidBody._position.z + newData.otherBox[i]._position.z + newData.otherBox[i]._size.z / 2 + newData.ourBox._size.z / 2 + newData.ourBox._position.z + 0.01;
					}
					else if (dir == backward) {
						//z-
						_rigidBody._vel.z = 0.0f;
						_rigidBody._position.z = x->_rigidBody._position.z + newData.otherBox[i]._position.z - newData.otherBox[i]._size.z / 2 - newData.ourBox._size.z / 2 + newData.ourBox._position.z - 0.01;
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