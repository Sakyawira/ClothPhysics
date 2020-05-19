#include "Cloth.h"

#include <algorithm>
#include <random>

Cloth::Cloth(GLuint program)
{
	m_program = program;
}

void Cloth::Initialize(float _width, float _height, int _numParticlesX, int _numParticlesY, glm::vec3 _pos)
{
	// Reset particles and constraints
	m_fParticlesInX = _numParticlesX;
	m_fParticlesInY = _numParticlesY;

	m_vParticles.clear();
	m_vConstraints.clear();

	m_vParticles.resize(_numParticlesX * _numParticlesY);

	m_objPosition = _pos;

	int verticesID = 0;
	
	// Creates particles in a grid of particles from (0,0,0) to (width,-height,0)
	for (int y = 0; y < _numParticlesY; y++)
	{
		const int tempIndexCalc = y * _numParticlesX;
		for (int x = 0; x < _numParticlesX; x++)
		{
			glm::vec3 pos = glm::vec3(_width * (x / (float)_numParticlesX) + _pos.x,
									 -_height * (y / (float)_numParticlesY) + _pos.y,
									 _pos.z);

			m_vParticles[tempIndexCalc + x] = Particle(pos); // insert particle in column x at y'th row

			// Set vertices id and vertices for each particle
			m_vParticles[tempIndexCalc + x].SetVertexId(verticesID);
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
			//Cloth base constraints
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

			//Cloth folding constraints (2 apart)
			if (x < _numParticlesX - 2)
			{
				CreateConstraint(GetParticle(x, y), GetParticle(x + 2, y), true);
			}
			if (y < _numParticlesY - 2)
			{
				CreateConstraint(GetParticle(x, y), GetParticle(x, y + 2), true);
			}
			if (x < _numParticlesX - 2 && y < _numParticlesY - 2)
			{
				CreateConstraint(GetParticle(x, y), GetParticle(x + 2, y + 2), true);
			}
			if (x < _numParticlesX - 2 && y < _numParticlesY - 2)
			{
				CreateConstraint(GetParticle(x + 2, y), GetParticle(x, y + 2), true);
			}

			//Cloth folding constraints (3 apart)
			if (x < _numParticlesX - 3)
			{
				CreateConstraint(GetParticle(x, y), GetParticle(x + 3, y), true);
			}
			if (y < _numParticlesY - 3)
			{
				CreateConstraint(GetParticle(x, y), GetParticle(x, y + 3), true);
			}
			if (x < _numParticlesX - 3 && y < _numParticlesY - 3)
			{
				CreateConstraint(GetParticle(x, y), GetParticle(x + 3, y + 3), true);
			}
			if (x < _numParticlesX - 3 && y < _numParticlesY - 3)
			{
				CreateConstraint(GetParticle(x + 3, y), GetParticle(x, y + 3), true);
			}
		}
	}

	//Shuffle the constraints so they are processed in a random order
	auto rng = std::default_random_engine{};
	std::shuffle(m_vConstraints.begin(), m_vConstraints.end(), rng);
	

	// Set pins at top of the cloth

	//for (int i = 0; i < _numParticlesX; i++)
	//{
	//	GetParticle(0 + i, 0)->SetPin(true);
	//}
	GetParticle(0, 0)->SetPin(true);
	GetParticle(_numParticlesX - 1, 0)->SetPin(true);
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
	m_iIndicesPoints.clear();
	for (auto& constraint : m_vConstraints)
	{
		if (!constraint.GetIsAlive() || constraint.GetParticle2() - constraint.GetParticle1() < 5.0f)
		{
			m_iIndicesPoints.push_back(constraint.GetParticle1()->GetVertexId());
			m_iIndicesPoints.push_back(constraint.GetParticle2()->GetVertexId());
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

	// Use Texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _texture->GetID());
	const GLchar* name = "tex";
	glUniform1i(glGetUniformLocation(m_program, name), 0);

	GLuint camLoc = glGetUniformLocation(m_program, "camPos");
	glUniform3fv(camLoc, 1, glm::value_ptr(_camera.get_position() + _camera.get_look_dir() * 15.0f));

	//ModelMatrix
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

	//Constantly update vertices and indices when rendering
	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);

	glBufferSubData(GL_ARRAY_BUFFER, 0, m_fVerticesPoints.size() * sizeof(GLfloat), m_fVerticesPoints.data());
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_iIndicesPoints.size() * sizeof(GLuint), &m_iIndicesPoints[0], GL_DYNAMIC_DRAW);
	m_indicesSize = m_iIndicesPoints.size();

	//Draw the cloth
	glDrawElements(GL_LINES, m_indicesSize, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Cloth::Process(float _deltaTime)
{
	m_iIndicesPoints.clear();
	for (unsigned int i = 0; i < m_vConstraints.size(); ++i)
	{
		if (m_vConstraints[i].GetIsAlive())
		{
			m_iIndicesPoints.push_back(m_vConstraints[i].GetParticle1()->GetVertexId());
			m_iIndicesPoints.push_back(m_vConstraints[i].GetParticle2()->GetVertexId());
		}
	}

	// iterate over all constraints several times
	for (int i = 0; i < CONSTRAINT_ITERATIONS; i++) 
	{
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
		m_fVerticesPoints[i] = (particle.GetPos().x);
		m_fVerticesPoints[i + 1] = (particle.GetPos().y);
		m_fVerticesPoints[i + 2] = (particle.GetPos().z);
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
		float left_offset = dir * (middle / i * 0.01f);

		// The amount of horizontal distance the right particles need to move
		float right_offset = dir * -(middle / (m_fParticlesInX + 1 - i) * 0.01f);

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
