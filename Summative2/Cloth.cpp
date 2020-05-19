#include "Cloth.h"

#include <algorithm>
#include <random>

Cloth::Cloth(GLuint program, int _numParticlesX, int _numParticlesY)
{
	m_program = program;
	m_fParticlesInX = _numParticlesX;
	m_fParticlesInY = _numParticlesY;
}

void Cloth::Initialize(float _width, float _height, glm::vec3 _pos)
{
	m_objPosition = _pos;

	// Reset particles and constraints
	m_vParticles.clear();
	m_vConstraints.clear();

	m_vParticles.resize(m_fParticlesInX * m_fParticlesInY);

	int verticesID = 0;
	
	// Creates Cloth with a size of _width times _height
	// Populate that Cloth with m_fParticlesInX times m_fParticlesInY number of Particles
	for (int y = 0; y < m_fParticlesInY; y++)
	{
		const int iIndexOffset = y * m_fParticlesInX;
		for (int x = 0; x < m_fParticlesInX; x++)
		{
			// Create a new position for a new particle
			glm::vec3 pos = glm::vec3(_width * (x / (float)m_fParticlesInX) + _pos.x,
									 -_height * (y / (float)m_fParticlesInY) + _pos.y,
									 _pos.z);

			// Create and insert a new particle in column x, row y
			m_vParticles[iIndexOffset + x] = Particle(pos); 

			// Set vertices id and vertices for each particle
			m_vParticles[iIndexOffset + x].SetVertexId(verticesID);
			++verticesID;

			// Add a 'Position' attribute for the cloth's mesh
			m_vertices.push_back(pos.x);
			m_vertices.push_back(pos.y);
			m_vertices.push_back(pos.z);

			// Add a 'Normal' attribute for the cloth's mesh
			m_vertices.push_back(0.0f);
			m_vertices.push_back(1.0f);
			m_vertices.push_back(1.0f);
		}
	}

	// Create constraints for each square
	for (int x = 0; x < m_fParticlesInX; x++)
	{
		for (int y = 0; y < m_fParticlesInY; y++)
		{
			// Cloth base constraints
			if (x < m_fParticlesInX - 1)
			{
				CreateConstraint(GetParticle(x, y), GetParticle(x + 1, y));
			}
			if (y < m_fParticlesInY - 1)
			{
				CreateConstraint(GetParticle(x, y), GetParticle(x, y + 1));
			}
			if (x < m_fParticlesInX - 1 && y < m_fParticlesInY - 1)
			{
				CreateConstraint(GetParticle(x, y), GetParticle(x + 1, y + 1));
			}
			if (x < m_fParticlesInX - 1 && y < m_fParticlesInY - 1)
			{
				CreateConstraint(GetParticle(x + 1, y), GetParticle(x, y + 1));
			}

			// Cloth folding constraints (2 apart)
			if (x < m_fParticlesInX - 2)
			{
				CreateConstraint(GetParticle(x, y), GetParticle(x + 2, y), true);
			}
			if (y < m_fParticlesInY - 2)
			{
				CreateConstraint(GetParticle(x, y), GetParticle(x, y + 2), true);
			}
			if (x < m_fParticlesInX - 2 && y < m_fParticlesInY - 2)
			{
				CreateConstraint(GetParticle(x, y), GetParticle(x + 2, y + 2), true);
			}
			if (x < m_fParticlesInX - 2 && y < m_fParticlesInY - 2)
			{
				CreateConstraint(GetParticle(x + 2, y), GetParticle(x, y + 2), true);
			}

			// Cloth folding constraints (3 apart)
			if (x < m_fParticlesInX - 3)
			{
				CreateConstraint(GetParticle(x, y), GetParticle(x + 3, y), true);
			}
			if (y < m_fParticlesInY - 3)
			{
				CreateConstraint(GetParticle(x, y), GetParticle(x, y + 3), true);
			}
			if (x < m_fParticlesInX - 3 && y < m_fParticlesInY - 3)
			{
				CreateConstraint(GetParticle(x, y), GetParticle(x + 3, y + 3), true);
			}
			if (x < m_fParticlesInX - 3 && y < m_fParticlesInY - 3)
			{
				CreateConstraint(GetParticle(x + 3, y), GetParticle(x, y + 3), true);
			}
		}
	}

	// Pin the Top Left and Top Right Particle
	GetParticle(0, 0)->SetPin(true);
	GetParticle(m_fParticlesInX - 1, 0)->SetPin(true);
	GenerateBuffers();
}

