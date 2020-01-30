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
		//only change the vert/frag shaders if you want to change the post processing shader. the shader is static.
		Framebuffer(const glm::vec2& windowSize, unsigned numColourBuffers = 1, void(*instructionsCallback)() = nullptr, const std::optional<char*>& vertShader = std::nullopt, const std::optional<char*>& fragShader = std::nullopt);

		static std::vector<Framebuffer*> _framebuffers;

		void bind();
		void unbind();
		void(*_callback)();
		std::vector<unsigned>& getColourBuffers() { return _colourBuffers; }

		//pointer so it can be initialized properly later
		static Shader* _fbShader;
	private:
		static char* _vertShader;

		static char* _fragShader;

		void generate(unsigned& fbo);
		void generateTextureAttachment();
		void attachTextures();

		//currently only generates depthstencil buffer, will perhaps add more functionality later
		void generateRenderBufferAttachment(unsigned& handle);

		//framebuffer object handle
		unsigned _fbo = 0;

		std::vector<unsigned> _colourBuffers;

		unsigned _depthBuffer = 0, _stencilBuffer = 0;
		unsigned _depthStencilBuffer = 0;

		glm::vec2 _windowSize;
	};
}