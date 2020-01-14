#include "Cappuccino/HitBox.h"
#include "Cappuccino/CappMacros.h"

#include <glad/glad.h>

#include <vector>

using namespace Cappuccino;

HitBox::HitBox(glm::vec3& newPos, float newRadius)
{
	_position = newPos;
	_radius = newRadius;

	std::vector<glm::vec3> data;
	CAPP_GL_CALL(glGenVertexArrays(1, &_VAO));
	CAPP_GL_CALL(glGenBuffers(1, &_VBO));
	CAPP_GL_CALL(glBindVertexArray(_VAO));
	CAPP_GL_CALL(glEnableVertexAttribArray(0));
	CAPP_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, _VBO));

	glm::vec3 p1(newPos.x, newPos.y + newRadius, newPos.z);
	glm::vec3 p2(newPos.x, newPos.y, newPos.z - newRadius);
	glm::vec3 p3(newPos.x - newRadius, newPos.y, newPos.z);
	glm::vec3 p4(newPos.x, newPos.y, newPos.z + newRadius);
	glm::vec3 p5(newPos.x + newRadius, newPos.y, newPos.z);
	glm::vec3 p6(newPos.x, newPos.y - newRadius, newPos.z);


	data.push_back(p1);
	data.push_back(p4);
	data.push_back(p5);
	data.push_back(p1);
	data.push_back(p2);
	data.push_back(p5);
	data.push_back(p1);
	data.push_back(p2);
	data.push_back(p3);
	data.push_back(p1);
	data.push_back(p3);
	data.push_back(p4);
	data.push_back(p6);
	data.push_back(p4);
	data.push_back(p5);
	data.push_back(p6);
	data.push_back(p2);
	data.push_back(p5);
	data.push_back(p6);
	data.push_back(p2);
	data.push_back(p3);
	data.push_back(p6);
	data.push_back(p3);
	data.push_back(p4);

	std::vector <float> data2;
	for (unsigned i = 0; i < data.size(); i++)
	{
		data2.push_back(data[i].x);
		data2.push_back(data[i].y);
		data2.push_back(data[i].z);
	}

	_numVerts = data2.size();

	CAPP_GL_CALL(glBufferData(GL_ARRAY_BUFFER, data2.size() * sizeof(float), &data2[0], GL_STATIC_DRAW));
	CAPP_GL_CALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0));

	CAPP_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
	CAPP_GL_CALL(glBindVertexArray(0));

}

HitBox::HitBox(glm::vec3& newPos, glm::vec3& newSize)
{
	_position = newPos;
	_size = newSize ;
	std::vector<glm::vec3> data;
	CAPP_GL_CALL(glGenVertexArrays(1, &_VAO));
	CAPP_GL_CALL(glBindVertexArray(_VAO));
	CAPP_GL_CALL(glGenBuffers(1, &_VBO));
	CAPP_GL_CALL(glEnableVertexAttribArray(0));
	CAPP_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, _VBO));

	glm::vec3 p1(newPos.x - (newSize.x / 2.0f), newPos.y + (newSize.y / 2.0f), newPos.z + (newSize.z / 2.0f));
	glm::vec3 p2(newPos.x + (newSize.x / 2.0f), newPos.y + (newSize.y / 2.0f), newPos.z + (newSize.z / 2.0f));
	glm::vec3 p3(newPos.x - (newSize.x / 2.0f), newPos.y - (newSize.y / 2.0f), newPos.z + (newSize.z / 2.0f));
	glm::vec3 p4(newPos.x + (newSize.x / 2.0f), newPos.y - (newSize.y / 2.0f), newPos.z + (newSize.z / 2.0f));
	glm::vec3 p5(newPos.x - (newSize.x / 2.0f), newPos.y + (newSize.y / 2.0f), newPos.z - (newSize.z / 2.0f));
	glm::vec3 p6(newPos.x + (newSize.x / 2.0f), newPos.y + (newSize.y / 2.0f), newPos.z - (newSize.z / 2.0f));
	glm::vec3 p7(newPos.x - (newSize.x / 2.0f), newPos.y - (newSize.y / 2.0f), newPos.z - (newSize.z / 2.0f));
	glm::vec3 p8(newPos.x + (newSize.x / 2.0f), newPos.y - (newSize.y / 2.0f), newPos.z - (newSize.z / 2.0f));

	data.push_back(p1);
	data.push_back(p2);
	data.push_back(p4);
	data.push_back(p1);
	data.push_back(p3);
	data.push_back(p4);
	data.push_back(p5);
	data.push_back(p6);
	data.push_back(p8);
	data.push_back(p5);
	data.push_back(p7);
	data.push_back(p8);
	data.push_back(p5);
	data.push_back(p6);
	data.push_back(p2);
	data.push_back(p5);
	data.push_back(p1);
	data.push_back(p2);
	data.push_back(p3);
	data.push_back(p7);
	data.push_back(p8);
	data.push_back(p3);
	data.push_back(p4);
	data.push_back(p8);
	data.push_back(p1);
	data.push_back(p5);
	data.push_back(p7);
	data.push_back(p1);
	data.push_back(p3);
	data.push_back(p7);
	data.push_back(p2);
	data.push_back(p6);
	data.push_back(p8);
	data.push_back(p2);
	data.push_back(p4);
	data.push_back(p8);

	std::vector <float> data2;
	for (unsigned i = 0; i < data.size(); i++)
	{
		data2.push_back(data[i].x);
		data2.push_back(data[i].y);
		data2.push_back(data[i].z);
	}

	_numVerts = data2.size();

	CAPP_GL_CALL(glBufferData(GL_ARRAY_BUFFER, data2.size() * sizeof(float), &data2[0], GL_STATIC_DRAW));
	CAPP_GL_CALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0));

	CAPP_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
	CAPP_GL_CALL(glBindVertexArray(0));
}

