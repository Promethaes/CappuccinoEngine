#include "Cappuccino/FrameBuffer.h"
#include "glad/glad.h"
#include "Cappuccino/CappMacros.h"
using namespace Cappuccino;
std::vector<Framebuffer*> Framebuffer::_framebuffers = {};
Cappuccino::Framebuffer::Framebuffer(const glm::vec2& windowSize, bool attachColour, bool attachDepth, bool attachStencil, void(*instructionsCallback)())
	:_windowSize(windowSize), _callback(instructionsCallback)
{

	generate(_fbo);
	bind();
	generateTextureAttachment(_colourBuffer);

	//attach colour as a texture (important for post proccessing)
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _colourBuffer, 0);

	generateRenderBufferAttachment(_depthStencilBuffer);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _depthStencilBuffer);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {

		printf("Fatal Error: Framebuffer incomplete\n");
		__debugbreak();
	}

	_framebuffers.push_back(this);

}

void Cappuccino::Framebuffer::generate(unsigned& fbo)
{
	glGenFramebuffers(1, &fbo);
}

void Cappuccino::Framebuffer::bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
}

void Cappuccino::Framebuffer::unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Cappuccino::Framebuffer::generateTextureAttachment(unsigned& handle)
{
	//generate the texture to bind to the framebuffer
	glGenTextures(1, &handle);
	glBindTexture(GL_TEXTURE_2D, handle);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _windowSize.x, _windowSize.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, handle, 0);
}

void Cappuccino::Framebuffer::generateRenderBufferAttachment(unsigned& handle)
{
	glGenRenderbuffers(1, &handle);
	glBindRenderbuffer(GL_RENDERBUFFER, handle);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, _windowSize.x, _windowSize.y);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}
