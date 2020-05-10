#pragma once
#include "Particle.h"

class Constraint
{
	public:

		//Constructor (takes both particles with which it is connected)
		Constraint(Particle* _p1, Particle* _p2);

		// Calculate Constraints
		bool Process();

		// Getters-Setters
		void SetIsAlive(bool _IsAlive);
		bool GetIsAlive() { return m_bIsAlive; };

		Particle* GetParticle1() {	return m_Particle1; };
		Particle* GetParticle2() {return m_Particle2;};

		float GetRestitutionDistance() { return m_fRestitutionDistance; };

	private:
	
		Particle* m_Particle1;
		Particle* m_Particle2;

		float m_bBurnTimer = 0.5f;
		bool m_bToBeDestroyed = false;
		bool m_bIsAlive = true;
		float m_fRestitutionDistance = 0.1f
};