Cappuccino::HitBox::HitBox(glm::vec3& newPos, glm::vec3& newSize, glm::mat4& newRotation)
{
	_OBB = true;
	_position = newPos;
	_size = newSize;
	_rotationMatrix = newRotation;

	std::vector<glm::vec3> data;
	CAPP_GL_CALL(glGenVertexArrays(1, &_VAO));
	CAPP_GL_CALL(glBindVertexArray(_VAO));
	CAPP_GL_CALL(glGenBuffers(1, &_VBO));
	CAPP_GL_CALL(glEnableVertexAttribArray(0));
	CAPP_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, _VBO));

	glm::vec3 p1(newPos.x - (newSize.x / 2.0f), newPos.y + (newSize.y / 2.0f), newPos.z + (newSize.z / 2.0f));
	glm::vec3 p2(newPos.x + (newSize.x / 2.0f), newPos.y + (newSize.y / 2.0f), newPos.z + (newSize.z / 2.0f));
	glm::vec3 p3(newPos.x - (newSize.x / 2.0f), newPos.y - (newSize.y / 2.0f), newPos.z + (newSize.z / 2.0f));
	glm::vec3 p4(newPos.x + (newSize.x / 2.0f), newPos.y - (newSize.y / 2.0f), newPos.z + (newSize.z / 2.0f));
	glm::vec3 p5(newPos.x - (newSize.x / 2.0f), newPos.y + (newSize.y / 2.0f), newPos.z - (newSize.z / 2.0f));
	glm::vec3 p6(newPos.x + (newSize.x / 2.0f), newPos.y + (newSize.y / 2.0f), newPos.z - (newSize.z / 2.0f));
	glm::vec3 p7(newPos.x - (newSize.x / 2.0f), newPos.y - (newSize.y / 2.0f), newPos.z - (newSize.z / 2.0f));
	glm::vec3 p8(newPos.x + (newSize.x / 2.0f), newPos.y - (newSize.y / 2.0f), newPos.z - (newSize.z / 2.0f));

	data.push_back(p1);
	data.push_back(p2);
	data.push_back(p4);
	data.push_back(p1);
	data.push_back(p3);
	data.push_back(p4);
	data.push_back(p5);
	data.push_back(p6);
	data.push_back(p8);
	data.push_back(p5);
	data.push_back(p7);
	data.push_back(p8);
	data.push_back(p5);
	data.push_back(p6);
	data.push_back(p2);
	data.push_back(p5);
	data.push_back(p1);
	data.push_back(p2);
	data.push_back(p3);
	data.push_back(p7);
	data.push_back(p8);
	data.push_back(p3);
	data.push_back(p4);
	data.push_back(p8);
	data.push_back(p1);
	data.push_back(p5);
	data.push_back(p7);
	data.push_back(p1);
	data.push_back(p3);
	data.push_back(p7);
	data.push_back(p2);
	data.push_back(p6);
	data.push_back(p8);
	data.push_back(p2);
	data.push_back(p4);
	data.push_back(p8);

	std::vector <float> data2;
	for (unsigned i = 0; i < data.size(); i++)
	{
		data2.push_back(data[i].x);
		data2.push_back(data[i].y);
		data2.push_back(data[i].z);
	}

	_numVerts = data2.size();

	CAPP_GL_CALL(glBufferData(GL_ARRAY_BUFFER, data2.size() * sizeof(float), &data2[0], GL_STATIC_DRAW));
	CAPP_GL_CALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0));

	CAPP_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
	CAPP_GL_CALL(glBindVertexArray(0));
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
		else if (_OBB)
		{
			//TODO
		}
		else {
			//circle then box
			float dist = checkCircleBox(ourRigidLoc + _position, rigidLoc + other._position, other._size);
			if (dist <= _radius * _radius)
				return true;
		}
	}
	else if (_OBB) {
		if (_radius) {// OBB sphere
			//TODO
		}
		else if (_OBB) {//OBB OBB collision
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
		else//OBB AABB collision
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
		if (other._radius) {
			//box then circle
			float dist = checkCircleBox(rigidLoc + other._position, ourRigidLoc + _position, _size);
			if (dist <= (other._radius * other._radius))
				return true;
		}
		else if (_OBB){//AABB OBB collision
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
		else {//box box
			if (checkSize((_position.x + ourRigidLoc.x), _size.x, (other._position.x + rigidLoc.x), other._size.x) && checkSize((_position.y + ourRigidLoc.y), _size.y, (other._position.y + rigidLoc.y), other._size.y) && checkSize((_position.z + ourRigidLoc.z), _size.z, (other._position.z + rigidLoc.z), other._size.z))
				return true;
		}
	}
	return false;
}

void Cappuccino::HitBox::draw()
{
	CAPP_GL_CALL(glBindVertexArray(_VAO));
	CAPP_GL_CALL(glDrawArrays(GL_TRIANGLES, 0, _numVerts / 3));	
}

bool HitBox::intersecting(const Ray& ray, glm::vec3& pos)
{
https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-box-intersection
	if (!_radius)
	{
		float txMin, txMax, tyMin, tyMax, tzMin, tzMax;
		auto inverseDir = 1.0f / (*ray._rayDir);

		glm::vec3 bounds[2]{ pos + _position - _size / 2.0f,pos + _position + _size / 2.0f };
		std::vector<int> sign;

		sign.push_back((inverseDir.x < 0));
		sign.push_back((inverseDir.y < 0));
		sign.push_back((inverseDir.z < 0));

		txMin = (bounds[sign[0]].x - ray._rayPos->x) * inverseDir.x;
		txMax = (bounds[1 - sign[0]].x - ray._rayPos->x) * inverseDir.x;

		tyMin = (bounds[sign[1]].y - ray._rayPos->y) * inverseDir.y;
		tyMax = (bounds[1 - sign[1]].y - ray._rayPos->y) * inverseDir.y;

		tzMin = (bounds[sign[2]].z - ray._rayPos->z) * inverseDir.z;
		tzMax = (bounds[1 - sign[2]].z - ray._rayPos->z) * inverseDir.z;

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
	}
	else
	{
		//TODO RAY/SPHERE
		return false;
	}
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
		if (_radius)
			rebindVBO(_position, _radius);
		else
			rebindVBO(_position, _size);
	}
	else
	{
		_rotationMatrix = glm::rotate(_rotationMatrix,glm::radians(angle),glm::vec3(0,0,1));
	}
}

