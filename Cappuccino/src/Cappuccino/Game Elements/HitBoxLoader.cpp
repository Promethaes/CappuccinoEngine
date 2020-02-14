#include "Cappuccino/Game Elements/HitBoxLoader.h"
#include <iostream>
using namespace Cappuccino;

HitBoxLoader::HitBoxLoader(const char* filename)
{
	char tempName[256] = " ";
	bool start = true;
	_boxes.push_back(HitBox(glm::vec3(0),glm::vec3(0)));
	FILE* file = fopen(filename, "r");
	bool moreFile = true;
	if (file == NULL) {
		printf("Failed to open file\n");
		moreFile = false;
	}
		
	
	while (moreFile)
	{
		char line[1024] = {0};
		int lineNumber = fscanf(file, "%s", line);
		if (lineNumber == EOF)
		{
			HitBox tempBox;
			if (tempName[0] == 'C')
			{
				tempBox = HitBox(findCenter(), findBox());
				_boxes.push_back(tempBox);
			}
			else if (tempName[0] == 'I')
			{
				tempBox = HitBox(findCenter(), findRadius());
				_boxes.push_back(tempBox);
			}
			else if (tempName[0] == 'B')
			{
				tempBox = HitBox(findCenter(), findBox());
				_boxes[0] = tempBox;
			}
			else if (tempName[0] == 'R')
			{
				std::string rotationString = tempName;
				rotationString = rotationString.substr(rotationString.find_first_of('_') + 1, rotationString.find_last_of('_') - 5);
				glm::mat4 tempRotation = { 0.7071068,  0.0000000,  0.7071068,0.0f,
					 0.0000000,  1.0000000,  0.0000000 ,0.0f,
					-0.7071068,  0.0000000, 0.7071068 , 0.0f,
					0.0f,0.0f,0.0f,1.0f };
				//glm::mat4 tempRotation = glm::rotate(tempBox._rotationMatrix, std::stof(rotationString), glm::vec3(0, 1, 0));
				tempBox = HitBox(findCenter(), findBox(), tempRotation);
				_boxes.push_back(tempBox);
			}
			_tempVerts.clear();
			moreFile ^= 1;
		}
		else if (strcmp(line, "o") == 0)
		{
			
			if (!start)
			{
				HitBox tempBox;
				if (tempName[0] == 'C')
				{
					 tempBox = HitBox(findCenter(),findBox());
					 _boxes.push_back(tempBox);
				}
				else if (tempName[0] == 'I')
				{
					tempBox = HitBox(findCenter(),findRadius());
					_boxes.push_back(tempBox);
				}
				else if (tempName[0] == 'B')
				{
					tempBox = HitBox(findCenter(), findBox());
					_boxes[0] = tempBox;
				}
				else if (tempName[0] == 'R')
				{
					std::string rotationString = tempName;
					rotationString = rotationString.substr(rotationString.find_first_of('_') + 1, rotationString.find_last_of('_') - 5);
					glm::mat4 tempRotation = { 0.7071068,  0.0000000,  0.7071068,0.0f,//very hard coded
					 0.0000000,  1.0000000,  0.0000000 ,0.0f,
					-0.7071068,  0.0000000, 0.7071068 , 0.0f,
					0.0f,0.0f,0.0f,1.0f };
					// glm::rotate(tempBox._rotationMatrix, std::stof(rotationString), glm::vec3(0, 1, 0));
					tempBox = HitBox(findCenter(), findBox(), tempRotation);

					//for (unsigned i = 0; i < 4; i++) {
					//	for (unsigned j = 0; j < 4; j++) {
					//		std::cout << tempRotation[i][j]<<" ";
					//	}
					//	std::cout << std::endl;
					//}
					_boxes.push_back(tempBox);
				}
				//hitBox creation
				int errorThing = fscanf(file, "%s\n", &tempName);
				_tempVerts.clear();
			}
			else
			{
				int errorThing = fscanf(file, "%s\n", &tempName);
				start ^= 1;
			}
			
		}
		else if (strcmp(line, "v") == 0) {
			glm::vec3 vertex;
			int errorThing = fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			_tempVerts.push_back(vertex);
		}
	}
	_tempVerts.clear();
}

float HitBoxLoader::findRadius()
{
	float high = _tempVerts[0].y;
	float low = _tempVerts[0].y;
	for (unsigned int i = 0; i < _tempVerts.size(); i++)
	{
		if (high < _tempVerts[i].y)
			high = _tempVerts[i].y;

		if (low > _tempVerts[i].y)
			low = _tempVerts[i].y;
	}
	return (low+high)/2-low;
}

glm::vec3 HitBoxLoader::findBox()
{
	glm::vec3 tempHigh = _tempVerts[0];
	glm::vec3 tempLow = _tempVerts[0];

	for (unsigned int i = 0; i < _tempVerts.size(); i++)
	{
		if (_tempVerts[i].x > tempHigh.x)
			tempHigh.x = _tempVerts[i].x;
		if (_tempVerts[i].y > tempHigh.y)
			tempHigh.y = _tempVerts[i].y;
		if (_tempVerts[i].z > tempHigh.z)
			tempHigh.z = _tempVerts[i].z;

		if (_tempVerts[i].x < tempLow.x)
			tempLow.x = _tempVerts[i].x;
		if (_tempVerts[i].y < tempLow.y)
			tempLow.y = _tempVerts[i].y;
		if (_tempVerts[i].z < tempLow.z)
			tempLow.z = _tempVerts[i].z;
	}
	return glm::vec3(tempHigh - tempLow);
}

glm::vec3 HitBoxLoader::findCenter()
{
	glm::vec3 tempHigh = _tempVerts[0];
	glm::vec3 tempLow = _tempVerts[0];
	for (unsigned int i = 0; i < _tempVerts.size(); i++)
	{
		if (_tempVerts[i].x > tempHigh.x)
			tempHigh.x = _tempVerts[i].x;
		if (_tempVerts[i].y > tempHigh.y)
			tempHigh.y = _tempVerts[i].y;
		if (_tempVerts[i].z > tempHigh.z)
			tempHigh.z = _tempVerts[i].z;

		if (_tempVerts[i].x < tempLow.x)
			tempLow.x = _tempVerts[i].x;
		if (_tempVerts[i].y < tempLow.y)
			tempLow.y = _tempVerts[i].y;
		if (_tempVerts[i].z < tempLow.z)
			tempLow.z = _tempVerts[i].z;
	}
	
	return glm::vec3(tempHigh.x / 2 + tempLow.x / 2,
	                 tempHigh.y / 2 + tempLow.y / 2,
	                 tempHigh.z / 2 + tempLow.z / 2);
}
