#include "Cappuccino/HitBoxLoader.h"
using namespace Cappuccino;

HitBoxLoader::HitBoxLoader(const char* filename)
{
	char tempName[256];
	bool start = true;
	_boxes.push_back(HitBox(glm::vec3(0),glm::vec3(0)));
	FILE* file = fopen(filename, "r");
	if (file == NULL)
		printf("Failed to open file\n");
	bool moreFile = true;
	while (moreFile)
	{
		char line[1024];
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
				//hitBox creation
				fscanf(file, "%s\n", &tempName);
				_tempVerts.clear();
			}
			else
			{
				fscanf(file, "%s\n", &tempName);
				start ^= 1;
			}
			
		}
		else if (strcmp(line, "v") == 0) {
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			_tempVerts.push_back(vertex);
		}
		

	}
	
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

glm::vec3 Cappuccino::HitBoxLoader::findSize()
{
	//TODO
}
