/***********************
  Bachelor of Software Engineering
  Media Design School
  Auckland
  New Zealand

  (c) 2018 Media Design School

  File Name   :   TessModel.cpp
  Description :   contains definition of TessModel.cpp
  Author      :   Sakyawira Nanda Ruslim
  Mail        :   Sakyawira.Rus8080@mediadesign.school.nz
********************/
#include "TessModel.h"

/***********************
* Constructor	: Pass in a point vertices, create VBO and VAO
* @parameter	: shader's program and camera
* @return		: -
***********************/
TessModel::TessModel(GLuint program, Camera* camera)
{
	this->program = program;
	this->camera = camera;
	GLfloat points[] = { -1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f, -1.0, 1.0, 0.0f }; 
	glPatchParameteri(GL_PATCH_VERTICES, 4); //comment for tri patch
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), &points, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0); 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
	glBindVertexArray(0);
}

/***********************
* render		: render the model
* @parameter	: position to render the model
* @return		: -
***********************/
void TessModel::render(glm::vec3 position)
{
	glUseProgram(this->program);

	//this->camera->use_camera(this->program);
	
	float camDistance = glm::distance(position, camera->get_position());
	GLint camDistanceLoc = glGetUniformLocation(program, "camDistance");
	glUniform1f(camDistanceLoc, camDistance);

	glm::mat4 model;
	model = glm::translate(model, position);
	glm::mat4 mvp = camera->get_projection() * camera->get_view() * model;
	GLint mvLoc = glGetUniformLocation(program, "mvp");
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvp));
	glBindVertexArray(VAO);
	glDrawArrays(GL_PATCHES, 0, 4);
	glBindVertexArray(0);
}
