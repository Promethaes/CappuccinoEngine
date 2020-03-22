#include "Cappuccino/HitBox.h"
#include "Cappuccino/CappMacros.h"

#include <glad/glad.h>

#include <vector>

using namespace Cappuccino;

HitBox::HitBox(glm::vec3& newPos, float newRadius)
{
	_position = newPos;
	_radius = newRadius;
}

HitBox::HitBox(glm::vec3& newPos, glm::vec3& newSize)
{
	_position = newPos;
	_size = newSize ;
}

Cappuccino::HitBox::HitBox(glm::vec3& newPos, glm::vec3& newSize, glm::mat4& newRotation)
{
	_OBB = true;
	_position = newPos;
	_size = newSize;
	_rotationMatrix = newRotation;
}

bool HitBox::checkCollision(HitBox& other, glm::vec3& rigidLoc, glm::vec3& ourRigidLoc)
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
		else if (other._OBB)//circle OBB
		{
			//TODO
		}
		else {//circle ABB
			float dist = checkCircleBox(ourRigidLoc + _position, rigidLoc + other._position, other._size);
			if (dist <= _radius * _radius)
				return true;
		}
	}
	else if (_OBB) {
		if (_radius) {// OBB sphere
			//TODO
		}
		else if (other._OBB) {//OBB OBB
			glm::vec3 dist = (ourRigidLoc + _position) - (rigidLoc + other._position);
			return !(
				checkPlane(dist, _rotationMatrix[0], other) ||
				checkPlane(dist, _rotationMatrix[1], other) ||
				checkPlane(dist, _rotationMatrix[2], other) ||
				checkPlane(dist, other._rotationMatrix[0], other) ||
				checkPlane(dist, other._rotationMatrix[1], other) ||
				checkPlane(dist, other._rotationMatrix[2], other) ||
				checkPlane(dist, glm::cross(glm::vec3(_rotationMatrix[0][0], _rotationMatrix[0][1], _rotationMatrix[0][2]), glm::vec3(other._rotationMatrix[0][0], other._rotationMatrix[0][1], other._rotationMatrix[0][2])), other) ||
				checkPlane(dist, glm::cross(glm::vec3(_rotationMatrix[0][0], _rotationMatrix[0][1], _rotationMatrix[0][2]), glm::vec3(other._rotationMatrix[1][0], other._rotationMatrix[1][1], other._rotationMatrix[1][2])), other) ||
				checkPlane(dist, glm::cross(glm::vec3(_rotationMatrix[0][0], _rotationMatrix[0][1], _rotationMatrix[0][2]), glm::vec3(other._rotationMatrix[2][0], other._rotationMatrix[2][1], other._rotationMatrix[2][2])), other) ||
				checkPlane(dist, glm::cross(glm::vec3(_rotationMatrix[1][0], _rotationMatrix[1][1], _rotationMatrix[1][2]), glm::vec3(other._rotationMatrix[0][0], other._rotationMatrix[0][1], other._rotationMatrix[0][2])), other) ||
				checkPlane(dist, glm::cross(glm::vec3(_rotationMatrix[1][0], _rotationMatrix[1][1], _rotationMatrix[1][2]), glm::vec3(other._rotationMatrix[1][0], other._rotationMatrix[1][1], other._rotationMatrix[1][2])), other) ||
				checkPlane(dist, glm::cross(glm::vec3(_rotationMatrix[1][0], _rotationMatrix[1][1], _rotationMatrix[1][2]), glm::vec3(other._rotationMatrix[2][0], other._rotationMatrix[2][1], other._rotationMatrix[2][2])), other) ||
				checkPlane(dist, glm::cross(glm::vec3(_rotationMatrix[2][0], _rotationMatrix[2][1], _rotationMatrix[2][2]), glm::vec3(other._rotationMatrix[0][0], other._rotationMatrix[0][1], other._rotationMatrix[0][2])), other) ||
				checkPlane(dist, glm::cross(glm::vec3(_rotationMatrix[2][0], _rotationMatrix[2][1], _rotationMatrix[2][2]), glm::vec3(other._rotationMatrix[1][0], other._rotationMatrix[1][1], other._rotationMatrix[1][2])), other) ||
				checkPlane(dist, glm::cross(glm::vec3(_rotationMatrix[2][0], _rotationMatrix[2][1], _rotationMatrix[2][2]), glm::vec3(other._rotationMatrix[2][0], other._rotationMatrix[2][1], other._rotationMatrix[2][2])), other));
		}
		else//OBB AABB
		{
			glm::vec3 dist = (ourRigidLoc + _position) - (rigidLoc + other._position);
			return !(
				checkPlane(dist, _rotationMatrix[0], other) ||
				checkPlane(dist, _rotationMatrix[1], other) ||
				checkPlane(dist, _rotationMatrix[2], other) ||
				checkPlane(dist, other._rotationMatrix[0], other) ||
				checkPlane(dist, other._rotationMatrix[1], other) ||
				checkPlane(dist, other._rotationMatrix[2], other) ||
				checkPlane(dist, glm::cross(glm::vec3(_rotationMatrix[0][0], _rotationMatrix[0][1], _rotationMatrix[0][2]), glm::vec3(other._rotationMatrix[0][0], other._rotationMatrix[0][1], other._rotationMatrix[0][2])), other) ||
				checkPlane(dist, glm::cross(glm::vec3(_rotationMatrix[0][0], _rotationMatrix[0][1], _rotationMatrix[0][2]), glm::vec3(other._rotationMatrix[1][0], other._rotationMatrix[1][1], other._rotationMatrix[1][2])), other) ||
				checkPlane(dist, glm::cross(glm::vec3(_rotationMatrix[0][0], _rotationMatrix[0][1], _rotationMatrix[0][2]), glm::vec3(other._rotationMatrix[2][0], other._rotationMatrix[2][1], other._rotationMatrix[2][2])), other) ||
				checkPlane(dist, glm::cross(glm::vec3(_rotationMatrix[1][0], _rotationMatrix[1][1], _rotationMatrix[1][2]), glm::vec3(other._rotationMatrix[0][0], other._rotationMatrix[0][1], other._rotationMatrix[0][2])), other) ||
				checkPlane(dist, glm::cross(glm::vec3(_rotationMatrix[1][0], _rotationMatrix[1][1], _rotationMatrix[1][2]), glm::vec3(other._rotationMatrix[1][0], other._rotationMatrix[1][1], other._rotationMatrix[1][2])), other) ||
				checkPlane(dist, glm::cross(glm::vec3(_rotationMatrix[1][0], _rotationMatrix[1][1], _rotationMatrix[1][2]), glm::vec3(other._rotationMatrix[2][0], other._rotationMatrix[2][1], other._rotationMatrix[2][2])), other) ||
				checkPlane(dist, glm::cross(glm::vec3(_rotationMatrix[2][0], _rotationMatrix[2][1], _rotationMatrix[2][2]), glm::vec3(other._rotationMatrix[0][0], other._rotationMatrix[0][1], other._rotationMatrix[0][2])), other) ||
				checkPlane(dist, glm::cross(glm::vec3(_rotationMatrix[2][0], _rotationMatrix[2][1], _rotationMatrix[2][2]), glm::vec3(other._rotationMatrix[1][0], other._rotationMatrix[1][1], other._rotationMatrix[1][2])), other) ||
				checkPlane(dist, glm::cross(glm::vec3(_rotationMatrix[2][0], _rotationMatrix[2][1], _rotationMatrix[2][2]), glm::vec3(other._rotationMatrix[2][0], other._rotationMatrix[2][1], other._rotationMatrix[2][2])), other));
		}
	}
	else{
		if (other._radius) {//AABB circle
			float dist = checkCircleBox(rigidLoc + other._position, ourRigidLoc + _position, _size);
			if (dist <= (other._radius * other._radius))
				return true;
		}
		else if (other._OBB){//AABB OBB
			glm::vec3 dist = (ourRigidLoc + _position) - (rigidLoc + other._position);
			return !(
				checkPlane(dist, _rotationMatrix[0], other) ||
				checkPlane(dist, _rotationMatrix[1], other) ||
				checkPlane(dist, _rotationMatrix[2], other) ||
				checkPlane(dist, other._rotationMatrix[0], other) ||
				checkPlane(dist, other._rotationMatrix[1], other) ||
				checkPlane(dist, other._rotationMatrix[2], other) ||
				checkPlane(dist, glm::cross(glm::vec3( _rotationMatrix[0][0], _rotationMatrix[0][1], _rotationMatrix[0][2]), glm::vec3(other._rotationMatrix[0][0], other._rotationMatrix[0][1], other._rotationMatrix[0][2])), other) ||
				checkPlane(dist, glm::cross(glm::vec3( _rotationMatrix[0][0], _rotationMatrix[0][1], _rotationMatrix[0][2]), glm::vec3(other._rotationMatrix[1][0], other._rotationMatrix[1][1], other._rotationMatrix[1][2])), other) ||
				checkPlane(dist, glm::cross(glm::vec3( _rotationMatrix[0][0], _rotationMatrix[0][1], _rotationMatrix[0][2]), glm::vec3(other._rotationMatrix[2][0], other._rotationMatrix[2][1], other._rotationMatrix[2][2])), other) ||
				checkPlane(dist, glm::cross(glm::vec3( _rotationMatrix[1][0], _rotationMatrix[1][1], _rotationMatrix[1][2]), glm::vec3(other._rotationMatrix[0][0], other._rotationMatrix[0][1], other._rotationMatrix[0][2])), other) ||
				checkPlane(dist, glm::cross(glm::vec3( _rotationMatrix[1][0], _rotationMatrix[1][1], _rotationMatrix[1][2]), glm::vec3(other._rotationMatrix[1][0], other._rotationMatrix[1][1], other._rotationMatrix[1][2])), other) ||
				checkPlane(dist, glm::cross(glm::vec3( _rotationMatrix[1][0], _rotationMatrix[1][1], _rotationMatrix[1][2]), glm::vec3(other._rotationMatrix[2][0], other._rotationMatrix[2][1], other._rotationMatrix[2][2])), other) ||
				checkPlane(dist, glm::cross(glm::vec3( _rotationMatrix[2][0], _rotationMatrix[2][1], _rotationMatrix[2][2]), glm::vec3(other._rotationMatrix[0][0], other._rotationMatrix[0][1], other._rotationMatrix[0][2])), other) ||
				checkPlane(dist, glm::cross(glm::vec3( _rotationMatrix[2][0], _rotationMatrix[2][1], _rotationMatrix[2][2]), glm::vec3(other._rotationMatrix[1][0], other._rotationMatrix[1][1], other._rotationMatrix[1][2])), other) ||
				checkPlane(dist, glm::cross(glm::vec3( _rotationMatrix[2][0], _rotationMatrix[2][1], _rotationMatrix[2][2]), glm::vec3(other._rotationMatrix[2][0], other._rotationMatrix[2][1], other._rotationMatrix[2][2])), other));
		}
		else {//AABB AABB
			if (checkSize((_position.x + ourRigidLoc.x), _size.x, (other._position.x + rigidLoc.x), other._size.x) && checkSize((_position.y + ourRigidLoc.y), _size.y, (other._position.y + rigidLoc.y), other._size.y) && checkSize((_position.z + ourRigidLoc.z), _size.z, (other._position.z + rigidLoc.z), other._size.z))
				return true;
		}
	}
	return false;
}

