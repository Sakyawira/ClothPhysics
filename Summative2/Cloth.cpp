#include "Cloth.h"

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
	// Set pins at top of the cloth
	float offsetX = -1;
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

void Cloth::CreateConstraint(Particle* _p1, Particle* _p2)
{
	m_vConstraints.push_back(Constraint(_p1, _p2));
}

void Cloth::Render(Camera& _camera) 
{
	glUseProgram(m_program);
	glDisable(GL_CULL_FACE);

	//ModelMatrix
	glm::mat4 translation = glm::translate(glm::mat4(), m_objPosition);
	glm::vec3 rot = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 rotation = glm::rotate(glm::mat4(), glm::radians(rot.x), glm::vec3(1.0f, 0.0f, 0.0f));
	rotation = glm::rotate(glm::mat4(), glm::radians(rot.y), glm::vec3(0.0f, 1.0f, 0.0f));
	rotation = glm::rotate(glm::mat4(), glm::radians(rot.z), glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 scale = glm::scale(glm::mat4(), m_objScale);

	glm::mat4 Model = translation * rotation * scale;

	glm::mat4 VP = _camera.get_projection() * _camera.get_view();

	//Constantly update vertices and indices when rendering
	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);

	glBufferSubData(GL_ARRAY_BUFFER, 0, m_fVerticesPoints.size() * sizeof(GLfloat), m_fVerticesPoints.data());
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_iIndicesPoints.size() * sizeof(GLuint), &m_iIndicesPoints[0], GL_DYNAMIC_DRAW);
	m_indicesSize = m_iIndicesPoints.size();

	glUniformMatrix4fv(glGetUniformLocation(m_program, "MVP"), 1, GL_FALSE, glm::value_ptr(VP * Model));

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

	std::vector<Constraint>::iterator constraint;
	for (int i = 0; i < CONSTRAINT_ITERATIONS; i++) // iterate over all constraints several times
	{
		for (constraint = m_vConstraints.begin(); constraint != m_vConstraints.end(); constraint++)
		{
			constraint->Process(/*_deltaTime*/); // satisfy constraint.
		}
	}

	int i = 0;
	std::vector<Particle>::iterator particle;
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
	std::vector<Particle>::iterator particle;
	for (auto& particle : m_vParticles)
	{
		particle.ApplyForce(_force); // add the forces to each particle
	}
}