void Cappuccino::HitBox::rebindVBO(glm::vec3& newPos, float newRadius)
{
	CAPP_GL_CALL(glGenVertexArrays(1, &_VAO));
	CAPP_GL_CALL(glBindVertexArray(_VAO));
	CAPP_GL_CALL(glGenBuffers(1, &_VBO));
	CAPP_GL_CALL(glEnableVertexAttribArray(0));
	CAPP_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, _VBO));
	glm::vec3 data [24];
	glm::vec3 p1(newPos.x, newPos.y + newRadius, newPos.z);
	glm::vec3 p2(newPos.x, newPos.y, newPos.z - newRadius);
	glm::vec3 p3(newPos.x - newRadius, newPos.y, newPos.z);
	glm::vec3 p4(newPos.x, newPos.y, newPos.z + newRadius);
	glm::vec3 p5(newPos.x + newRadius, newPos.y, newPos.z);
	glm::vec3 p6(newPos.x, newPos.y - newRadius, newPos.z);


	data[0]=(p1);
	data[1]=(p4);
	data[2]=(p5);
	data[3]=(p1);
	data[4]=(p2);
	data[5]=(p5);
	data[6]=(p1);
	data[7]=(p2);
	data[8]=(p3);
	data[9]=(p1);
	data[10]=(p3);
	data[11]=(p4);
	data[12]=(p6);
	data[13]=(p4);
	data[14]=(p5);
	data[15]=(p6);
	data[16]=(p2);
	data[17]=(p5);
	data[18]=(p6);
	data[19]=(p2);
	data[20]=(p3);
	data[21]=(p6);
	data[22]=(p3);
	data[23]=(p4);

	float data2 [24*3];
	for (unsigned i = 0; i < 24; i++)
	{
		data2[i*3]=(data[i].x);
		data2[i*3+1]=(data[i].y);
		data2[i*3+2]=(data[i].z);
	}

	_numVerts = 24*3;

	CAPP_GL_CALL(glBufferData(GL_ARRAY_BUFFER, 24*3 * sizeof(float), &data2[0], GL_STATIC_DRAW));
	CAPP_GL_CALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0));

	CAPP_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
	CAPP_GL_CALL(glBindVertexArray(0));
}

