#pragma once
#include <glm/glm.hpp>
namespace Cappuccino{
	class HitBox{
	public:
		/*
		
		*/
		HitBox(glm::vec3& newPos, float& newRadius);
		HitBox(glm::vec3& newPos,glm::vec3& newSize);
		bool checkCollision(HitBox& other, glm::vec3& rigidLoc, glm::vec3& ourRigidLoc);
		float checkCircleBox(glm::vec3& circ, glm::vec3& boxPos, glm::vec3& boxSize);
		bool checkSize(float box1, float size1, float box2, float size2);
		float checkDist(float circ,float boxPos,float boxSize);
	private:
		glm::vec3 _position;
		float _radius = 0;
		glm::vec3 _size;
		glm::vec2 _rotationAround;
	};
}