bool HitBox::intersecting(const Ray& ray, glm::vec3& pos)
{
https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-box-intersection
	if (!_radius&&!_OBB)//ray aabb
	{

		glm::vec3 dimensionBox(0.0f);//0=neutral,-1 negative, 1 positive
		glm::vec3 dimensionRay(0.0f);

		glm::vec3 raySpaceBoxPos = pos + _position - ray._rayPos;//raypos is now 0,0,0
		glm::vec3 boxMin(raySpaceBoxPos - (_size / 2.0f));//get the new box position max and min
		glm::vec3 boxMax(raySpaceBoxPos + (_size / 2.0f));


		if (boxMin.x > 0.0f && boxMax.x > 0.0f)
			dimensionBox.x = 1;
		else if (boxMin.x < 0.0f && boxMax.x < 0.0f)
			dimensionBox.x = -1;

		if (boxMin.y > 0.0f && boxMax.y > 0.0f)
			dimensionBox.y = 1;
		else if (boxMin.y < 0.0f && boxMax.y < 0.0f)
			dimensionBox.y = -1;

		if (boxMin.z > 0.0f && boxMax.z > 0.0f)
			dimensionBox.z = 1;
		else if (boxMin.z < 0.0f && boxMax.z < 0.0f)
			dimensionBox.z = -1;


		if (ray._rayDir.x > 0.0f)
			dimensionRay.x = 1;
		else if (ray._rayDir.x < 0.0f)
			dimensionRay.x = -1;

		if (ray._rayDir.y > 0.0f)
			dimensionRay.y = 1;
		else if (ray._rayDir.y < 0.0f)
			dimensionRay.y = -1;

		if (ray._rayDir.z > 0.0f)
			dimensionRay.z = 1;
		else if (ray._rayDir.z < 0.0f)
			dimensionRay.z = -1;

		for (unsigned i = 0; i < 3; i++)
			if (dimensionBox[i] != 0 && dimensionBox[i] != dimensionRay[i])
				return false;

		float txMin, txMax, tyMin, tyMax, tzMin, tzMax;

		auto inverseDir = 1.0f / (ray._rayDir);

		glm::vec3 bounds[2]{ pos + _position - (_size / 2.0f), pos + _position + (_size / 2.0f) };
		std::vector<int> sign;

		sign.push_back((inverseDir.x < 0));
		sign.push_back((inverseDir.y < 0));
		sign.push_back((inverseDir.z < 0));

		txMin = (bounds[sign[0]].x - ray._rayPos.x) * inverseDir.x;
		txMax = (bounds[1 - sign[0]].x - ray._rayPos.x) * inverseDir.x;

		tyMin = (bounds[sign[1]].y - ray._rayPos.y) * inverseDir.y;
		tyMax = (bounds[1 - sign[1]].y - ray._rayPos.y) * inverseDir.y;

		tzMin = (bounds[sign[2]].z - ray._rayPos.z) * inverseDir.z;
		tzMax = (bounds[1 - sign[2]].z - ray._rayPos.z) * inverseDir.z;

		if ((txMin > tyMax) || (tyMin > txMax))
			return false;
		if (tyMin > txMin)
			txMin = tyMin;
		if (tyMax < txMax)
			txMax = tyMax;
		if ((txMin > tzMax) || (tzMin > txMax))
			return false;

		return true;
	}
	else if (_OBB)//RAY / OBB
	{
		//TODO
		return false;
	}
	else//RAY / SPHERE
	{
		//TODO 
		return false;
	}
}

