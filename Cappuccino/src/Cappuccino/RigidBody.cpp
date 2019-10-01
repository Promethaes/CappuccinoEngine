#include "RigidBody.h"


Cappuccino::RigidBody::RigidBody(std::vector<glm::vec3> s1, std::vector<glm::vec3> s2)
{
	shapeOne = s1;
	shapeTwo = s2;
}

bool Cappuccino::RigidBody::GJK(std::vector<glm::vec3> shapeOne, std::vector<glm::vec3> shapeTwo)
{
	// setting arbitrary direction to start with
	glm::vec3 direction = { 1,0,0 };

	// setting first simplice (point)
	std::vector<glm::vec3> simplices;
	simplices.push_back(getSupport(shapeOne, shapeTwo, direction));

	// setting second simplice (line)
	direction = -direction;
	simplices.push_back(getSupport(shapeOne, shapeTwo, direction));

	// setting third simplice (triangle)
	direction = glm::cross(simplices[0], simplices[1]);
	simplices.push_back(getSupport(shapeOne, shapeTwo, direction));

	// checking if the third simplice is in the direction of the origin
	// removes and replaces it with a simplice in the opposite direction if it is not in the right direction
	if (glm::dot(simplices[-1], direction) <= 0)
	{
		direction = -direction;
		simplices.pop_back();
		simplices.push_back(getSupport(shapeOne, shapeTwo, direction));
	}

	// setting fourth simplice (tetrahedron)
	direction = glm::cross(simplices[0], glm::cross(simplices[1], simplices[2]));
	simplices.push_back(getSupport(shapeOne, shapeTwo, direction));

	// checking if the fourth simplice is in the direction of the origin
	// removes and replaces it with a simplice in the opposite direction if it is not in the right direction
	if (glm::dot(simplices[-1], direction) <= 0)
	{
		direction = -direction;
		simplices.pop_back();
		simplices.push_back(getSupport(shapeOne, shapeTwo, direction));
	}

	// recursive call to containsOrigin to see if the origin exists inside the concave hull formed by the Minowske Difference
	if (containsOrigin(shapeOne, shapeTwo, simplices))
	{
		return true;
	}
	else
	{
		return false;
	}
}

// recursive function used to determine if the origin is within the simplice
bool Cappuccino::RigidBody::containsOrigin(std::vector<glm::vec3> shapeOne, std::vector<glm::vec3> shapeTwo, std::vector<glm::vec3> vertices)
{
	// Indices 0, 1, 2, and 3 are relative to the vertices A, B, C, and D
	// define a variable to hold the farthest index and value
	float closestDistance = 100000000000; // where's MAXFLOAT
	int farthestIndex = 0;

	// define the opposite of the guaranteed plane in the direction of the origin (negation)
	glm::vec3 O = -glm::cross(vertices[0], glm::cross(vertices[1], vertices[2]));

	// define the three planes to check (bottom side is clear due to the checking done in GJK)
	glm::vec3 ABD = glm::cross(vertices[0], glm::cross(vertices[1], vertices[3]));
	glm::vec3 BCD = glm::cross(vertices[1], glm::cross(vertices[2], vertices[3]));
	glm::vec3 ACD = glm::cross(vertices[0], glm::cross(vertices[2], vertices[3]));

	// check the planes against the negative
	if (glm::dot(ABD, O) <= 0 || glm::dot(BCD, O) <= 0 || glm::dot(ACD, O) <= 0)
	{
		return true;
	}
	else
	{
		// Checks distance between origin and the plane
		// BCD plane
		if (glm::dot(BCD, O) < closestDistance)
		{
			closestDistance = glm::dot(BCD, O);
			farthestIndex = 0;
		}
		// ACD plane
		if (glm::dot(ACD, O) < closestDistance)
		{
			closestDistance = glm::dot(ACD, O);
			farthestIndex = 1;
		}
		// ABD plane
		if (glm::dot(ABD, O) < closestDistance)
		{
			closestDistance = glm::dot(ABD, O);
			farthestIndex = 2;
		}

		// Set the new simplice in the direction of the origin and remove the farthest
		vertices.erase(vertices.begin() + farthestIndex);
		if (farthestIndex == 0)
			vertices.push_back(getSupport(shapeOne, shapeTwo, BCD));
		else if (farthestIndex == 1)
			vertices.push_back(getSupport(shapeOne, shapeTwo, ACD));
		else if (farthestIndex == 2)
			vertices.push_back(getSupport(shapeOne, shapeTwo, ACD));

		// loop to check we don't have duplicate vertices; if we do, origin is no longer within the Minowski Difference
		// and therefore, the two objects are not colliding
		for (int i = 0; i < vertices.size(); i++)
		{
			for (int j = i + 1; j < vertices.size(); j++)
			{
				if (vertices[i] == vertices[j])
				{
					return false;
				}
			}
		}

		// if there are no dupes, recurve
		return containsOrigin(shapeOne, shapeTwo, vertices);
	}
}

// returns the simplice (vertex of the tetrahedron being built)
glm::vec3 Cappuccino::RigidBody::getSupport(std::vector<glm::vec3> shapeOne, std::vector<glm::vec3> shapeTwo, glm::vec3 direction)
{
	glm::vec3 s1 = getFarthest(shapeOne, direction);
	glm::vec3 s2 = getFarthest(shapeTwo, -direction);
	glm::vec3 s3 = s1 - s2;
	return s3;
}

glm::vec3 Cappuccino::RigidBody::getFarthest(std::vector<glm::vec3> shapeOne, glm::vec3 direction)
{
	int farthestIndex = 0;
	float farthestDistance = 0.0f;

	for (int i = 0; i < shapeOne.size(); i++)
	{
		if (glm::dot(shapeOne[i], direction) > farthestDistance)
		{
			farthestDistance = glm::dot(shapeOne[i], direction);
			farthestIndex = i;
		}
	}

	return shapeOne[farthestIndex];
}

