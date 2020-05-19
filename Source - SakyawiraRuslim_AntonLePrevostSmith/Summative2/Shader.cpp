/***********************
  Bachelor of Software Engineering
  Media Design School
  Auckland
  New Zealand

  (c) 2018 Media Design School

  File Name   :   Shader.cpp
  Description :   define the Shader class' constructor and functions
  Author      :   Sakyawira Nanda Ruslim
  Mail        :   Sakyawira.Rus8080@mediadesign.school.nz
********************/
#include "Shader.h"


Shader::Shader(const char * VertexShaderFilename, const char * FragmentShaderFilename, std::vector<Shader*>& shaderVector)
{
	m_program = ShaderLoader::CreateProgram(VertexShaderFilename, FragmentShaderFilename);
	shaderVector.push_back(this);
}

Shader::Shader(const char* VertexShaderFilename, const char* FragmentShaderFilename, const char* geometryShaderFilename, std::vector<Shader*>& shaderVector)
{
	m_program = ShaderLoader::CreateProgram(VertexShaderFilename, FragmentShaderFilename, geometryShaderFilename);
	shaderVector.push_back(this);
}

Shader::Shader(const char* VertexShaderFilename, const char* FragmentShaderFilename, const char* TessControlShaderFilename, const char* TessEvalShaderFilename, std::vector<Shader*>& shaderVector)
{
	m_program = ShaderLoader::CreateProgram(VertexShaderFilename, FragmentShaderFilename, TessControlShaderFilename, TessEvalShaderFilename);
	shaderVector.push_back(this);
}

void Shader::Transform(glm::mat4 translationMatrix, glm::mat4 rotationZ, glm::mat4 scaleMatrix, glm::mat4 modelMatrix, Camera& camera )
{
	//// Put translation uniform into '*program'
	//GLuint translateLoc = glGetUniformLocation(m_program, "translation");
	//glUniformMatrix4fv(translateLoc, 1, GL_FALSE, glm::value_ptr(translationMatrix));

	//// Put rotation uniform into '*program'
	//GLuint rotationLoc = glGetUniformLocation(m_program, "rotation");
	//glUniformMatrix4fv(rotationLoc, 1, GL_FALSE, glm::value_ptr(rotationZ));

	//// Put scaling uniform into '*program'
	//GLuint scaleLoc = glGetUniformLocation(m_program, "scale");
	//glUniformMatrix4fv(scaleLoc, 1, GL_FALSE, glm::value_ptr(scaleMatrix));

	GLuint modelLoc = glGetUniformLocation(m_program, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	// Put model uniform into '*program'
	GLuint MVPLoc = glGetUniformLocation(m_program, "MVP");
	glUniformMatrix4fv(MVPLoc, 1, GL_FALSE, glm::value_ptr((camera.get_vp() * modelMatrix)));

	GLuint camLoc = glGetUniformLocation(m_program, "camPos");
	glUniform3fv(camLoc, 1, glm::value_ptr(camera.get_position()));
}

void Shader::PassTexture(std::vector<Texture*> v_texture)
{
	std::string textureUniform = "tex";
	unsigned int textureNumber = 0;
	// rendering texture "texture"
	for (auto texture: v_texture)
	{
		glActiveTexture(GL_TEXTURE0 + textureNumber);
		glBindTexture(GL_TEXTURE_2D, texture->GetID());
		const GLchar* name = textureUniform.c_str();
		glUniform1i(glGetUniformLocation(m_program, name), textureNumber);
		textureNumber++;
		textureUniform += std::to_string(textureNumber);
	}

	//// Get texture to render
	//glActiveTexture(GL_TEXTURE1);
	//glBindTexture(GL_TEXTURE_2D, v_texture2->GetID());
	//glUniform1i(glGetUniformLocation(m_program, "tex1"), 1);
}

void Shader::PassUniform(const GLchar * name, GLfloat _float)
{
	GLint currentTimeLoc = glGetUniformLocation(m_program, name);
	glUniform1f(currentTimeLoc, _float);
}

void Shader::PassFrameUniform(const GLchar * name, GLint _int)
{
	GLint frameCountsLoc = glGetUniformLocation(m_program, name);
	glUniform1i(frameCountsLoc, _int);
}

void Shader::Activate()
{
	glUseProgram(m_program);
}

GLuint Shader::GetProgram()
{
	return m_program;
}
