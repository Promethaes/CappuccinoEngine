#pragma once

#include <string>
#include <vector>

namespace Cappuccino {

	// LUT Loader based on Mesh Loader in CappuccinoEngine
	// Probably move this into engine
	class LUT {
	public:

		LUT(const std::string& path);

		// Loads 3D LUTs (.CUBE only)
		bool loadLUT();

		static void setDefaultPath(const std::string& directory);

		bool loaded = false;
		unsigned int _lutSize;
		std::string _lutName;

		std::vector<glm::vec3> _domainMin;
		std::vector<glm::vec3> _domainMax;
		std::vector<glm::vec3> _rgbValues;

		unsigned _textureID = 0;
	protected:
		static std::string _lutDirectory;
		std::string _path;
	};
}