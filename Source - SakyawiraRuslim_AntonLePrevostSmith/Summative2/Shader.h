/***********************
  Bachelor of Software Engineering
  Media Design School
  Auckland
  New Zealand

  (c) 2018 Media Design School

  File Name   :   Shader.h
  Description :   declare the Shader class which contains the program and control the uniform of an object
  Author      :   Sakyawira Nanda Ruslim
  Mail        :   Sakyawira.Rus8080@mediadesign.school.nz
********************/
#pragma once
#include <glew.h>
#include <vector>
#include <gtc/type_ptr.hpp>
#include "ShaderLoader.h"
#include "Texture.h"
#include "Camera.h"

class Shader
{
	//Square 1
public:
	Shader(const char* VertexShaderFilename, const char* FragmentShaderFilename, std::vector<Shader*>& shaderVector);
	Shader(const char* VertexShaderFilename, const char* FragmentShaderFilename, const char* geometryShaderFilename, std::vector<Shader*>& shaderVector);
	Shader(const char* VertexShaderFilename, const char* FragmentShaderFilename, const char* TessControlShaderFilename, const char* TessEvalShaderFilename, std::vector<Shader*>& shaderVector);
	~Shader() = default;
	void Transform(glm::mat4 translationMatrix, glm::mat4 rotationZ, glm::mat4 scaleMatrix, glm::mat4 modelMatrix, Camera& camera);
	// void PassTexture(Texture* v_texture);
	void PassTexture(std::vector<Texture*> v_texture);
	void PassUniform(const GLchar* name, GLfloat _float);
	void PassFrameUniform(const GLchar * name, GLint _int);
	void Activate();
	GLuint GetProgram();

private:
	GLuint m_program;
	
};

