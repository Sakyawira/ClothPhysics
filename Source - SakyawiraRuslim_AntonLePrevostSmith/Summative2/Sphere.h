#pragma once

#include <glew.h>
#include <freeglut.h>

#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>

#include "Mesh.h"

class Sphere : public Mesh
{
public:
	Sphere();
	~Sphere();

	/*void Render();*/
	 
private:
	//GLuint m_VAO{ 0 };
	//GLuint EBO{ 0 };
	//GLuint VBO{ 0 };

	// GLuint IndiceCount;
	int DrawType;
};