void Cloth::Unpin()
{
	// Unpin pins at top of the cloth
	for (int i = 0; i < m_fParticlesInX; i++)
	{
		GetParticle(0 + i, 0)->SetPin(false);
	}
}

void Cloth::GenerateBuffers()
{
	m_indices.clear();
	for (auto& constraint : m_vConstraints)
	{
		if (!constraint.GetIsAlive() || constraint.GetParticle2() - constraint.GetParticle1() < 5.0f)
		{
			m_indices.push_back(constraint.GetParticle1()->GetVertexId());
			m_indices.push_back(constraint.GetParticle2()->GetVertexId());
		}
	}

	// Generating Vertex Array
	glGenVertexArrays(1, &m_VAO);					
	glBindVertexArray(m_VAO);

	// Generating Vertex Buffer
	glGenBuffers(1, &m_VBO);                        
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

	// Generating Index Buffer
	glGenBuffers(1, &m_EBO);						
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);

	// Assign VBO Buffer Data
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(GLfloat), &m_vertices.front(), GL_DYNAMIC_DRAW);
	// Assign EBO Buffer Data
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(GLuint), &m_indices.front(), GL_DYNAMIC_DRAW);
	m_indicesSize = m_indices.size();

	// Position Attribute
	glVertexAttribPointer(
		0, 
		3, 
		GL_FLOAT, 
		GL_FALSE, 
		6 * sizeof(GLfloat), 
		(void*)0);
	glEnableVertexAttribArray(0);

	// Normal Attribute
	glVertexAttribPointer(
		1, 
		3, 
		GL_FLOAT, 
		GL_FALSE, 
		6 * sizeof(GLfloat), 
		(void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
}

Particle* Cloth::GetParticle(int _x, int _y)
{
	return &m_vParticles[_y * m_fParticlesInX + _x];
}

void Cloth::CreateConstraint(Particle* _p1, Particle* _p2, bool _foldingConstraint)
{
	m_vConstraints.push_back(Constraint(_p1, _p2, _foldingConstraint));
}

void Cloth::Render(Camera& _camera, Texture* _texture)
{
	glUseProgram(m_program);
	glDisable(GL_CULL_FACE);

	// Pass Texture to Shader
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _texture->GetID());
	const GLchar* name = "tex";
	glUniform1i(glGetUniformLocation(m_program, name), 0);

	GLuint camLoc = glGetUniformLocation(m_program, "camPos");
	glUniform3fv(camLoc, 1, glm::value_ptr(_camera.get_position() + _camera.get_look_dir() * 15.0f));

	// Setup and Pass Model Matrix to Shader
	glm::mat4 translation = glm::translate(glm::mat4(), m_objPosition);
	glm::vec3 rot = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 rotation = glm::rotate(glm::mat4(), glm::radians(rot.x), glm::vec3(1.0f, 0.0f, 0.0f));
	rotation = glm::rotate(glm::mat4(), glm::radians(rot.y), glm::vec3(0.0f, 1.0f, 0.0f));
	rotation = glm::rotate(glm::mat4(), glm::radians(rot.z), glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 scale = glm::scale(glm::mat4(), m_objScale);

	glm::mat4 Model = translation * rotation * scale;

	glm::mat4 VP = _camera.get_projection() * _camera.get_view();

	GLuint modelLoc = glGetUniformLocation(m_program, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(Model));

	glUniformMatrix4fv(glGetUniformLocation(m_program, "MVP"), 1, GL_FALSE, glm::value_ptr(VP * Model));

	// Update Buffer Datas, because we might lose some particles
	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);

	glBufferSubData(GL_ARRAY_BUFFER, 0, m_vertices.size() * sizeof(GLfloat), m_vertices.data());
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(GLuint), &m_indices.front(), GL_DYNAMIC_DRAW);
	m_indicesSize = m_indices.size();

	// Draw
	glDrawElements(GL_LINES, m_indicesSize, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Cloth::Process(float _deltaTime)
{
	m_indices.clear();
	for (unsigned int i = 0; i < m_vConstraints.size(); ++i)
	{
		if (m_vConstraints[i].GetIsAlive())
		{
			m_indices.push_back(m_vConstraints[i].GetParticle1()->GetVertexId());
			m_indices.push_back(m_vConstraints[i].GetParticle2()->GetVertexId());
		}
	}

	// iterate over all constraints several times
	for (int i = 0; i < CONSTRAINT_ITERATIONS; i++) 
	{
#pragma omp parallel for
		for (auto& constraint: m_vConstraints)
		{
			// satisfy constraint.
			constraint.Process();
		}
	}

	int i = 0;
	for (auto& particle :  m_vParticles)
	{
		//Process the particle
		particle.Process(-10.0f, _deltaTime);

		//Update the positions of the particles
		m_vertices[i] = (particle.GetPos().x);
		m_vertices[i + 1] = (particle.GetPos().y);
		m_vertices[i + 2] = (particle.GetPos().z);
		i += 6;

		//Handle particle picking if left mouse clicked and dragged
		//if (Input::GetInstance()->MouseState[0] == INPUT_HOLD)
		//{
		//	if (!m_isHoldingParticle)
		//	{
		//		ProcessParticlePick(&(*particle));
		//	}
		//	else
		//	{
		//		ProcessParticlePick(pickedParticle);
		//	}
		//}
		//else
		//{
		//	//set as not holding a particle
		//	m_isHoldingParticle = false;
		//}
	}
	//glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);

	//glBufferSubData(GL_ARRAY_BUFFER, 0, m_fVerticesPoints.size() * sizeof(GLfloat), m_fVerticesPoints.data());
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_iIndicesPoints.size() * sizeof(GLuint), &m_iIndicesPoints[0], GL_DYNAMIC_DRAW);
	//m_indicesSize = m_iIndicesPoints.size();
}

