#include "Constraint.h"
#include <iostream>


Constraint::Constraint(Particle* _p1, Particle* _p2, bool _foldingConstraint)
{
	m_Particle1 = _p1;
	m_Particle2 = _p2;
	m_bFoldingConstraint = _foldingConstraint;

	m_fRestitutionDistance = glm::distance(m_Particle1->GetPos(), m_Particle2->GetPos());
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

bool Constraint::Process()
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
			particleDistance = 0.00000000001f;
		}

		/*---------Spread flames here---------*/
		//If burn timer is greater than certain value, set the other particle on fire

		/*---------Add damage here---------*/
		//F2: The cloth can be torn under some amount of force
		//If the particles are too far away, add damage
		//If it's not too far away add health
		//If one or more of our particles dies, then we die too

		//If this constraint is still alive calculate the constraints
		glm::vec3 correctionOffset;
		glm::vec3 halfCorrectionOffset;

		if(m_bFoldingConstraint)
		{
			//Make the folding constraints have 1/10th of the applied offset
			correctionOffset = particleDif * ((1.0f - (m_fRestitutionDistance / particleDistance)) / 15.0f);
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

	SetIsAlive(false);

	return false;
}
