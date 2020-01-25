#pragma once
#include "glm/common.hpp"
#include "ShaderProgram.h"
#include <vector>
#include <optional>

namespace Cappuccino {

	//framebuffer class, currently only supports colour buffer as a texture and depth/stencil buffer as a renderbuffer
	class Framebuffer {
	public:
		//enable your depth testing, blending, and what not with a callback given in this constructor
		Framebuffer(const glm::vec2& windowSize, void(*instructionsCallback)() = nullptr, const std::optional<char*>& vertShader = std::nullopt, const std::optional<char*>& fragShader = std::nullopt);
		
		static std::vector<Framebuffer*> _framebuffers;
		
		void bind();
		void unbind();
		void(*_callback)();
		unsigned& getColourBuffer() { return _colourBuffer; }

		//pointer so it can be initialized properly later
		Shader* _fbShader;
	private:
		char* _vertShader;

		char* _fragShader;

		void generate(unsigned& fbo);
		void generateTextureAttachment(unsigned& handle);

		//currently only generates depthstencil buffer, will perhaps add more functionality later
		void generateRenderBufferAttachment(unsigned& handle);

		//framebuffer object handle
		unsigned _fbo = 0;

		unsigned _colourBuffer = 0,_depthBuffer = 0,_stencilBuffer = 0;
		unsigned _depthStencilBuffer = 0;

		glm::vec2 _windowSize;
	};
}