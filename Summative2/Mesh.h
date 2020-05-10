/***********************
  Bachelor of Software Engineering
  Media Design School
  Auckland
  New Zealand

  (c) 2018 Media Design School

  File Name   :   Mesh.h
  Description :   declare the Mesh class which contains the VAO, VBO, and EBO of an object
  Author      :   Sakyawira Nanda Ruslim
  Mail        :   Sakyawira.Rus8080@mediadesign.school.nz
********************/
#pragma once
#include <glew.h>
#include <vector>
#include <gtc/type_ptr.hpp>

class Mesh
{
	//Square 1
public:
	Mesh() = default;
	Mesh(const std::vector<GLuint>& indices, const std::vector<GLfloat>& vertices, std::vector<Mesh*>& meshVector);
	virtual ~Mesh() = default;

	void Bind();
	int GetSize();

	std::vector<GLfloat> GetVertices();

protected:

	GLuint m_VAO{0};
	GLuint m_EBO{0};
	GLuint m_VBO{0};

	// These are copies, they can not be used to control the actual one
	std::vector<GLfloat> m_vertices;

	int m_indicesSize;

	// These are copies
	//const std::vector<GLuint> m_indices;
};
