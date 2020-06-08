#include "Cloth.h"

#include <algorithm>
#include <cstdlib>  // For srand() and rand()
#include <random>

Cloth::Cloth(GLuint program, int _numParticlesX, int _numParticlesY)
{
	m_program = program;
	m_fParticlesInX = _numParticlesX;
	m_fParticlesInY = _numParticlesY;
}

void Cloth::Initialize(float _width, float _height, glm::vec3 _pos)
{
	//Reset the number of particles we have created
	m_particleCount = -1;
	
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
			m_vParticles[iIndexOffset + x] = Particle(pos, ++m_particleCount); 

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
				GetParticle(x, y)->IncrementConnectionCount();
				GetParticle(x + 1, y)->IncrementConnectionCount();
			}
			if (y < m_fParticlesInY - 1)
			{
				CreateConstraint(GetParticle(x, y), GetParticle(x, y + 1));
				GetParticle(x, y)->IncrementConnectionCount();
				GetParticle(x, y + 1)->IncrementConnectionCount();
			}
			if (x < m_fParticlesInX - 1 && y < m_fParticlesInY - 1)
			{
				CreateConstraint(GetParticle(x, y), GetParticle(x + 1, y + 1));
				GetParticle(x, y)->IncrementConnectionCount();
				GetParticle(x + 1, y + 1)->IncrementConnectionCount();
			}
			if (x < m_fParticlesInX - 1 && y < m_fParticlesInY - 1)
			{
				CreateConstraint(GetParticle(x + 1, y), GetParticle(x, y + 1));
				GetParticle(x + 1, y)->IncrementConnectionCount();
				GetParticle(x, y + 1)->IncrementConnectionCount();
			}

			//// Cloth folding/interweaved constraints (2 apart)
			if (x < m_fParticlesInX - 2)
			{
				CreateConstraint(GetParticle(x, y), GetParticle(x + 2, y));
				GetParticle(x, y)->IncrementConnectionCount();
				GetParticle(x + 2, y)->IncrementConnectionCount();
			}
			if (y < m_fParticlesInY - 2)
			{
				CreateConstraint(GetParticle(x, y), GetParticle(x, y + 2));
				GetParticle(x, y)->IncrementConnectionCount();
				GetParticle(x, y + 2)->IncrementConnectionCount();
			}
			if (x < m_fParticlesInX - 2 && y < m_fParticlesInY - 2)
			{
				CreateConstraint(GetParticle(x, y), GetParticle(x + 2, y + 2));
				GetParticle(x, y)->IncrementConnectionCount();
				GetParticle(x + 2, y + 2)->IncrementConnectionCount();
			}
			if (x < m_fParticlesInX - 2 && y < m_fParticlesInY - 2)
			{
				CreateConstraint(GetParticle(x + 2, y), GetParticle(x, y + 2));
				GetParticle(x + 2, y)->IncrementConnectionCount();
				GetParticle(x, y + 2)->IncrementConnectionCount();
			}

			////// Cloth folding/interweaved constraints (3 apart)
			//if (x < m_fParticlesInX - 3)
			//{
			//	CreateConstraint(GetParticle(x, y), GetParticle(x + 3, y));
			//	GetParticle(x, y)->IncrementConnectionCount();
			//	GetParticle(x + 3, y)->IncrementConnectionCount();
			//}
			//if (y < m_fParticlesInY - 3)
			//{
			//	CreateConstraint(GetParticle(x, y), GetParticle(x, y + 3));
			//	GetParticle(x, y)->IncrementConnectionCount();
			//	GetParticle(x, y + 3)->IncrementConnectionCount();
			//}
			//if (x < m_fParticlesInX - 3 && y < m_fParticlesInY - 3)
			//{
			//	CreateConstraint(GetParticle(x, y), GetParticle(x + 3, y + 3));
			//	GetParticle(x, y)->IncrementConnectionCount();
			//	GetParticle(x + 3, y + 3)->IncrementConnectionCount();
			//}
			//if (x < m_fParticlesInX - 3 && y < m_fParticlesInY - 3)
			//{
			//	CreateConstraint(GetParticle(x + 3, y), GetParticle(x, y + 3));
			//	GetParticle(x + 3, y)->IncrementConnectionCount();
			//	GetParticle(x, y + 3)->IncrementConnectionCount();
			//}
		}
	}

	auto rng = std::default_random_engine{};
	std::shuffle(m_vConstraints.begin(), m_vConstraints.end(), rng);

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
		if (constraint.GetIsAlive())
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
	if (m_indices.size() > 1)
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
}

