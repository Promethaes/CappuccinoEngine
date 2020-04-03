#include "Cappuccino/LUT.h"

#include "Cappuccino/CappMacros.h"
#include "Cappuccino/ResourceManager.h"

#include <algorithm>
#include <fstream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <sstream>

namespace Cappuccino {

	std::string LUT::_lutDirectory = "./Assets/LUTs/";

	LUT::LUT(const std::string& path)
	{
		this->_path = path;
	}
	
	bool LUT::loadLUT()
	{
		if (loaded) {
			return true;
		}

		//loading file
		std::ifstream input(_lutDirectory + _path);

		if (!input.good()) {
			CAPP_PRINT_ERROR("Problem loading file: %s", _path.c_str());
			return false;
		}
		//import data
		while (!input.eof()) {
			std::string inputString;
			getline(input, inputString);

			if(inputString.empty() || inputString[0] == '#') {
				continue;
			}

			if(inputString.compare(0, 5, "TITLE") == 0) {
				//LUT title
				std::string token;
				std::istringstream sin(inputString);

				sin >> token >> _lutName;
				continue;
			}
			
			if (inputString.compare(0, 11, "LUT_3D_SIZE") == 0) {
				std::string token;
				std::istringstream sin(inputString);

				sin >> token >> _lutSize;
				_rgbValues.reserve(static_cast<unsigned>(pow(_lutSize, 3)));
				continue;
			}

			if (inputString.compare(0, 10, "DOMAIN_MIN") == 0) {
				glm::vec3 minTemp;
				std::sscanf(inputString.c_str(), "DOMAIN_MIN %f %f %f", &minTemp.x, &minTemp.y, &minTemp.z);
				_domainMin.push_back(minTemp);
				continue;
			}

			if(inputString.compare(0, 10, "DOMAIN_MAX") == 0) {
				glm::vec3 maxTemp;
				std::sscanf(inputString.c_str(), "DOMAIN_MAX %f %f %f", &maxTemp.x, &maxTemp.y, &maxTemp.z);
				_domainMax.push_back(maxTemp);
				continue;
			}

			glm::vec3 line;
			const bool lutReadStatus = std::sscanf(inputString.c_str(), "%f %f %f", &line.r, &line.g, &line.b);
			CAPP_ASSERT(lutReadStatus, "Failed to read LUT colour values!\n\tFilepath: %s", _path.c_str());
			_rgbValues.push_back(line);
		}

		input.close();

		// Load LUT
		glGenTextures(1, &_textureID);
		glBindTexture(GL_TEXTURE_3D, _textureID);

		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);

		glTexImage3D(GL_TEXTURE_3D, 0, GL_RGB8, _lutSize, _lutSize, _lutSize, 0, GL_RGB, GL_FLOAT, _rgbValues.data());

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
