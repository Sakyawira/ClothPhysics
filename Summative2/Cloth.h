#pragma once
#include "Mesh.h"
#include "Particle.h"
#include "Constraint.h"
#define CONSTRAINT_ITERATIONS 5

class Cloth : public Mesh
{
	public:
		Cloth();
		void Initialize(float _width, float _height, int _numParticlesWidth, int _numParticlesHeight, glm::vec3 _pos);
		void GenerateBuffers();
		void Render();
		void Process(float _deltaTick);
		void ApplyForce(const glm::vec3 _force);

	private:
		std::vector<float> m_fVerticesPoints;
		std::vector<int> m_iIndicesPoints;

		int m_fParticlesInX; // number of particles in "x" direction
		int m_fParticlesInY; // number of particles in "y" direction

		bool m_isHoldingParticle = false;
		Particle* pickedParticle;

		std::vector<Particle> m_vParticles; // all particles that are part of this cloth
		std::vector<Constraint> m_vConstraints; // alle constraints between particles as part of this cloth

		Particle* GetParticle(int _x, int _y);
		void CreateConstraint(Particle* _p1, Particle* _p2);

};