/***********************
  Bachelor of Software Engineering
  Media Design School
  Auckland
  New Zealand

  (c) 2018 Media Design School

  File Name   :   Texture.h
  Description :   declare the Texture class
  Author      :   Sakyawira Nanda Ruslim
  Mail        :   Sakyawira.Rus8080@mediadesign.school.nz
********************/
#pragma once
#include <glew.h>
#include <SOIL.h>
#include <vector>
#include <gtc/type_ptr.hpp>
#include "ShaderLoader.h"

class Texture
{
	//Square 1
public:
	Texture() = default;
	Texture(const char * textureDir);
	~Texture() = default;
	GLuint GetID();

protected:
	GLuint textureID;

};