void Cloth::Process(float _deltaTime)
{
	// iterate over all constraints several times
	for (int i = 0; i < CONSTRAINT_ITERATIONS; i++) 
	{
		for (auto& constraint: m_vConstraints)
		{
			// Process constraint.
			if(constraint.GetIsAlive() && !constraint.Process(_deltaTime, m_debugMode))
			{
				constraint.SetIsAlive(false);
			}
		}
	}

	//Check if each constraints particle is alive and set them based on that
	for (int i = 0; i < m_vConstraints.size(); ++i)
	{
		m_vConstraints[i].DetermineIsAlive();
	}

	//Check if each constraints particle is alive and set them based on that
	for (auto& constraint : m_vConstraints)
	{
		constraint.DetermineIsAlive();
	}

	int i = 0;
	for (auto& particle :  m_vParticles)
	{
		//Process the particle
		particle.Process(-10.0f, _deltaTime);

		//Update the positions of the particles
		m_vertices[i]     = (particle.GetPos().x);
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

	m_indices.clear();
	for (auto& m_vConstraint : m_vConstraints)
	{
		if (m_vConstraint.GetIsAlive())
		{
			m_indices.push_back(m_vConstraint.GetParticle1()->GetVertexId());
			m_indices.push_back(m_vConstraint.GetParticle2()->GetVertexId());
		}
	}
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

bool Cloth::SameFaceDir(glm::vec3 _point1, glm::vec3 _point2, glm::vec3 _point3, glm::vec3 _middlePyramid, glm::vec3 _particle)
{
	glm::vec3 normal = FindTriangleNormal(_point1, _point2, _point3);
	glm::vec3 centroid = (_point1 + _point2 + _point3) / 3.0f;

	// Check whether or not the triangle and a line between
	// the middle of the pyramid and the particle
	// are facing each other
	// if return is -1, they are facing each other (opposite direction)
	float dotV4 = glm::dot(normal, centroid - _particle);

	//// Check whether or not the triangle and a line between a point in the triangle and the particle are facing each other
	//float dotP = glm::dot(normal, _point3 - p);

	return dotV4 > 0;
}

bool Cloth::SameFaceDir(glm::vec3 _point1, glm::vec3 _point2, glm::vec3 _point3, glm::vec3 _point4, glm::vec3 _middlePyramid, glm::vec3 _particle)
{
	// Get the normal of a quad (facing direction)
	//glm::vec3 normal = cross(_point2 - _point1, _point3 - _point1);

	// Check if the quad and a line between two points in the quad are facing each other
	//float dotV4 = glm::dot(normal, _point4 - _point1);

	// Check whetehr or not the quad and a line between the particle and a point in the quad are facing each other
	//float dotP = glm::dot(normal, _particle - _point1);

	// if both are facing each other or both are not facing each other
	// if return is -1, they are facing each other (opposite direction)
	//float dotV4 = glm::dot(normal, _middlePyramid - _particle);
	return _particle.y > _middlePyramid.y && _particle.y < _point4.y;
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

void Cloth::SetOnFire()
{

	GetParticle(rand() % m_fParticlesInX, rand() % m_fParticlesInY)->SetOnFire(true);
}

void Cloth::SetDebug(bool _debug)
{
	m_debugMode = _debug;
}

void Cloth::BoxCollision(GameObject* _box)
{
	// For each particle
	for (auto& particle : m_vParticles)
	{
		glm::vec3 point = particle.GetPos();
		if ((point.x >= _box->GetMin().x && point.x <= _box->GetMax().x) &&
			(point.y >= _box->GetMin().y && point.y <= _box->GetMax().y) &&
			(point.z >= _box->GetMin().z && point.z <= _box->GetMax().z))
		{
			// If it is not colliding before
			if (particle.isCollided == false)
			{
				particle.isCollided = true;
				particle.first_point_col = particle.GetPos();
			}
			// if it has collided at the previous frame
			else
			{
				glm::vec3 center = _box->GetLocation();

				// Direction Vector from Particle to the Sphere
				glm::vec3 direction = glm::normalize(particle.GetPos() - center);

				// Calculate the difference between the radius of the sphere and its distance with the particle 
				// Add it back to the particle position, so it is moved back out
				float difference = glm::distance(particle.GetPos(), particle.first_point_col);
				particle.AdjustPosition(direction * difference);
			}
		}
		// if it is not colliding anymore
		else
		{
			particle.isCollided = false;
		}
	}
}

void Cloth::PyramidCollision(GameObject* _pyramid)
{
	glm::vec3 v1 = glm::vec3(-0.5f, 0.0f, -0.5f) * _pyramid->GetScale()	+ _pyramid->GetLocation();
	glm::vec3 v2 = glm::vec3(-0.5f, 0.0f, 0.5f) * _pyramid->GetScale() + _pyramid->GetLocation();
	glm::vec3 v3 = glm::vec3(0.5f, 0.0f, 0.5f) * _pyramid->GetScale() + _pyramid->GetLocation();
	glm::vec3 v4 = glm::vec3(0.5f, 0.0f, -0.5f) * _pyramid->GetScale() + _pyramid->GetLocation();
	glm::vec3 top_pyramid	 = glm::vec3(0.0f, 0.5f, 0.0f) * _pyramid->GetScale() + _pyramid->GetLocation();
	glm::vec3 bottom_pyramid = _pyramid->GetLocation();

	for (auto& particle : m_vParticles)
	{
		glm::vec3 _particleLoc = particle.GetPos();
		// True means pyramid collide
		if (SameFaceDir(v1, v2, top_pyramid, bottom_pyramid, _particleLoc) && 
			SameFaceDir(v2, v3, top_pyramid, bottom_pyramid, _particleLoc) &&
			SameFaceDir(v3, v4, top_pyramid, bottom_pyramid, _particleLoc) && 
			SameFaceDir(v4, v1, top_pyramid, bottom_pyramid, _particleLoc) &&
			SameFaceDir(v1, v2, v3, top_pyramid, bottom_pyramid, _particleLoc))
		{
			// If it is not colliding before
			if (particle.isCollided == false)
			{
				particle.isCollided = true;
				particle.first_point_col = particle.GetPos();
			}
			// if it has collided at the previous frame
			else
			{
				glm::vec3 center = _pyramid->GetLocation() + glm::vec3(0.0f, 0.25f, 0.0f) * _pyramid->GetScale();//(_pyramid->GetLocation() +  top_pyramid) * 0.5f;

				// Direction Vector from Particle to the Sphere
				glm::vec3 direction = glm::normalize(particle.GetPos() - center);

				// Calculate the difference between the radius of the sphere and its distance with the particle 
				// Add it back to the particle position, so it is moved back out
				float difference = glm::distance(particle.GetPos(), particle.first_point_col);
				particle.AdjustPosition(direction * difference);
			}
			std::cout << "Collided!" << std::endl;
		}
		// if it is not colliding anymore
		else
		{
			particle.isCollided = false;
		}
	}
}

void Cloth::SphereCollision(GameObject* _sphere)
{
	float offset = 0.0f;
	float radius = _sphere->GetScale().x;
	glm::vec3 center = _sphere->GetLocation();

	// For each particle
	for (auto& particle : m_vParticles)
	{
		// Direction Vector from Particle to the Sphere
		glm::vec3 direction = glm::normalize(particle.GetPos() - center);

		// Distance between Particle and Sphere Center
		float distance = glm::distance(particle.GetPos(), center);

		// If the distance is less than the radius, it means the particle is inside the sphere
		if (distance < radius + offset)
		{
			// Calculate the difference between the radius of the sphere and its distance with the particle 
			// Add it back to the particle position, so it is moved back out
			float difference = ((radius + offset) - distance);
			particle.AdjustPosition(direction * difference);
		}
	}
}
