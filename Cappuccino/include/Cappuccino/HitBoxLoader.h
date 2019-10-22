#pragma once
#include <string>
#include <vector>
#include "HitBox.h"
#include <glm/glm.hpp>
#include <fstream>
namespace Cappuccino
{
	class HitBoxLoader
	{
	public:
		HitBoxLoader(const char* filename);
		/*
		Purp: This function will find the radius of the mesh
		Pre: None
		Post: A float of the radius of the circle hitbox
		*/
		float findRadius();
		/*
		Purp: This function will find the size of cube hitbox 
		Pre: None
		Post: A vec3 of the size of the cube hitBox
		*/
		glm::vec3 findBox();
		/*
		Purp: This function will find the center of hitbox
		Pre: None
		Post: A vec3 of the position of the hitbox relative to the rigidbody location
		*/
		glm::vec3 findCenter();
		std::vector <HitBox> _boxes;
	private:

		std::vector <glm::vec3> _tempVerts;
	};
}