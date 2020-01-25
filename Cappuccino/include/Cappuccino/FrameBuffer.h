#pragma once
#include "glm/common.hpp"
#include <vector>

namespace Cappuccino {

	class Framebuffer {
	public:
		Framebuffer(const glm::vec3& windowSize,bool attachColour,bool attachDepth,bool attachStencil = false, void(*instructionsCallback)() = nullptr);
		
		static std::vector<Framebuffer*> _framebuffers;
		
		void bind();
		void unbind();
		void(*_callback)();
		unsigned& getColourBuffer() { return _colourBuffer; }
	private:
		void generate(unsigned& fbo);
		void generateTextureAttachment(unsigned& handle);

		//currently only generates depthstencil buffer, will perhaps add more functionality later
		void generateRenderBufferAttachment(unsigned& handle);

		//framebuffer object handle
		unsigned _fbo = 0;

		unsigned _colourBuffer = 0,_depthBuffer = 0,_stencilBuffer = 0;
		unsigned _depthStencilBuffer = 0;

		glm::vec3 _windowSize;
	};
}