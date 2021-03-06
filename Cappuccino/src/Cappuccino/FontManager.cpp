#include "Cappuccino/FontManager.h"
#include "Cappuccino/CappMacros.h"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

namespace Cappuccino {

	std::map<char, Character> Character::_characters;
	FT_Library FontManager::_lib;
	bool FontManager::_initialized = false;
	std::string FontManager::_typefacePath = "";
	
	bool FontManager::init(const std::string& defaultPath)
	{
		if (_initialized)
			return true;
		_typefacePath = defaultPath;
		if (FT_Init_FreeType(&_lib)) {
			CAPP_PRINT_ERROR("Failed to initialize FreeType!");
			return false;
		}

		_initialized = true;
		return true;

	}

	void FontManager::setDefaultPath(const std::string& defaultPath)
	{
		_typefacePath = defaultPath;
	}

	void FontManager::loadTypeFace(const std::string& relativePath)
	{
		FT_Face face;

		if (FT_New_Face(_lib, std::string(_typefacePath + relativePath).c_str(), 0, &face)) {
			CAPP_PRINT_ERROR("Failed to load typeface %s", relativePath.c_str());
			return;
		}

		FT_Set_Pixel_Sizes(face, 0, 48);
		//this code is NOT ours, it belongs to https://learnopengl.com/In-Practice/Text-Rendering

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction

		for (GLubyte i = 0; i < 128; i++)
		{
			// Load character glyph 
			if (FT_Load_Char(face, i, FT_LOAD_RENDER))
			{
				CAPP_PRINT_ERROR("Failed to load glyph %i", i);
				continue;
			}
			// Generate texture
			GLuint texture;
			CAPP_GL_CALL(glGenTextures(1, &texture));
			CAPP_GL_CALL(glBindTexture(GL_TEXTURE_2D, texture));
			CAPP_GL_CALL(glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RED,
				face->glyph->bitmap.width,
				face->glyph->bitmap.rows,
				0,
				GL_RED,
				GL_UNSIGNED_BYTE,
				face->glyph->bitmap.buffer
			));
			// Set texture options
			CAPP_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
			CAPP_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
			CAPP_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
			CAPP_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
			// Now store character for later use
			Character character = {
				texture,
				glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
				glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
				face->glyph->advance.x
			};
			Character::_characters.insert(std::pair<GLchar, Character>(i, character));
		}
		FT_Done_Face(face);

	}

	void FontManager::exit()
	{
		FT_Done_FreeType(_lib);
	}

	Text::Text(const std::string& text, Shader& defaultShader, const glm::vec2& defaultPosition, const glm::vec3& defaultColour, float defaultScale)
	{
		_text = text;
		_textShader = &defaultShader;
		_position = defaultPosition;
		_colour = defaultColour;
		_scale = defaultScale;
		

		CAPP_GL_CALL(glGenVertexArrays(1, &_VAO));
		CAPP_GL_CALL(glGenBuffers(1, &_VBO));
		CAPP_GL_CALL(glBindVertexArray(_VAO));
		CAPP_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, _VBO));
		CAPP_GL_CALL(glBufferData(GL_ARRAY_BUFFER, (sizeof(GLfloat) * 6) * 4, NULL, GL_DYNAMIC_DRAW));
		CAPP_GL_CALL(glEnableVertexAttribArray(0));
		CAPP_GL_CALL(glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0));
		CAPP_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
		CAPP_GL_CALL(glBindVertexArray(0));
	}

	void Text::draw()
	{
		

		CAPP_GL_CALL(glActiveTexture(GL_TEXTURE0));
		CAPP_GL_CALL(glBindVertexArray(_VAO));
		
		_textShader->use();
		_textShader->loadOrthoProjectionMatrix(_windowSize.x,_windowSize.y);
		_textShader->setUniform("textColour", _colour);
#if UITEST
		_textShader->use();
		_textShader->loadOrthoProjectionMatrix(1600.0f, 1200.0f);
		_textShader->setUniform("textColour", _colour);
#endif
		
		auto tempPos = _position;
		std::string::const_iterator c;
		for (c = _text.begin(); c != _text.end(); c++) {
			Character ch = Character::_characters[*c];
			GLfloat xpos = tempPos.x + ch._bearing.x * _scale;
			GLfloat ypos = tempPos.y - (ch._size.y - ch._bearing.y) * _scale;

			GLfloat w = ch._size.x * _scale;
			GLfloat h = ch._size.y * _scale;
			// Update VBO for each character
			GLfloat vertices[6][4] = {
				{ xpos,     ypos + h,   0.0, 0.0 },
				{ xpos,     ypos,       0.0, 1.0 },
				{ xpos + w, ypos,       1.0, 1.0 },

				{ xpos,     ypos + h,   0.0, 0.0 },
				{ xpos + w, ypos,       1.0, 1.0 },
				{ xpos + w, ypos + h,   1.0, 0.0 }
			};
			// Render glyph texture over quad
			CAPP_GL_CALL(glBindTexture(GL_TEXTURE_2D, ch._textureId));
			// Update content of VBO memory
			CAPP_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, _VBO));
			CAPP_GL_CALL(glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices));
			CAPP_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
			// Render quad
			CAPP_GL_CALL(glDrawArrays(GL_TRIANGLES, 0, 6));
			// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
			tempPos.x += (ch._advance >> 6)* _scale; // Bitshift by 6 to get value in pixels (2^6 = 64)
		}
		CAPP_GL_CALL(glBindVertexArray(0));
		CAPP_GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));
	}


}