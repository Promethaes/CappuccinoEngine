#include <glad/glad.h>
#include <glm/glm.hpp>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <fstream>
#include "Cappuccino/LUT.h"
#include "Cappuccino/CappMacros.h"
#include "Cappuccino/ResourceManager.h"

namespace Cappuccino {

	std::string LUT::_lutDirectory = "./Assets/LUTs/";

	LUT::LUT(const std::string& path)
	{
		this->_path = path;
	}
	bool LUT::loadLUT()
	{
		if (loaded)
			return true;
		char inputString[128];

		std::string titleData;
		std::vector<int> sizeData;
		std::vector<glm::vec3> domainMinData;
		std::vector<glm::vec3> domainMaxData;
		std::vector<glm::vec3> lookupData;
		//loading file
		std::ifstream input{};
		input.open(_lutDirectory + _path);

		if (!input.good()) {
			std::cout << "Problem loading file: " << _path << "\n";
			return false;
		}
		//import data
		while (!input.eof()) {
			input.getline(inputString, 128);

			//LUT title
			if (inputString[0] == 'T') {
				std::string tempTitle;
				std::sscanf(inputString, "TITLE \"%s\"", tempTitle);
				titleData = tempTitle;
			}
			else if (inputString[4] == '3' && inputString[5] == 'D') {
				int tempSize = 0;
				std::sscanf(inputString, "LUT_3D_SIZE %i", &tempSize);
				sizeData.push_back(tempSize);
			}
			else if (inputString[7] == 'M' && inputString[8] == 'I' && inputString[9] == 'N') {
				glm::vec3 minTemp;
				std::sscanf(inputString, "DOMAIN_MIN %f %f %f", &minTemp.x, &minTemp.y, &minTemp.z);
				domainMinData.push_back(minTemp);
			}
			else if (inputString[7] == 'M' && inputString[8] == 'A' && inputString[9] == 'X') {
				glm::vec3 maxTemp;
				std::sscanf(inputString, "DOMAIN_MAX %f %f %f", &maxTemp.x, &maxTemp.y, &maxTemp.z);
				domainMaxData.push_back(maxTemp);
			}
			else if (inputString[0] == '0' || inputString[0] == '1') {
				glm::vec3 lutData(0);
				std::sscanf(inputString, "%f %f %f", &lutData.x, &lutData.y, &lutData.z);
				lookupData.push_back(lutData);
			}
			else
				continue;
		}

		_lutName = titleData;
		_lutSize = sizeData[0];
		_domainMin.push_back(domainMinData[0]);
		_domainMax.push_back(domainMaxData[0]);

		for (unsigned int i = 0; i < lookupData.size(); i++)
			_rgbValues.push_back(lookupData[i]);

		input.close();

		//load texture


		glGenTextures(1, &_textureID);
		glBindTexture(GL_TEXTURE_3D, _textureID);

		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);

		glTexImage3D(GL_TEXTURE_3D, 0, GL_RGB, _lutSize, _lutSize, _lutSize, 0, GL_RGB, GL_FLOAT, _rgbValues.data());
		glBindTexture(GL_TEXTURE_3D, 0);


		return loaded = true;
	}

	void LUT::setDefaultPath(const std::string& directory)
	{
		std::string dir = directory;
		std::transform(dir.begin(), dir.end(), dir.begin(), ::tolower);

		if (dir == "default")
			_lutDirectory = "./Assets/LUTs/";
		else
			_lutDirectory = directory;
	}
}