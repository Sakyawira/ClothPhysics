#pragma once
#include "Mesh.h"
#include "Particle.h"
#include "Constraint.h"
#include "Camera.h"
#include "GameObject.h"
#include "Texture.h"

const static int CONSTRAINT_ITERATIONS = 5;

class Cloth : public Mesh
{
	public:
		Cloth(GLuint program, int _numParticlesX, int _numParticlesY);
		void Initialize(float _width, float _height, glm::vec3 _pos);
		void GenerateBuffers();
		void Render(Camera& _camera, Texture* _texture);
		void Process(float _deltaTick);
		void ApplyForce(const glm::vec3 _force);
		void ApplyGravityForce(const glm::vec3 _force);
		void ApplyWindForce(const glm::vec3 _force);
		void Unpin();
		void Squish(int dir);
		void SetOnFire();
		void SphereCollision(GameObject* _sphere);
		void BoxCollision(GameObject* _box);
		void PyramidCollision(GameObject* _pyramid);
		void ProcessParticlePick(Particle* particle, Camera* _camera);

		void SetDebug(bool _debug);
		bool GetDebug() const { return m_debugMode; }

	private:

		bool SameFaceDir(glm::vec3 _point1, glm::vec3 _point2, glm::vec3 _point3, glm::vec3 _middlePyramid, glm::vec3 _particle);
		bool SameFaceDir(glm::vec3 _point1, glm::vec3 _point2, glm::vec3 _point3, glm::vec3 _point4, glm::vec3 _middlePyramid, glm::vec3 _particle);
		glm::vec3 FindTriangleNormal(glm::vec3 _point1, glm::vec3 _point2, glm::vec3 _point3);
		void ApplyWindForceAtTriangle(Particle* _p1, Particle* _p2, Particle* _p3, const glm::vec3 _force);

		Particle* GetParticle(int _x, int _y);
		void CreateConstraint(Particle* _p1, Particle* _p2, bool _foldingConstraint = false);

		GLuint m_program;

		bool m_debugMode = false;

		// number of particles in "x" direction
		int m_fParticlesInX; 
		// number of particles in "y" direction
		int m_fParticlesInY; 

		bool m_isHoldingParticle = false;
		Particle* m_pickedParticle;

		// Particles which construct this cloth
		unsigned int m_particleCount = 0;
		std::vector<Particle> m_vParticles;
		// Constraints between Particles which construct this cloth
		std::vector<Constraint> m_vConstraints; 

		// Scale
		glm::vec3 m_scale = glm::vec3(1.0f, 1.0f, 1.0f);

		// Translation Matrix
		glm::vec3 m_objPosition = glm::vec3(0, 0, 0);
		glm::mat4 m_translationMatrix = glm::translate(glm::mat4(), m_objPosition);

		// Rotation
		glm::vec3 m_rotationAxisZ = glm::vec3(0.0f, 1.0f, 0.0f);
		float m_rotationAngle = 0.0f;
		glm::mat4 m_rotationZ = glm::rotate(glm::mat4(), glm::radians(m_rotationAngle), m_rotationAxisZ);

		// Scaling 
		glm::vec3 m_objScale = glm::vec3(1.0f * m_scale.x, 1.0f * m_scale.y, 1.0f * m_scale.z);
		glm::mat4 m_scaleMatrix = glm::scale(glm::mat4(), m_objScale);

		// Model matrix
		glm::mat4 m_modelMatrix = m_translationMatrix * m_rotationZ * m_scaleMatrix;

};