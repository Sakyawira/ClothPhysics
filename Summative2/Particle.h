/***********************
  Bachelor of Software Engineering
  Media Design School
  Auckland
  New Zealand

  (c) 2018 Media Design School

  File Name   :   Particle.h
  Description :   contains declaration of GameObject class which control the updates and drawing pipeline of an object
  Author      :   Sakyawira Nanda Ruslim and Anton Le-Prevost Smith
  Mail        :   Sakyawira.Rus8080@mediadesign.school.nz and Anton.Le8064@mediadesign.school.nz
********************/
#pragma once

// Library Includes
#include <vector>

// Dependency Includes
#include <glew.h>
#include <gtc/type_ptr.hpp>
#include <glm.hpp>
#include <mat4x2.hpp>
class Particle
{   
    public:
        Particle() {};
        Particle(glm::vec3 _position);
        ~Particle(){};

        void Process(float _groundY);

		// Applies force - affected by the mass
        void ApplyForce(glm::vec3 _force);
	
		// Applies gravity force - not affected by mass
        void ApplyGravityForce(glm::vec3 _force);

        void DecrementConnectionCount() { m_iConnectionCount--; }

        void AdjustPosition(const glm::vec3 _v3) { if (!m_bIsPinned) m_v3Position += _v3; }
        void AdjustPinnedPosition(const glm::vec3 _v3) {m_v3Position += _v3; }

        // Getters-Setters
        glm::vec3 GetPos() { return m_v3Position; }
        void SetPin(bool _IsPinned) { m_bIsPinned = _IsPinned; }

        int GetVertexId() { return m_iVertexId; }
        void SetVertexId(int _id) { m_iVertexId = _id; }

        int GetIndexId() { return m_iIndexId; }
        void SetIndexId(int _id) { m_iIndexId = _id; }
        
    private:

        // Keeps track of whether it can be moved
        bool m_bIsPinned = false;
  
        // Position of the particle
        glm::vec3 m_v3Position{};

        // Old Position of the particle
        glm::vec3 m_v3OldPosition{};

        // Acceleration
        glm::vec3 m_v3Acceleration{};

        // Connection Count
        int m_iConnectionCount{};

         // Mass of particle 
        float m_fMass = 1.0f;

        // Dampening value
        float m_fDampening = 0.01f;

        // Vertex and Index
        int m_iVertexId{};
        int m_iIndexId{};
};
