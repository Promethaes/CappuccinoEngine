#include "..\..\include\Cappuccino\HitBoxLoader.h"

Cappuccino::HitBoxLoader::HitBoxLoader(const char* filename)
{
	std::string tempName;
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
			boxes.push_back(tempBox);
			tempVerts.clear();
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
				boxes.push_back(tempBox);
				tempVerts.clear();
			}
			else
			{
				start ^= 1;
			}
			
		}
		else if (strcmp(line, "v") == 0) {
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			tempVerts.push_back(vertex);
		}
		

	}

}

float Cappuccino::HitBoxLoader::findRadius()
{
	float high = tempVerts[0].y;
	float low = tempVerts[0].y;
	for (unsigned int i = 0; i < tempVerts.size(); i++)
	{
		if (high < tempVerts[i].y)
			high = tempVerts[i].y;

		if (low > tempVerts[i].y)
			low = tempVerts[i].y;
	}
	return (high-low)/2;
}

glm::vec3 Cappuccino::HitBoxLoader::findBox()
{
	glm::vec3 tempHigh = tempVerts[0];
	glm::vec3 tempLow = tempVerts[0];

	for (unsigned int i = 0; i < tempVerts.size(); i++)
	{
		if (tempVerts[i].x > tempHigh.x)
			tempHigh.x = tempVerts[i].x;
		if (tempVerts[i].y > tempHigh.y)
			tempHigh.y = tempVerts[i].y;
		if (tempVerts[i].z > tempHigh.z)
			tempHigh.z = tempVerts[i].z;

		if (tempVerts[i].x < tempLow.x)
			tempLow.x = tempVerts[i].x;
		if (tempVerts[i].y < tempLow.y)
			tempLow.y = tempVerts[i].y;
		if (tempVerts[i].z < tempLow.z)
			tempLow.z = tempVerts[i].z;
	}
	return glm::vec3(tempHigh - tempLow);
}

glm::vec3 Cappuccino::HitBoxLoader::findCenter()
{
	glm::vec3 tempHigh = tempVerts[0];
	glm::vec3 tempLow = tempVerts[0];
	for (unsigned int i = 0; i < tempVerts.size(); i++)
	{
		if (tempVerts[i].x > tempHigh.x)
			tempHigh.x = tempVerts[i].x;
		if (tempVerts[i].y > tempHigh.y)
			tempHigh.y = tempVerts[i].y;
		if (tempVerts[i].z > tempHigh.z)
			tempHigh.z = tempVerts[i].z;

		if (tempVerts[i].x < tempLow.x)
			tempLow.x = tempVerts[i].x;
		if (tempVerts[i].y < tempLow.y)
			tempLow.y = tempVerts[i].y;
		if (tempVerts[i].z < tempLow.z)
			tempLow.z = tempVerts[i].z;
	}
	return glm::vec3(
		(tempHigh.x / 2) + (tempLow.x / 2),
		(tempHigh.y / 2) + (tempLow.y / 2), 
		(tempHigh.z / 2) + (tempLow.z / 2));
}
