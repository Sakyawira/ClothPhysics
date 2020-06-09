#include "Particle.h"

#include "Random.h"

#include <iostream>

Particle::Particle(glm::vec3 _position, unsigned int _id)
{
	m_v3Position = _position;
	m_v3OldPosition = _position;
	m_ID = _id;
	m_burnRate = -50.0f + -200.0f * GenerateFloat();
}

void Particle::Process(float _groundY, float _deltaTime)
{
	if (m_bIsAlive)
	{
		if (!m_bIsPinned)
		{
			if (m_bOnFire)
			{
				//Increase burn timer, apply upward force and reduce health
				m_fBurnTimer += _deltaTime;
				ApplyForce(glm::vec3(GenerateFloat() * 0.5, 0.5f, GenerateFloat() * 0.5));
				AddHealth(m_burnRate * _deltaTime);
			}
			
			// Verlet Integration
			glm::vec3 v3Temp = m_v3Position;
			m_v3Position = m_v3Position + (m_v3Position - m_v3OldPosition) * (1.0f - m_fDampening) + (m_v3Acceleration * _deltaTime * _deltaTime);
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
	else
	{

		if(m_iConnectionCount < 0)
		{
			// You have a bug!
			system("pause");
		}
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
	m_v3Acceleration += _force;
}

void Particle::AddHealth(float _addedHealth)
{
	m_fHealth += _addedHealth;

	if(m_fHealth > 100.0f)
	{
		m_fHealth = 100.0f;
	}
	else if(m_fHealth <= 0.0f)
	{
		m_fHealth = 0.0f;
		m_bIsAlive = false;
	}
}

void Particle::DecrementConnectionCount()
{
	if (m_iConnectionCount > 0)
	{
		m_iConnectionCount--;
	}
	
	if (m_iConnectionCount <= 0)
	{
		m_fHealth = 0.0f;
		m_bIsAlive = false;
		SetPin(false);
	}
}

void Particle::AdjustPosition(const glm::vec3 _v3)
{
	if (!m_bIsPinned)
	{
		m_v3Position += _v3;
	}
		
}
