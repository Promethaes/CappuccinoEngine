#include "Cappuccino/UI.h"

#include "Cappuccino/CappMacros.h"
#include "Cappuccino/Mesh.h"

using namespace Cappuccino;

void UIComponent::updateComponent(float dt) {}
void UIComponent::drawComponent() {}

void UserInterface::update(float dt)
{
	for(int i = _uiComponents.size() - 1; i >= 0; --i) {
		_uiComponents[i]->updateComponent(dt);
	}
	for(int i = _uiComponents.size() - 1; i >= 0; --i) {
		_uiComponents[i]->drawComponent();
	}
}
	
//Text
UIText::UIText(const std::string& text, const glm::vec2& windowSize, Shader& defaultShader, const glm::vec2& defaultPosition, const glm::vec3& defaultColour, float defaultScale)
	:Text(text,defaultShader,defaultPosition,defaultColour,defaultScale)
{
	_windowSize = windowSize;
}

void UIText::drawComponent()
{
	draw();
}

UIBar::UIBar(const glm::vec2& defaultPosition, const glm::vec4& defaultColour, const glm::vec3& barDimensions, OriginPoint point)
{
		
	_colour = defaultColour;
	_position = defaultPosition;
	_transform.translate(glm::vec3(_position.x, _position.y, 0));
	_barDimensions = barDimensions;
	_transform.scale(barDimensions, 1.0f);

	if(point == OriginPoint::BottomLeft)
		_barMesh = new Mesh("Cube3.obj");
	else if(point == OriginPoint::Middle)
		_barMesh = new Mesh("Cube2.obj");
	else if(point == OriginPoint::BottomRight)
		_barMesh = new Mesh("Cube.obj");


	_barMesh->loadMesh();
}

void UIBar::updateComponent(float dt)
{
	_transform.update();
}

void UIBar::drawComponent()
{
	_barShader.use();
		
	_barShader.setUniform("colour", _colour);
	_barShader.loadOrthoProjectionMatrix(1600.0f / 20, 1200.0f / 20);
		

	_barShader.loadModelMatrix(_transform._transformMat);
		
	_barMesh->draw();

}