#include "Cloth.h"

Cloth::Cloth()
{

}

void Cloth::Initialize()
{
	m_fWidth = _width;
	m_fHeight = _height;
	//Reset particles and constraints
	m_fParticlesWidth = _numParticlesWidth;
	m_fParticlesHeight = _numParticlesHeight;

	m_vParticles.clear();
	m_vConstraints.clear();

	m_vParticles.resize(_numParticlesWidth * _numParticlesHeight);

	int verticesID = 0;
	// creating particles in a grid of particles from (0,0,0) to (width,-height,0)
	for (int y = 0; y < _numParticlesWidth; y++)
	{
		for (int x = 0; x < _numParticlesHeight; x++)
		{
			float xOffset = 0.0f;

			//Give x particles an offsetted x position .
			//so it doesnt start off as a flat cloth
			if (x % 2 == 0)
			{
				xOffset = -0.2f;
			}
			else
			{
				xOffset = 0.2f;
			}

			glm::vec3 pos = glm::vec3(_width * (x / (float)_numParticlesWidth) + _pos.x,
				-_height * (y / (float)_numParticlesHeight) + _pos.y,
				_pos.z + xOffset);
			m_vParticles[y * _numParticlesWidth + x] = Particle(pos); // insert particle in column x at y'th row

			//Set vertices id and vertices for each particle
			m_vParticles[y * _numParticlesWidth + x].SetVertID(verticesID);
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

	// Make constraints for each square
	for (int x = 0; x < _numParticlesWidth; x++)
	{
		for (int y = 0; y < _numParticlesHeight; y++)
		{
			if (x < _numParticlesWidth - 1)
			{
				MakeConstraint(GetParticle(x, y), GetParticle(x + 1, y));
			}
			if (y < _numParticlesHeight - 1)
			{
				MakeConstraint(GetParticle(x, y), GetParticle(x, y + 1));
			}
			if (x < _numParticlesWidth - 1 && y < _numParticlesHeight - 1)
			{
				MakeConstraint(GetParticle(x, y), GetParticle(x + 1, y + 1));
			}
			if (x < _numParticlesWidth - 1 && y < _numParticlesHeight - 1)
			{
				MakeConstraint(GetParticle(x + 1, y), GetParticle(x, y + 1));
			}
		}
	}

	// Set pins at top of the cloth
	float offsetX = -1;
	for (int i = 0; i < _numParticlesWidth; i++)
	{
		GetParticle(0 + i, 0)->SetPinned(true);
	}
	GenerateBuffers();
}
