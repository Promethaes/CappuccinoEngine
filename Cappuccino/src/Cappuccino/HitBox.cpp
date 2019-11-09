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
	_size = newSize;
	std::vector<glm::vec3> data;
	CAPP_GL_CALL(glGenVertexArrays(1, &_VAO));
	CAPP_GL_CALL(glBindVertexArray(_VAO));
	CAPP_GL_CALL(glGenBuffers(1, &_VBO));
	CAPP_GL_CALL(glEnableVertexAttribArray(0));
	CAPP_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, _VBO));

	glm::vec3 p1(newPos.x - (newSize.x / 2), newPos.y + (newSize.y / 2), newPos.z + (newSize.z / 2));
	glm::vec3 p2(newPos.x + (newSize.x / 2), newPos.y + (newSize.y / 2), newPos.z + (newSize.z / 2));
	glm::vec3 p3(newPos.x - (newSize.x / 2), newPos.y - (newSize.y / 2), newPos.z + (newSize.z / 2));
	glm::vec3 p4(newPos.x + (newSize.x / 2), newPos.y - (newSize.y / 2), newPos.z + (newSize.z / 2));
	glm::vec3 p5(newPos.x - (newSize.x / 2), newPos.y + (newSize.y / 2), newPos.z - (newSize.z / 2));
	glm::vec3 p6(newPos.x + (newSize.x / 2), newPos.y + (newSize.y / 2), newPos.z - (newSize.z / 2));
	glm::vec3 p7(newPos.x - (newSize.x / 2), newPos.y - (newSize.y / 2), newPos.z - (newSize.z / 2));
	glm::vec3 p8(newPos.x + (newSize.x / 2), newPos.y - (newSize.y / 2), newPos.z - (newSize.z / 2));

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
		else {
			//circle then box
			float dist = checkCircleBox(ourRigidLoc + _position, rigidLoc + other._position, other._size);
			if (dist <= _radius * _radius)
				return true;
		}
	}
	else
	{
		if (other._radius) {
			//box then circle
			float dist = checkCircleBox(rigidLoc + other._position, ourRigidLoc + _position, _size);
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
	else
	{
		//TODO RAY/SPHERE
		return false;
	}
}

void Cappuccino::HitBox::rotateBox(float angle)
{
	int temp1, temp2;
	if (angle/90.0f == 1.0f)
	{
		temp1 = _size.x;
		temp2 = _size.z;
		_size.x = temp2;
		_size.z = temp1;
		_position = glm::vec3(_position.z,_position.y,-_position.x);
	}
	else if (angle / 90.0f == 2.0f)
	{
		_position.x *= -1;
		_position.z *= -1;
	}
	else if (angle / 90.0f == 3.0f)
	{
		temp1 = _size.x;
		temp2 = _size.z;
		_size.x = temp2;
		_size.z = temp1;
		_position = glm::vec3(-_position.z, _position.y, _position.x);
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
