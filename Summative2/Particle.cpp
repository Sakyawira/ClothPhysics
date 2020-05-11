#include "Particle.h"

Particle::Particle(glm::vec3 _position)
{
	m_v3Position = _position;
	m_v3OldPosition = _position;
}

void Particle::Process(float _groundY, float _deltaTime)
{
	if(!m_bIsPinned)
	{
		// Verlet Integration
		glm::vec3 v3Temp = m_v3Position;
		m_v3Position = m_v3Position + (m_v3Position - m_v3OldPosition) * (1.0f - m_fDampening) + (m_v3Acceleration * _deltaTime);
		m_v3OldPosition = v3Temp;

		// Particle drops to the ground
		if (m_v3Position.y < _groundY)
		{
			m_v3Position.y = _groundY;
		}

		// Reset Acceleration so it doesn't keep on building up everytime a force is added (AddForce is called / in this case every frame)
		m_v3Acceleration = glm::vec3(0.0f, 0.0f, 0.0f);
	}
}

void Particle::ApplyForce(glm::vec3 _force)
{
	// Accelerate particle based on force passed in
	// Make sure it's not zero so we don't divide by zero and waste 2 hours of time because I forgot to initialise the mass in the first place god damnit
	if(m_fMass != 0)
	{
		m_v3Acceleration += _force / m_fMass;
	}
	else
	{
		//oops you did it again
		assert(0);
	}
}

void Particle::ApplyGravityForce(glm::vec3 _force)
{
	// Accelerate particle based on force passed in
	// Make sure it's not zero so we don't divide by zero and waste 2 hours of time because I forgot to initialise the mass in the first place god damnit
	m_v3Acceleration += _force;
}