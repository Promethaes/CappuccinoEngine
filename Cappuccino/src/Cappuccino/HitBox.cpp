#include "Cappuccino/HitBox.h"

Cappuccino::HitBox::HitBox(glm::vec3& newPos, float& newRadius)
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
		if (other._radius){//circle circle
			float distanceMin = (_radius * other._radius)* (_radius * other._radius);
			float actualDistanceX = (ourRigidLoc.x + _position.x) - (rigidLoc.x + other._position.x);
			float actualDistanceY = (ourRigidLoc.y + _position.y) - (rigidLoc.y + other._position.y);
			float actualDistanceZ = (ourRigidLoc.z + _position.z) - (rigidLoc.z + other._position.z);
			float totaldist = (actualDistanceX * actualDistanceX) + (actualDistanceY * actualDistanceY) + (actualDistanceZ * actualDistanceZ);
			if (totaldist < distanceMin)
				return true;
		}
		else{
			//circle then box
			float dist = checkCircleBox(_position,other._position,other._size);
			if (dist <= _radius * _radius)
				return true;
		}
	}
	else
	{
		if (other._radius)	{
			//box then circle
			float dist = checkCircleBox(other._position,_position,_size);
			if (dist <= (other._radius * other._radius))
				return true;
		}
		else{//box box
			if (checkSize((_position.x + ourRigidLoc.x),_size.x ,(other._position.x + rigidLoc.x),other._size.x) && checkSize((_position.y + ourRigidLoc.y),_size.y, (other._position.y + rigidLoc.y),other._size.y) && checkSize((_position.z + ourRigidLoc.z),_size.z, (other._position.z + rigidLoc.z),other._size.z))
				return true;
		}
	}
	return false;
}

float Cappuccino::HitBox::checkCircleBox(glm::vec3& circ,glm::vec3& boxPos,glm::vec3& boxSize)
{
	float dist = 0.0f;
	dist += (checkDist(circ.x,boxPos.x,boxSize.x));
	dist += (checkDist(circ.y, boxPos.y, boxSize.y));
	dist += (checkDist(circ.z, boxPos.z, boxSize.z));
	return dist;
}

bool Cappuccino::HitBox::checkSize(float box1,float length1, float box2,float length2)
{
	if (box1<box2+length2&&box2<box1+length1)
		return true;
	return false;
}

float Cappuccino::HitBox::checkDist(float circ, float boxPos, float boxSize)
{
	float dist = 0.0f;
	if (circ < boxPos) {
		dist += (boxPos - circ) * (boxPos - circ);
	}
	if (circ > boxSize) {
		dist += (circ - boxSize) * (circ - boxSize);
	}
	return dist;
}