glm::vec3 Cappuccino::HitBox::getIntersectPoint(const Ray& ray, glm::vec3& position)
{
	glm::vec3 point(0.0f);//return data
	bool didCollide = false;
	glm::vec3 dimensionBox(0.0f);//0=neutral,-1 negative, 1 positive
	glm::vec3 dimensionRay(0.0f);

	if (!_OBB && !_radius) {//if aabb
		glm::vec3 raySpaceBoxPos = position + _position - ray._rayPos;//raypos is now 0,0,0
		glm::vec3 boxMin(raySpaceBoxPos - (_size / 2.0f));//get the new box position max and min
		glm::vec3 boxMax(raySpaceBoxPos + (_size / 2.0f));

		if (boxMin.x > 0.0f && boxMax.x > 0.0f)
			dimensionBox.x = 1;
		else if(boxMin.x < 0.0f && boxMax.x < 0.0f)
			dimensionBox.x = -1;

		if (boxMin.y > 0.0f && boxMax.y > 0.0f)
			dimensionBox.y = 1;
		else if (boxMin.y < 0.0f && boxMax.y < 0.0f)
			dimensionBox.y = -1;

		if (boxMin.z > 0.0f && boxMax.z > 0.0f)
			dimensionBox.z = 1;
		else if (boxMin.z < 0.0f && boxMax.z < 0.0f)
			dimensionBox.z = -1;


		if (ray._rayDir.x > 0.0f)
			dimensionRay.x = 1;
		else if (ray._rayDir.x < 0.0f)
			dimensionRay.x = -1;

		if (ray._rayDir.y > 0.0f)
			dimensionRay.y = 1;
		else if (ray._rayDir.y < 0.0f)
			dimensionRay.y = -1;

		if (ray._rayDir.z > 0.0f)
			dimensionRay.z = 1;
		else if (ray._rayDir.z < 0.0f)
			dimensionRay.z = -1;

		for (unsigned i = 0; i < 3; i++)
			if (dimensionBox[i] != 0 && dimensionBox[i] != dimensionRay[i])
				return point;



		//x
		float xMultLow = boxMin.x / ray._rayDir.x;//get the multiplier for the other 2 dimensions
		float xMultHigh = boxMax.x / ray._rayDir.x;//max and min
		{
			glm::vec3 rayLowPosX = ray._rayDir * xMultLow;//create where the ray would strike the box or not at the minimum of the box in that dimension
			if (rayLowPosX.y<boxMax.y && rayLowPosX.y>boxMin.y&& rayLowPosX.z<boxMax.z && rayLowPosX.z>boxMin.z)//if the striking position is in between the max and min in all dimensions 
				if ((point == glm::vec3(0.0f) || glm::length(point) > glm::length(rayLowPosX))&& glm::length(rayLowPosX)!=0.0f)//if the length of the vector is smaller than the record
				{
					point = rayLowPosX;//replace the record
					didCollide = true;
				}
		}
		
		{
			glm::vec3 rayHighPosX = ray._rayDir * xMultHigh;
			if (rayHighPosX.y<=boxMax.y && rayHighPosX.y>=boxMin.y&& rayHighPosX.z<=boxMax.z && rayHighPosX.z>=boxMin.z)
				if ((point == glm::vec3(0.0f) || glm::length(point) > glm::length(rayHighPosX))&& glm::length(rayHighPosX)!=0.0f)
				{
					point = rayHighPosX;
					didCollide = true;
				}
		}
		

		//y

		float yMultLow = boxMin.y / ray._rayDir.y;
		float yMultHigh = boxMax.y / ray._rayDir.y;

		{
			glm::vec3 rayLowPosY = ray._rayDir * yMultLow;
			if (rayLowPosY.x<=boxMax.x && rayLowPosY.x>=boxMin.x&& rayLowPosY.z<=boxMax.z && rayLowPosY.z>=boxMin.z)
				if ((point == glm::vec3(0.0f) || glm::length(point) > glm::length(rayLowPosY))&& glm::length(rayLowPosY)!=0.0f) {
					point = rayLowPosY;
					didCollide = true;
				}
					
		}
		{
			glm::vec3 rayHighPosY = ray._rayDir * yMultHigh;
			if (rayHighPosY.x<=boxMax.x && rayHighPosY.x>=boxMin.x&& rayHighPosY.z<=boxMax.z && rayHighPosY.z>=boxMin.z)
				if ((point == glm::vec3(0.0f) || glm::length(point) > glm::length(rayHighPosY))&& glm::length(rayHighPosY)!=0.0f) {
					point = rayHighPosY;
					didCollide = true;
				}
					
		}
		
		

		//z

		float zMultLow = boxMin.z / ray._rayDir.z;
		float zMultHigh = boxMax.z / ray._rayDir.z;

		{
			glm::vec3 rayLowPosZ = ray._rayDir * zMultLow;
			if (rayLowPosZ.y<=boxMax.y && rayLowPosZ.y>=boxMin.y&& rayLowPosZ.x<=boxMax.x && rayLowPosZ.x>=boxMin.x)
				if ((point == glm::vec3(0.0f) || glm::length(point) > glm::length(rayLowPosZ))&& glm::length(rayLowPosZ)!=0.0f) {
					point = rayLowPosZ;
					didCollide = true;
				}
					
		}
		{
			glm::vec3 rayHighPosZ = ray._rayDir * zMultHigh;
			if (rayHighPosZ.x<=boxMax.x && rayHighPosZ.x>=boxMin.x&& rayHighPosZ.y<=boxMax.y && rayHighPosZ.y>=boxMin.y)
				if ((point == glm::vec3(0.0f) || glm::length(point) > glm::length(rayHighPosZ))&& glm::length(rayHighPosZ)!=0.0f) {
					point = rayHighPosZ;
					didCollide = true;
				}
					
		}

	}

	//if (point == glm::vec3(0.0f)) {
	//	std::cout << "THIS SHOULD NOT HAPPEN! Tell EVYN ray Intersect hit an edge case!\n";
	//}
	//if (!didCollide)
	//	std::cout << "Fuck\n";
	return point;
}

