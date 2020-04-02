#include "Cappuccino/PointLight.h"
#include "Cappuccino/ResourceManager.h"

#include <glm/gtc/matrix_transform.hpp>


Cappuccino::PointLight::PointLight(const glm::vec3& position, const glm::vec3& colour, const bool shadowCaster) :
	_pos(position), _col(colour), isShadowCaster(shadowCaster)
{
	if(shadowCaster) {
		projectionMat = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 400.0f);

		const auto transform = glm::translate(glm::mat4(1.0f), position);
		viewMat = glm::inverse(transform);

		glCreateFramebuffers(1, &shadowBuffer);
		glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &depthMap);
		glBindTexture(GL_TEXTURE_CUBE_MAP, depthMap);
		for(unsigned i = 0; i < 6; ++i) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT16, resolution, resolution, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
		}

		glTextureParameteri(depthMap, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTextureParameteri(depthMap, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTextureParameteri(depthMap, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTextureParameteri(depthMap, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(depthMap, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glNamedFramebufferTexture(shadowBuffer, GL_DEPTH_ATTACHMENT, depthMap, 0);

		if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			printf("ERROR: POINT LIGHT FRAMEBUFFER NOT COMPLETE\n");
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}