void Cappuccino::HitBox::rebindVBO(glm::vec3& newPos, glm::vec3& newSize)
{
	CAPP_GL_CALL(glGenVertexArrays(1, &_VAO));
	CAPP_GL_CALL(glBindVertexArray(_VAO));
	CAPP_GL_CALL(glGenBuffers(1, &_VBO));
	CAPP_GL_CALL(glEnableVertexAttribArray(0));
	CAPP_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, _VBO));
	glm::vec3 data[36];
	glm::vec3 p1(newPos.x - (newSize.x / 2.0f), newPos.y + (newSize.y / 2.0f), newPos.z + (newSize.z / 2.0f));
	glm::vec3 p2(newPos.x + (newSize.x / 2.0f), newPos.y + (newSize.y / 2.0f), newPos.z + (newSize.z / 2.0f));
	glm::vec3 p3(newPos.x - (newSize.x / 2.0f), newPos.y - (newSize.y / 2.0f), newPos.z + (newSize.z / 2.0f));
	glm::vec3 p4(newPos.x + (newSize.x / 2.0f), newPos.y - (newSize.y / 2.0f), newPos.z + (newSize.z / 2.0f));
	glm::vec3 p5(newPos.x - (newSize.x / 2.0f), newPos.y + (newSize.y / 2.0f), newPos.z - (newSize.z / 2.0f));
	glm::vec3 p6(newPos.x + (newSize.x / 2.0f), newPos.y + (newSize.y / 2.0f), newPos.z - (newSize.z / 2.0f));
	glm::vec3 p7(newPos.x - (newSize.x / 2.0f), newPos.y - (newSize.y / 2.0f), newPos.z - (newSize.z / 2.0f));
	glm::vec3 p8(newPos.x + (newSize.x / 2.0f), newPos.y - (newSize.y / 2.0f), newPos.z - (newSize.z / 2.0f));

	data[0]=(p1);
	data[1]=(p2);
	data[2]=(p4);
	data[3]=(p1);
	data[4]=(p3);
	data[5]=(p4);
	data[6]=(p5);
	data[7]=(p6);
	data[8]=(p8);
	data[9]=(p5);
	data[10]=(p7);
	data[11]=(p8);
	data[12]=(p5);
	data[13]=(p6);
	data[14]=(p2);
	data[15]=(p5);
	data[16]=(p1);
	data[17]=(p2);
	data[18]=(p3);
	data[19]=(p7);
	data[20]=(p8);
	data[21]=(p3);
	data[22]=(p4);
	data[23]=(p8);
	data[24]=(p1);
	data[25]=(p5);
	data[26]=(p7);
	data[27]=(p1);
	data[28]=(p3);
	data[29]=(p7);
	data[30]=(p2);
	data[31]=(p6);
	data[32]=(p8);
	data[33]=(p2);
	data[34]=(p4);
	data[35]=(p8);

	float data2 [36*3];
	for (unsigned i = 0; i < 36; i++)
	{
		data2[i*3]=(data[i].x);
		data2[i*3+1]=(data[i].y);
		data2[i*3+2]=(data[i].z);
	}

	_numVerts = 36*3;

	CAPP_GL_CALL(glBufferData(GL_ARRAY_BUFFER, 36 * 3 * sizeof(float), &data2[0], GL_STATIC_DRAW));
	CAPP_GL_CALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0));

	CAPP_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
	CAPP_GL_CALL(glBindVertexArray(0));
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

	//return ((dist*plane).length()>(
	//	((glm::vec3(_rotationMatrix[0][0],_rotationMatrix[0][1], _rotationMatrix[0][2])*(_size/2.0f))*plane).length() +
	//	((glm::vec3(_rotationMatrix[1][0], _rotationMatrix[1][1], _rotationMatrix[1][2]) * (_size / 2.0f)) * plane).length() +
	//	((glm::vec3(_rotationMatrix[2][0], _rotationMatrix[2][1], _rotationMatrix[2][2]) * (_size / 2.0f)) * plane).length() +
	//	((glm::vec3(other._rotationMatrix[0][0], other._rotationMatrix[0][1], other._rotationMatrix[0][2]) * (other._size / 2.0f)) * plane).length() +
	//	((glm::vec3(other._rotationMatrix[1][0], other._rotationMatrix[1][1], other._rotationMatrix[1][2]) * (other._size / 2.0f)) * plane).length() +
	//	((glm::vec3(other._rotationMatrix[2][0], other._rotationMatrix[2][1], other._rotationMatrix[2][2]) * (other._size / 2.0f)) * plane).length()
	//	));

	//https://gamedev.stackexchange.com/questions/112883/simple-3d-obb-collision-directx9-c for help
}

