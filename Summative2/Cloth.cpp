#include "Cloth.h"

Cloth::Cloth()
{

}

void Cloth::Initialize(float _width, float _height, int _numParticlesX, int _numParticlesY, glm::vec3 _pos)
{
	// Reset particles and constraints
	m_fParticlesInX = _numParticlesX;
	m_fParticlesInY = _numParticlesY;

	m_vParticles.clear();
	m_vConstraints.clear();

	m_vParticles.resize(_numParticlesX * _numParticlesY);

	int verticesID = 0;
	
	// Creates particles in a grid of particles from (0,0,0) to (width,-height,0)
	for (int y = 0; y < _numParticlesX; y++)
	{
		for (int x = 0; x < _numParticlesY; x++)
		{
			float xOffset = 0.0f;

			// Give x particles an offsetted x position .
			// so it doesnt start off as a flat cloth
			if (x % 2 == 0)
			{
				xOffset = -0.2f;
			}
			else
			{
				xOffset = 0.2f;
			}

			glm::vec3 pos = glm::vec3(_width * (x / (float)_numParticlesX) + _pos.x,
				-_height * (y / (float)_numParticlesY) + _pos.y,
				_pos.z + xOffset);
			m_vParticles[y * _numParticlesX + x] = Particle(pos); // insert particle in column x at y'th row

			// Set vertices id and vertices for each particle
			m_vParticles[y * _numParticlesX + x].SetVertID(verticesID);
			++verticesID;

			//Position values
			m_fVerticesPoints.push_back(pos.x);
			m_fVerticesPoints.push_back(pos.y);
			m_fVerticesPoints.push_back(pos.z);

			//Normal values
			m_fVerticesPoints.push_back(0.0f);
			m_fVerticesPoints.push_back(1.0f);
			m_fVerticesPoints.push_back(1.0f);
		}
	}

	// Create constraints for each square
	for (int x = 0; x < _numParticlesX; x++)
	{
		for (int y = 0; y < _numParticlesY; y++)
		{
			if (x < _numParticlesX - 1)
			{
				CreateConstraint(GetParticle(x, y), GetParticle(x + 1, y));
			}
			if (y < _numParticlesY - 1)
			{
				CreateConstraint(GetParticle(x, y), GetParticle(x, y + 1));
			}
			if (x < _numParticlesX - 1 && y < _numParticlesY - 1)
			{
				CreateConstraint(GetParticle(x, y), GetParticle(x + 1, y + 1));
			}
			if (x < _numParticlesX - 1 && y < _numParticlesY - 1)
			{
				CreateConstraint(GetParticle(x + 1, y), GetParticle(x, y + 1));
			}
		}
	}

	// Set pins at top of the cloth
	float offsetX = -1;
	for (int i = 0; i < _numParticlesX; i++)
	{
		GetParticle(0 + i, 0)->SetPin(true);
	}
	GenerateBuffers();
}

void Cloth::GenerateBuffers()
{
	m_iIndicesPoints.clear();
	for (auto& constraint : m_vConstraints)
	{
		if (!constraint.GetDestroyed() || constraint.GetParticle2() - constraint.GetParticle1() < 5.0f)
		{
			m_iIndicesPoints.push_back(constraint.GetParticle1()->GetVertID());
			m_iIndicesPoints.push_back(constraint.GetParticle2()->GetVertID());
		}
	}

	//Generating Buffers and Arrays
	glGenVertexArrays(1, &m_VAO);					//Vert Array
	glBindVertexArray(m_VAO);

	glGenBuffers(1, &m_VBO);                        //Vert Buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

	glGenBuffers(1, &m_EBO);						//Index Buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);

	glBufferData(GL_ARRAY_BUFFER, m_fVerticesPoints.size() * sizeof(GLfloat), &m_fVerticesPoints[0], GL_DYNAMIC_DRAW);			//VBO Buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_iIndicesPoints.size() * sizeof(GLuint), &m_iIndicesPoints[0], GL_DYNAMIC_DRAW);    //EBO Buffer
	m_indicesSize = m_iIndicesPoints.size();

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
}

void Cloth::CreateConstraint(Particle* p1, Particle* p2)
{

}