void Cloth::ApplyForce(const glm::vec3 _force)
{
	for (auto& particle : m_vParticles)
	{
		particle.ApplyForce(_force); // add the forces to each particle
	}
}

void Cloth::ApplyGravityForce(const glm::vec3 _force)
{
	for (auto& particle : m_vParticles)
	{
		particle.ApplyGravityForce(_force); // add the forces to each particle
	}
}

glm::vec3 Cloth::FindTriangleNormal(glm::vec3 _point1, glm::vec3 _point2, glm::vec3 _point3)
{
	glm::vec3 vector1 = _point2 - _point1;
	glm::vec3 vector2 = _point3 - _point1;

	return glm::cross(vector1, vector2);
}

void Cloth::ApplyWindForceAtTriangle(Particle* _p1, Particle* _p2, Particle* _p3, const glm::vec3 _force)
{
	glm::vec3 normal = FindTriangleNormal(_p1->GetPos(), _p2->GetPos(), _p3->GetPos());
	glm::vec3 d = glm::normalize(normal);
	glm::vec3 force = normal * (glm::dot(d, _force));
	_p1->ApplyForce(force);
	_p2->ApplyForce(force);
	_p3->ApplyForce(force);
}

void Cloth::ApplyWindForce(const glm::vec3 _force)
{
	for (int x = 0; x < m_fParticlesInX - 1; ++x)
	{
		for (int y = 0; y < m_fParticlesInY - 1; ++y)
		{
			ApplyWindForceAtTriangle(GetParticle(x + 1, y), GetParticle(x, y), GetParticle(x, y + 1), _force);
			ApplyWindForceAtTriangle(GetParticle(x + 1, y + 1), GetParticle(x + 1, y), GetParticle(x, y + 1), _force);
		}
	}
}

void Cloth::Squish(int dir)
{
	// Get All Top Horizontal Particles
	for (int i = 1; i <= m_fParticlesInX; i++)
	{
		// Get the middle particle's number
		int middle = m_fParticlesInX / 2;

		// The amount of horizontal distance the left particles need to move
		float left_offset = 0.2f * dir * (middle / i * 0.01f);

		// The amount of horizontal distance the right particles need to move
		float right_offset = 0.2f * dir * -(middle / (m_fParticlesInX + 1 - i) * 0.01f);

		// For Particles to the left of the middle particle
		if (i < middle)
		{
			GetParticle(i - 1, 0)->AdjustPinnedPosition(glm::vec3(left_offset, 0.0f, 0.0f));
		}
		// For Particles to the right of the middle particle
		else if (i > middle)
		{
			GetParticle(i - 1, 0)->AdjustPinnedPosition(glm::vec3(right_offset, 0.0f, 0.0f));
		}
	}
}
