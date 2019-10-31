#include "Cappuccino/HitBoxLoader.h"

Cappuccino::HitBoxLoader::HitBoxLoader(const char* filename)
{
	char tempName[256];
	bool start = true;

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
			if (tempName == "Cube")
			{
				tempBox = HitBox(findCenter(), findBox());
			}
			else if (tempName == "Icosphere")
			{
				tempBox = HitBox(findCenter(), findRadius());
			}
			//hitBox creation
			_boxes.push_back(tempBox);
			_tempVerts.clear();
			moreFile ^= 1;
		}
		else if (strcmp(line, "o") == 0)
		{
			fscanf(file, "%s\n", &tempName);
			if (!start)
			{
				HitBox tempBox;
				if (tempName == "Cube")
				{
					 tempBox = HitBox(findCenter(),findBox());
				}
				else if (tempName == "Icosphere")
				{
					tempBox = HitBox(findCenter(),findRadius());
				}
				//hitBox creation
				_boxes.push_back(tempBox);
				_tempVerts.clear();
			}
			else
			{
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

float Cappuccino::HitBoxLoader::findRadius()
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

glm::vec3 Cappuccino::HitBoxLoader::findBox()
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

glm::vec3 Cappuccino::HitBoxLoader::findCenter()
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
	return glm::vec3(
		(tempHigh.x / 2) + (tempLow.x / 2),
		(tempHigh.y / 2) + (tempLow.y / 2), 
		(tempHigh.z / 2) + (tempLow.z / 2));
}