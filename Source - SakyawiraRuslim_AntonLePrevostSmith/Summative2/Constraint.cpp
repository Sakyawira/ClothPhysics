#include "Constraint.h"
#include <iostream>


Constraint::Constraint(Particle* _p1, Particle* _p2, bool _foldingConstraint)
{
	m_Particle1 = _p1;
	m_Particle2 = _p2;
	m_bFoldingConstraint = _foldingConstraint;

	if (m_Particle1 && m_Particle2)
	{
		m_fRestitutionDistance = glm::distance(m_Particle1->GetPos(), m_Particle2->GetPos());
	}
}

void Constraint::SetIsAlive(bool _isAlive)
{
	m_bIsAlive = _isAlive;

	// If this constraint is not alive anymore and the connected particles exist,
	// then have the particles decrement their number of connections.
	if(!m_bIsAlive)
	{
		if(m_Particle1 && m_Particle2)
		{
			m_Particle1->DecrementConnectionCount();
			m_Particle2->DecrementConnectionCount();
		}
	}
}

bool Constraint::Process(float _deltaTime)
{
	// Calculates and applies the constraints to the connect particles,
	// if the particles are valid
	if(m_bIsAlive && m_Particle1 && m_Particle2)
	{
		// Get the distance between the particles
		glm::vec3 particleDif = m_Particle1->GetPos() - m_Particle2->GetPos();
		float particleDistance = glm::length(particleDif);

		if(particleDistance == 0.0f)
		{
			particleDistance = 0.000001f;
		}

		/*---------Spread flames here---------*/
		//If burn timer is greater than certain value, set the other particle on fire
		//Spread the flame if it has been burning long enough
		if (m_Particle1->IsOnFire() && !m_Particle2->IsPinned() && m_Particle1->GetBurnTimer() >= 0.3f + static_cast<float>(rand() % 200) / 100.0f)
		{
			m_Particle2->SetOnFire(true);
		}
		if (m_Particle2->IsOnFire() && !m_Particle1->IsPinned() && m_Particle2->GetBurnTimer() >= 0.3f + static_cast<float>(rand() % 200) / 100.0f)
		{
			m_Particle1->SetOnFire(true);
		}

		/*---------Add damage here---------*/
		//F2: The cloth can be torn under some amount of force
		//If the particles are too far away, add damage
		//If it's not too far away add health
		//If one or more of our particles dies, then we die too
		float tearDistance = particleDistance - m_fRestitutionDistance;
		float constraintTearResistance = 0.5f;
		if(m_Particle1->IsPinned() || m_Particle2->IsPinned())
		{
			constraintTearResistance = 10.0f;
		}

		if (tearDistance > constraintTearResistance)
		{
			//Reduces health by around 50+ health per second
			m_Particle1->AddHealth(-25.0f * particleDistance * _deltaTime);
			m_Particle2->AddHealth(-25.0f * particleDistance * _deltaTime);
		}
		else
		{
			//It's healing, as long as it's not burning.
			if (!m_Particle1->IsOnFire())
			{
				//Adds health at a rate of 100 per second
				m_Particle1->AddHealth(25.0f * _deltaTime);
			}
			if (!m_Particle2->IsOnFire())
			{
				m_Particle2->AddHealth(25.0f * _deltaTime);
			}
		}

		//One or more of our particles is dead. We should die too.
		if (m_Particle1->GetHealth() <= 0.0f || m_Particle2->GetHealth() <= 0.0f)
		{
			return false;
		}

		/*------Apply correction offset here------*/
		//If this constraint is still alive calculate the constraints
		glm::vec3 correctionOffset;
		glm::vec3 halfCorrectionOffset;

		if(m_bFoldingConstraint)
		{
			//Make the folding constraints have 1/10th of the applied offset
			correctionOffset = particleDif * (m_stiffness - ((m_fRestitutionDistance / particleDistance) * m_stiffness));
			halfCorrectionOffset = correctionOffset * 0.5f;
		}
		else
		{
			correctionOffset = particleDif * (m_stiffness - ((m_fRestitutionDistance / particleDistance) * m_stiffness));
			halfCorrectionOffset = correctionOffset * 0.5f;
		}

		m_Particle1->AdjustPosition(-halfCorrectionOffset);
		m_Particle2->AdjustPosition(halfCorrectionOffset);
		
		return true;
	}

	//Constraint is ded
	return false;
}