void Cappuccino::HitBox::rotateBox(float angle)
{
	if (!_OBB) {
		if (angle / 90.0f == 1.0f)
		{
			_size = glm::vec3(_size.z, _size.y, _size.x);
			_position = glm::vec3(_position.z, _position.y, -_position.x);
		}
		else if (angle / 90.0f == 2.0f)
		{
			_position.x *= -1;
			_position.z *= -1;
		}
		else if (angle / 90.0f == 3.0f)
		{
			_size = glm::vec3(_size.z, _size.y, _size.x);
			_position = glm::vec3(-_position.z, _position.y, _position.x);
		}
	}
	else
	{
		if (angle / 90.0f == 1.0f)
		{
			_position = glm::vec3(_position.z, _position.y, -_position.x);
		}
		else if (angle / 90.0f == 2.0f)
		{
			_position.x *= -1;
			_position.z *= -1;
		}
		else if (angle / 90.0f == 3.0f)
		{
			_position = glm::vec3(-_position.z, _position.y, _position.x);
		}
		_rotationMatrix = glm::rotate(_rotationMatrix,glm::radians(angle),glm::vec3(0,1,0));
	}
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

bool Cappuccino::HitBox::checkPlane(glm::vec3 dist, glm::vec3 plane, HitBox other)
{
	return (std::fabs(glm::dot(dist, plane)) > 
		(
			 fabs(glm::dot((glm::vec3(_rotationMatrix[0])*(_size.x/2)),plane))+
			 fabs(glm::dot((glm::vec3(_rotationMatrix[1])*(_size.y/2)),plane))+
			 fabs(glm::dot((glm::vec3(_rotationMatrix[2])*(_size.z/2)),plane))+
			 fabs(glm::dot((glm::vec3(other._rotationMatrix[0])*(other._size.x/2)),plane))+
			 fabs(glm::dot((glm::vec3(other._rotationMatrix[1])*(other._size.y/2)),plane))+
			 fabs(glm::dot((glm::vec3(other._rotationMatrix[2])*(other._size.z/2)),plane))
		));
	//https://gamedev.stackexchange.com/questions/112883/simple-3d-obb-collision-directx9-c for help
}

