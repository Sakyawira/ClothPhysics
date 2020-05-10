#pragma once
#include "Mesh.h"
#include "Particle.h"
#include "Constraint.h"

class Cloth : public Mesh
{
	public:
		Cloth();
		void Initialize();

	private:
		std::vector<float> m_fVerticesPoints;
		std::vector<int> m_iIndicesPoints;

		float m_fWidth;
		float m_fHeight;

		int m_fParticlesWidth; // number of particles in "width" direction
		int m_fParticlesHeight; // number of particles in "height" direction

		bool m_isHoldingParticle = false;
		Particle* pickedParticle;

		std::vector<Particle> m_vParticles; // all particles that are part of this cloth
		std::vector<Constraint> m_vConstraints; // alle constraints between particles as part of this cloth

		Particle* GetParticle(int x, int y);
		void MakeConstraint(Particle* p1, Particle* p2);

};