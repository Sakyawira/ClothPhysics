/***********************
  Bachelor of Software Engineering
  Media Design School
  Auckland
  New Zealand

  (c) 2018 Media Design School

  File Name   :   GeometryModel.cpp
  Description :   contains definition of GeometryModel.cpp
  Author      :   Sakyawira Nanda Ruslim
  Mail        :   Sakyawira.Rus8080@mediadesign.school.nz
********************/
#include "GeometryModel.h"

/***********************
* Constructor	: Pass in a point vertices, create VBO and VAO
* @parameter	: shader's program and camera
* @return		: -
***********************/
GeometryModel::GeometryModel(GLuint program, Camera* camera) 
{
	this->program = program; 
	this->camera = camera;
	GLfloat points[] = 
	{ 
		0.0f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f //passing in 1 point 
	};
	glBindVertexArray(m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenVertexArrays(1, &m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), &points, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(
		2,
		2,									// 2 float components for coordinates 
		GL_FLOAT,
		GL_FALSE,
		8 * sizeof(GLfloat),				// Stride the single vertex (pos + color + tex)
		(GLvoid*)(6 * sizeof(GLfloat)));	// offset from beginning of Vertex

	glBindVertexArray(0);
}

/***********************
* render		: render the model
* @parameter	: position to render the model
* @return		: -
***********************/
void GeometryModel::render(glm::vec3 position, Texture* _texture)
{
	glUseProgram(this->program);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _texture->GetID());
	const GLchar* name = "tex";
	glUniform1i(glGetUniformLocation(program, name), 0);

	glm::mat4 model;
	model = glm::translate(model, position);
	glm::mat4 mvp = camera->get_projection() * camera->get_view() * model;
	GLint vpLoc = glGetUniformLocation(program, "mvp");
	glUniformMatrix4fv(vpLoc, 1, GL_FALSE, glm::value_ptr(mvp));

	glBindVertexArray(m_VAO);
	glDrawArrays(GL_POINTS, 0, 1);
	glBindVertexArray(0);
}
