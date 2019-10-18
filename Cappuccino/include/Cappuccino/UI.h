#pragma once
#include "Cappuccino/FontManager.h"
#include <vector>

namespace Cappuccino {

	/*
	base class for your UI elements
	*/
	class UIComponent {
	public:
		/*
		Purp: base class function with no definition, some components may not need an update function
		*/
		virtual void updateComponent(float dt);

		/*
		Purp: base class draw function, has no definition
		*/
		virtual void drawComponent();

	};

	class UserInterface {
	public:

		void update(float dt);

		std::vector<UIComponent*> _uiComponents;
	};

	//UI components
	
	class UIText : public UIComponent,public Text {
	public:
		UIText(const std::string& text, Shader& defaultShader, const glm::vec2& defaultPosition, const glm::vec3& defaultColour, float defaultScale);

		void drawComponent() override;
	};
}