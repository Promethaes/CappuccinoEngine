#pragma once

#include <glm/glm.hpp>

namespace Cappuccino {
	/*
	Purp: a point light. be sure to update the view pos every frame
	and also, be sure to load the view matrix every frame (both externally)
	*/
	class PointLight {
	public:
		
		PointLight(const glm::vec3& position, const glm::vec3& colour, bool shadowCaster = false);
		~PointLight();

		void setShadowCaster(bool caster);
		
		glm::vec3 _pos;
		glm::vec3 _col;
		bool _isActive = true;

		class {
			friend PointLight;
			bool value = true;
		public:
			
			operator bool() const { return value; }
		} _isShadowCaster;
		
		unsigned shadowBuffer = 0, depthMap = 0;
		unsigned resolution = 1024;
		glm::mat4 projectionMat = glm::mat4(1.0f);
		glm::mat4 viewMat = glm::mat4(1.0f);
		
	};
}