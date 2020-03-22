#include "Cappuccino/UI.h"

#include "Cappuccino/CappMacros.h"
#include "Cappuccino/Mesh.h"
#include "Cappuccino/ResourceManager.h"

using namespace Cappuccino;


void UIComponent::updateComponent(float dt) {}
void UIComponent::drawComponent() {}

std::vector<UserInterface*> UserInterface::_allUI = {};

UserInterface::UserInterface()
{
	_allUI.push_back(this);
}

void UserInterface::update(const float dt)
{
	for (int i = static_cast<int>(_uiComponents.size() - 1); i >= 0; --i) {
		_uiComponents[i]->updateComponent(dt);
	}

}

void UserInterface::draw()
{
	for (int i = static_cast<int>(_uiComponents.size() - 1); i >= 0; --i) {
		_uiComponents[i]->drawComponent();
	}
}

// Text
UIText::UIText(const std::string& text, const glm::vec2& windowSize, const glm::vec2& defaultPosition, const glm::vec3& defaultColour, float defaultScale)
	: Text(text, _textShader, defaultPosition, defaultColour, defaultScale) {
	_textShader = *ShaderLibrary::loadShader("DefaultText", "font.vert", "font.frag");
	_windowSize = windowSize;
}

void UIText::drawComponent()
{
	if (isVisible())
		draw();
}

UIBar::UIBar(const glm::vec2& defaultPosition, const glm::vec4& defaultColour, const glm::vec3& barDimensions, OriginPoint point)
{
	_barShader = *ShaderLibrary::loadShader("DefaultBarUI", "screenSpaceModel.vert", "screenSpace.frag");

	_colour = defaultColour;
	_position = defaultPosition;
	_transform.translate(glm::vec3(_position.x, _position.y, 0));
	_barDimensions = barDimensions;
	_transform.scale(barDimensions, 1.0f);

	if (point == OriginPoint::BottomLeft)
		_barMesh = new Mesh("BottomLeftCube", "Cube3.obj");
	else if (point == OriginPoint::Middle)
		_barMesh = new Mesh("MiddleCube", "Cube2.obj");
	else if (point == OriginPoint::BottomRight)
		_barMesh = new Mesh("BottomRightCube", "Cube.obj");


	_barMesh->loadMesh();
}

void UIBar::updateComponent(float dt)
{
	_transform.update();
}

void UIBar::drawComponent()
{
	if (!isVisible())
		return;

	_barShader.use();

	_barShader.setUniform("colour", _colour);
	_barShader.loadOrthoProjectionMatrix(1600.0f,1000.0f);


	_barShader.loadModelMatrix(_transform._transformMat);

	_barMesh->draw();

}

Cappuccino::UIScreenQuad::UIScreenQuad(const std::vector<Cappuccino::Texture*>& textures)
{
	_quadShader = *ShaderLibrary::loadShader("DefaultUIQuad", "UIQuad.vert", "UIQuad.frag");
	_quadTextures = textures;

	_quadShader.use();
	_quadShader.setUniform("quadTexture1", 0);
	_quadShader.setUniform("quadTexture2", 1);
	_quadShader.setUniform("quadTexture3", 2);
	_quadShader.setUniform("quadTexture4", 3);

	float quadVertices[] = {
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};

	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

}

void Cappuccino::UIScreenQuad::drawComponent()
{
	if (!isVisible())
		return;
	glDisable(GL_DEPTH_TEST);
	_quadShader.use();
	
	for (unsigned i = 0; i < _quadTextures.size(); i++) 
		_quadTextures[i]->bind(i);
	

	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);

	_quadTextures.back()->unbind(0);
	_quadTextures.back()->unbind(1);
	_quadTextures.back()->unbind(2);
	_quadTextures.back()->unbind(3);
	
	glEnable(GL_DEPTH_TEST);
}
