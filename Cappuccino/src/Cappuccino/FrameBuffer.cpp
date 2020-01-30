#include "Cappuccino/FrameBuffer.h"
#include "glad/glad.h"
#include "Cappuccino/CappMacros.h"
using namespace Cappuccino;
std::vector<Framebuffer*> Framebuffer::_framebuffers = {};

char* Framebuffer::_vertShader = "";
char* Framebuffer::_fragShader = "";
Shader* Framebuffer::_fbShader = nullptr;
std::vector<unsigned> Framebuffer::_colourBuffers = {};
Cappuccino::Framebuffer::Framebuffer(const glm::vec2& windowSize, unsigned numColourBuffers, void(*instructionsCallback)(), const std::optional<char*>& vertShader, const std::optional<char*>& fragShader)
	:_windowSize(windowSize), _callback(instructionsCallback)
{
	_vertShader =
		R"(#version 420 core
				layout (location = 0) in vec2 aPos;
				layout (location = 1) in vec2 aTexCoords;

				out vec2 TexCoords;

				void main()
				{
					gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0); 
					TexCoords = aTexCoords;
				}  )";
	_fragShader =
		R"(#version 420 core
				out vec4 FragColor;
  
				in vec2 TexCoords;

				uniform sampler2D screenTexture;

				void main()
				{ 
					FragColor = texture(screenTexture, TexCoords);
				})";

	if (vertShader.has_value())
		_vertShader = vertShader.value();
	if (fragShader.has_value())
		_fragShader = fragShader.value();
	if (_fbShader == nullptr)
		_fbShader = new Shader(true, _vertShader, _fragShader);

	generate(_fbo);
	bind();
	for (unsigned i = 0; i < numColourBuffers; i++) {
		if (i > 7)
			break;
		generateTextureAttachment();
	}

	//attach colour as a texture (important for post proccessing)
	attachTextures();

	generateRenderBufferAttachment(_depthStencilBuffer);
	CAPP_GL_CALL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _depthStencilBuffer));

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {

		printf("Fatal Error: Framebuffer incomplete\n");
		__debugbreak();
	}

	unbind();

	_framebuffers.push_back(this);

}

void Cappuccino::Framebuffer::generate(unsigned& fbo)
{
	CAPP_GL_CALL(glGenFramebuffers(1, &fbo));
}

void Cappuccino::Framebuffer::bind()
{
	CAPP_GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, _fbo));
}

void Cappuccino::Framebuffer::unbind()
{
	CAPP_GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void Cappuccino::Framebuffer::generateTextureAttachment()
{
	//generate the texture to bind to the framebuffer
	_colourBuffers.push_back(0);
	CAPP_GL_CALL(glGenTextures(1, &_colourBuffers.back()));
	CAPP_GL_CALL(glBindTexture(GL_TEXTURE_2D, _colourBuffers.back()));
	CAPP_GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, _windowSize.x, _windowSize.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL));
	CAPP_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	CAPP_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	CAPP_GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));

	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, handle, 0);
}

void Cappuccino::Framebuffer::attachTextures()
{
	GLenum e[] = { GL_COLOR_ATTACHMENT0 };
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _colourBuffers.back(), 0);
	//activate the buffers
	glDrawBuffers(1, e);
}

void Cappuccino::Framebuffer::generateRenderBufferAttachment(unsigned& handle)
{
	CAPP_GL_CALL(glGenRenderbuffers(1, &handle));
	CAPP_GL_CALL(glBindRenderbuffer(GL_RENDERBUFFER, handle));
	CAPP_GL_CALL(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, _windowSize.x, _windowSize.y));
	CAPP_GL_CALL(glBindRenderbuffer(GL_RENDERBUFFER, 0));
}
