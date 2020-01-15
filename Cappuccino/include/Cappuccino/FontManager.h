#pragma once
#include "ft2build.h"
#include FT_FREETYPE_H

#include <string>
#include <optional>
#include <map>

#include "glm/common.hpp"

#include "Cappuccino/ShaderProgram.h"

namespace Cappuccino {
	//https://learnopengl.com/In-Practice/Text-Rendering
	class Character {
	public:
		unsigned _textureId;
		glm::ivec2 _size;
		glm::ivec2 _bearing;
		unsigned _advance;

		static std::map<char, Character> _characters;
	};
	class FontManager {
	public:

		static bool init(const std::string& defaultPath);

		static void setDefaultPath(const std::string& defaultPath);

		static void loadTypeFace(const std::string& relativePath);
		static void exit();

		static FT_Library _lib;
	private:
		static std::string _typefacePath;
		static bool _initialized;
	};
	class Text {
	public:
		/*
		PASSING IN A SHADER WILL NOT DO ANYTHING
		*/
		Text(const std::string& text, Shader& defaultShader,const glm::vec2& defaultPosition, const glm::vec3& defaultColour, float defaultScale);

		void draw();

		void setText(const std::string& newText) { _text = newText; }
		void changeTextShader(Shader& newShader) { _textShader = &newShader; }
		void setTextColour(const glm::vec3& newColour) { _colour = newColour; }
		void setTextPosition(const glm::vec2& newPosition) { _position = newPosition; }
		void setScale(float newScale) { _scale = newScale; }

		glm::vec3 getColour() const { return _colour; }
		glm::vec2& getPosition() { return _position; }
		std::string& getText() { return _text; }
	protected:
		glm::vec2 _windowSize;
		float _scale;
		glm::vec2 _position;
		glm::vec3 _colour;
		Shader* _textShader;
		std::string _text;
	private:
		unsigned _VAO, _VBO;
	};
}