/***********************
  Bachelor of Software Engineering
  Media Design School
  Auckland
  New Zealand

  (c) 2018 Media Design School

  File Name   :   TextLabel.h
  Description :   declare the TextLabel class' which controls displaying of texts and its properties
  Author      :   Sakyawira Nanda Ruslim
  Mail        :   Sakyawira.Rus8080@mediadesign.school.nz
********************/
#pragma once

// Dependency Includes
#include <glew.h>
#include <freeglut.h>
#include <SOIL.h>
#include <glm.hpp>
#include <fmod.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

// Library Includes
#include <iostream>
#include <vector>
#include <assert.h>

// Local Includes
#include "ShaderLoader.h"
#include "Camera.h"
#include "glm.hpp"

struct FontChar
{
	GLuint TextureID; // Texture ID
	glm::ivec2 Size; // Size of the glyph
	glm::ivec2 Bearing; // Offset of the glyph (top left) from the baseline
	GLuint Advance;
};

class TextLabel
{
	public:
		TextLabel(
			int _WINDOW_WIDTH,
			int _WINDOW_HEIGHT,
			std::string newText,
			std::string newFont,
			glm::vec2 newPos,
			std::vector<TextLabel*>& textVector,
			glm::vec3 newColor = glm::vec3(1.0f, 1.0f, 1.0f),
			float scale = 1.0f
		);
	
		~TextLabel() {};
	
		void Render();
		void SetText(std::string newText) { text = newText; };
		void SetColor(glm::vec3 newColor) { color = newColor; };
		void SetScale(GLfloat newScale) { scale = newScale; };
		void SetPosition(glm::vec2 newPosition) { position = newPosition; };
	
	private:
		GLuint GenerateTexture(FT_Face face);

		std::string text;
		GLfloat scale;
		glm::vec3 color;
		glm::vec2 position;

		GLuint VAO, VBO, program;
		glm::mat4 proj;
		std::map<GLchar, FontChar> Characters;
};