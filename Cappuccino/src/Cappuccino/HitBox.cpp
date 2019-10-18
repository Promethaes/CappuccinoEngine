#include "Cappuccino/HitBox.h"

Cappuccino::HitBox::HitBox(glm::vec3& newPos, float newRadius)
{
	_position = newPos;
	_radius = newRadius;
}

Cappuccino::HitBox::HitBox(glm::vec3& newPos, glm::vec3& newSize)
{
	_position = newPos;
	_size = newSize;
}

bool Cappuccino::HitBox::checkCollision(HitBox& other, glm::vec3& rigidLoc, glm::vec3& ourRigidLoc)
{
	if (_radius) {
		if (other._radius) {//circle circle
			float distanceMin = (_radius * other._radius) * (_radius * other._radius);
			float actualDistanceX = (ourRigidLoc.x + _position.x) - (rigidLoc.x + other._position.x);
			float actualDistanceY = (ourRigidLoc.y + _position.y) - (rigidLoc.y + other._position.y);
			float actualDistanceZ = (ourRigidLoc.z + _position.z) - (rigidLoc.z + other._position.z);
			float totaldist = (actualDistanceX * actualDistanceX) + (actualDistanceY * actualDistanceY) + (actualDistanceZ * actualDistanceZ);
			if (totaldist < distanceMin)
				return true;
		}
		else {
			//circle then box
			float dist = checkCircleBox(ourRigidLoc+_position,rigidLoc+other._position,other._size);
			if (dist <= _radius * _radius)
				return true;
		}
	}
	else
	{
		if (other._radius) {
			//box then circle
			float dist = checkCircleBox(rigidLoc+other._position,ourRigidLoc+_position,_size);
			if (dist <= (other._radius * other._radius))
				return true;
		}
		else {//box box
			if (checkSize((_position.x + ourRigidLoc.x), _size.x, (other._position.x + rigidLoc.x), other._size.x) && checkSize((_position.y + ourRigidLoc.y), _size.y, (other._position.y + rigidLoc.y), other._size.y) && checkSize((_position.z + ourRigidLoc.z), _size.z, (other._position.z + rigidLoc.z), other._size.z))
				return true;
		}
	}
	return false;
}

float Cappuccino::HitBox::checkCircleBox(glm::vec3& circ, glm::vec3& boxPos, glm::vec3& boxSize)
{
	float dist = 0.0f;
	dist += (checkDist(circ.x, boxPos.x, boxSize.x));
	dist += (checkDist(circ.y, boxPos.y, boxSize.y));
	dist += (checkDist(circ.z, boxPos.z, boxSize.z));
	return dist;
}

bool Cappuccino::HitBox::checkSize(float pos1, float size1, float pos2, float size2)
{
	if (pos1 - (size1 / 2) < pos2 + (size2 / 2) && pos2 - (size2 / 2) < pos1 + (size1 / 2))
		return true;
	return false;
}

float Cappuccino::HitBox::checkDist(float circ, float boxPos, float boxSize)
{
	float dist = 0.0f;
	if (circ < boxPos - (boxSize / 2)) {
		dist += (boxPos - (boxSize / 2) - circ) * (boxPos - (boxSize / 2) - circ);
	}
	if (circ > boxSize / 2) {
		dist += (circ - (boxSize / 2)) * (circ - (boxSize / 2));
	}
	return dist;
}

Cappuccino::Capsule::Capsule(glm::vec3& pos, glm::vec2& size, angle orientation)
{
	_orientation = orientation;//this is the way that the capsule will lie across
	//the size is a vec2 as the sphere must be the same radius in both dimensions, the x handles the distance between the spheres while the y handles the size of the spheres
	if (_orientation == angle::x)
	{
		hitBox[0] = HitBox(pos, glm::vec3(size, size.y));//the cube , the two sides must be the same size
		hitBox[1] = HitBox(glm::vec3(pos.x - (size.x / 2), pos.y, pos.z), size.y / 2);
		hitBox[2] = HitBox(glm::vec3(pos.x + (size.x / 2), pos.y, pos.z), size.y / 2);
	}
	else if (_orientation == angle::y)
	{
		hitBox[0] = HitBox(pos, glm::vec3(size.y, size.x, size.y));
		hitBox[1] = HitBox(glm::vec3(pos.x, pos.y - (size.x / 2), pos.z), size.y / 2);
		hitBox[2] = HitBox(glm::vec3(pos.x, pos.y + (size.x / 2), pos.z), size.y / 2);
	}
	else if (_orientation == angle::z)
	{
		hitBox[0] = HitBox(pos, glm::vec3(size.y, size.y, size.x));
		hitBox[1] = HitBox(glm::vec3(pos.x, pos.y, pos.z - (size.x / 2)), size.y / 2);
		hitBox[2] = HitBox(glm::vec3(pos.x, pos.y, pos.z - (size.x / 2)), size.y / 2);
	}
}

bool Cappuccino::Capsule::checkCollision(HitBox& other, glm::vec3& rigidLoc, glm::vec3& ourRigidLoc)
{
	for (unsigned i = 0; i < 3; i++)
	{
		if (hitBox[i].checkCollision(other, rigidLoc, ourRigidLoc))
			return true;
	}
	return false;
}