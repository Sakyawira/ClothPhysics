/***********************
  Bachelor of Software Engineering
  Media Design School
  Auckland
  New Zealand

  (c) 2018 Media Design School

  File Name   :   FrameBuffer.h
  Description :   declare the FrameBuffer class
  Author      :   Sakyawira Nanda Ruslim
  Mail        :   Sakyawira.Rus8080@mediadesign.school.nz
********************/
#pragma once
#include <glew.h>
#include <SOIL.h>
#include <vector>
#include <gtc/type_ptr.hpp>
#include "Shader.h"
#include "Mesh.h"

class FrameBuffer 
{
    public:
        FrameBuffer(Shader* _shader, Mesh* _mesh);
        ~FrameBuffer();

        void PrepareRender();
        void Render(const GLchar* s_currentTime, GLfloat f_currentTime);

    private:
        GLuint renderTexture;
        GLuint frameBuffer;
        GLuint rbo;

        Shader* m_shader;
        Mesh* m_mesh;
};