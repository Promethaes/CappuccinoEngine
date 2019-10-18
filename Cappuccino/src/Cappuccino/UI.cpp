#include "Cappuccino/UI.h"

namespace Cappuccino {
	void UIComponent::updateComponent(float dt)
	{
	}
	void UIComponent::drawComponent()
	{
	}



	void UserInterface::update(float dt)
	{
		for (auto x : _uiComponents)
			x->updateComponent(dt);
		for (auto x : _uiComponents)
			x->drawComponent();
	}
	
	//Text
	UIText::UIText(const std::string& text, Shader& defaultShader, const glm::vec2& defaultPosition, const glm::vec3& defaultColour, float defaultScale)
		:Text(text,defaultShader,defaultPosition,defaultColour,defaultScale)
	{
	}

	void UIText::drawComponent()
	{
		draw();
	}
}