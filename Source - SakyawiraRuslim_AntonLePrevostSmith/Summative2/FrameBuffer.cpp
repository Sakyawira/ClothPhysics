/***********************
  Bachelor of Software Engineering
  Media Design School
  Auckland
  New Zealand

  (c) 2018 Media Design School

  File Name   :   FrameBuffer.cpp
  Description :   contains definition of FrameBuffer
  Author      :   Sakyawira Nanda Ruslim
  Mail        :   Sakyawira.Rus8080@mediadesign.school.nz
********************/
#include "FrameBuffer.h"

/***********************
* Constructor	: Create the frame buffer and the render texture, set shader and mesh pointer
* @parameter	: shader point and mesh pointer
* @return		: -
***********************/
FrameBuffer::FrameBuffer(Shader* _shader, Mesh* _mesh)
{
	m_shader = _shader;
	m_mesh = _mesh;

	// Create frame buffer
	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

	// Create render texture
	glGenTextures(1, &renderTexture);
	glBindTexture(GL_TEXTURE_2D, renderTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 800, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Attach texture to framebuffer object
	glFramebufferTexture2D(GL_FRAMEBUFFER, // target buffer 
		GL_COLOR_ATTACHMENT0, // attachment, could be //GL_DEPTH_ATTACHMENT or //GL_STENCIL_ATTACHMENT 
		GL_TEXTURE_2D, // texture target type
		renderTexture, // texture 
		0);

	// Create render buffer Object
	glGenRenderbuffers(1, &rbo); 
	glBindRenderbuffer(GL_RENDERBUFFER, rbo); 
	glRenderbufferStorage(GL_RENDERBUFFER, 
		// must be 
		GL_DEPTH24_STENCIL8, //use as depth - stencil buffer 
		800, 
		800) //viewport width and height
		;
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	// Attach rbo to the depth and stencil attachment of the frame buffer
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, //target 
		GL_DEPTH_STENCIL_ATTACHMENT, //attachment 
		GL_RENDERBUFFER, //renderbufferTarget 
		rbo); // render buffer

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) 
	{ 
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl; 
	}
}

FrameBuffer::~FrameBuffer()
{

}

/***********************
* PrepareRender: Everything drawn under this function wil be drawn to the frame buffer
* @parameter: -
* @return: -
***********************/
void FrameBuffer::PrepareRender()
{
	// Bind our framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

	glClearColor(0.5, 0.5, 0.5, 1.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST); // Depth is enabled to capture it
}

/***********************
* Render: Render the FrameBuffer
* @parameter: s_currentTime-> uniform name for current time, f_currentTime-> current time uniform
* @return: -
***********************/
void FrameBuffer::Render(const GLchar* s_currentTime, GLfloat f_currentTime)
{
	glm::mat4 position =  glm::translate(glm::mat4(), glm::vec3(0.0f, -1.0f, 0.0f));

	// Bind Default framebuffer 
	glBindFramebuffer(GL_FRAMEBUFFER, 0); 

	glClearColor(0.5, 0.5, 0.5, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	// glUseProgram(program);
	m_shader->Activate();
	m_shader->PassUniform(s_currentTime, f_currentTime);

	GLuint translateLoc = glGetUniformLocation(m_shader->GetProgram(), "translation");
	glUniformMatrix4fv(translateLoc, 1, GL_FALSE, glm::value_ptr(position));

	m_mesh->Bind();

	glDisable(GL_DEPTH_TEST); //screenspace quad so depth is not required

	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(m_shader->GetProgram(), "renderTexture"), 0);
	glBindTexture(GL_TEXTURE_2D, renderTexture);
	// glDrawArrays(GL_TRIANGLES, 0, 6);
	glDrawElements(GL_TRIANGLES, m_mesh->GetSize(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
