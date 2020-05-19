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
#include "GameObject.h"
#include <glew.h>
#include "Texture.h"
#include <SOIL.h>
#include <vector>
#include <gtc/type_ptr.hpp>
#include "ShaderLoader.h"


class CubeMap : public Texture
{
	//Square 1
public:

	// ~Texture() = default;
	CubeMap(std::vector<const char*> textureDirs);
	
	void Update();

	void Render(Shader* shader, Mesh* mesh, Camera& camera);
	//GLuint GetID();

private:
	//GLuint textureID;
	// Position
	float m_xPos = 0.0f;
	float m_yPos = 0.0f;
	float m_zPos = 0.0f;

	// Scale
	float m_scale = 20000.0f;

	// Translation Matrix
	glm::vec3 m_objPosition = glm::vec3(m_xPos, m_yPos, m_zPos);
	glm::mat4 m_translationMatrix = glm::translate(glm::mat4(), m_objPosition);

	// Rotation
	glm::vec3 m_rotationAxisZ = glm::vec3(0.0f, 0.0f, 1.0f);
	float m_rotationAngle = 0.0f;
	glm::mat4 m_rotationZ = glm::rotate(glm::mat4(), glm::radians(m_rotationAngle), m_rotationAxisZ);

	// Scaling 
	glm::vec3 m_objScale = glm::vec3(1.0f * m_scale, 1.0f * m_scale, 1.0f * m_scale);
	glm::mat4 m_scaleMatrix = glm::scale(glm::mat4(), m_objScale);

	// Model matrix
	glm::mat4 m_modelMatrix = m_translationMatrix * m_rotationZ * m_scaleMatrix;

	 glm::mat4 m_MVP = m_modelMatrix;


	 GLuint m_VAO{ 0 };
	 GLuint m_EBO{ 0 };
	 GLuint m_VBO{ 0 };
	// Shader
	//Shader* m_shader;

	// Mesh
//	Mesh* m_mesh;

};

