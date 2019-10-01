#pragma once
#include <glm/glm.hpp>
#include <vector>

namespace Cappuccino {
	class RigidBody {

		RigidBody(std::vector<glm::vec3> s1, std::vector<glm::vec3> s2);
		
		/*
		RigidBody(std::vector<glm::vec3> vertexData);
		void computeForce(float dt, glm::vec3 force);
		void computeGravity(float dt);
		void computeRotation(float dt, glm::vec3 rotation);
		void computeSAT(std::vector<glm::vec3> collider);
		*/


		bool GJK(std::vector<glm::vec3> shapeOne, std::vector<glm::vec3> shapeTwo);
		bool containsOrigin(std::vector<glm::vec3> shapeOne, std::vector<glm::vec3> shapeTwo, std::vector<glm::vec3> vertices);
		glm::vec3 getSupport(std::vector<glm::vec3> shapeOne, std::vector<glm::vec3> shapeTwo, glm::vec3 direction);
		glm::vec3 getFarthest(std::vector<glm::vec3> shapeOne, glm::vec3 direction);
		
	private:

		// test variables go here
		std::vector<glm::vec3> shapeOne;
		std::vector<glm::vec3> shapeTwo;

		/*
		std::vector<glm::vec3> convexHull;
		std::vector<glm::vec3> weakpoint;

		float gravity = 9.81f;
		float velocity= 0.0f;
		float mass= 1.0f;
		*/
	};
}

