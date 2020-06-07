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
        Particle(glm::vec3 _position, unsigned int _id);
        ~Particle(){};

        void Process(float _groundY, float _deltaTime);

		// Applies force - affected by the mass
        void ApplyForce(glm::vec3 _force);
	
		// Applies gravity force - not affected by mass
        void ApplyGravityForce(glm::vec3 _force);

        void AddHealth(float _addedHealth);

        void IncrementConnectionCount() { m_iConnectionCount++; }

        void DecrementConnectionCount();;

        void AdjustPosition(const glm::vec3 _v3) { if (!m_bIsPinned) m_v3Position += _v3; }
        void AdjustPinnedPosition(const glm::vec3 _v3) { m_v3Position += _v3; }

        // Getters-Setters
        glm::vec3 GetPos() { return m_v3Position; }
        void SetPos(const glm::vec3 _v3) { m_v3Position = _v3; }
        int GetConnectionCount() const { return m_iConnectionCount; }

        bool IsPinned() const { return m_bIsPinned; }
        void SetPin(bool _IsPinned) { m_bIsPinned = _IsPinned; }

        unsigned int GetID() const { return m_ID; }
	
        int GetVertexId() const { return m_iVertexId; }
        void SetVertexId(int _id) { m_iVertexId = _id; }

        int GetIndexId() const { return m_iIndexId; }
        void SetIndexId(int _id) { m_iIndexId = _id; }

        // Is Collided
        bool isCollided = false;

        // First point collision
        glm::vec3 first_point_col = glm::vec3(0.0f, 0.0f, 0.0f);


        bool GetIsAlive() const { return m_bIsAlive; }
	
        float GetHealth() const { return m_fHealth; }
        void SetHealth(float _health) { m_fHealth = _health; }

        bool IsOnFire() const { return m_bOnFire; }
        void SetOnFire(bool _onFire) {  m_bOnFire = _onFire; }

        float GetBurnTimer() const { return m_fBurnTimer; }
        void SetBurnTimer(float _burnTime) {  m_fBurnTimer = _burnTime; }
        
        
    private:

		// The particle id used to know which particle we are dealing with
        unsigned int m_ID = -1;

		// Keeps track of whether or not the particle lost all of its connections
        bool m_bIsAlive = true;

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
        float m_fMass = 0.1f;

        // Dampening value
        float m_fDampening = 0.01f;

		// Health
        float m_fHealth = 100.0f;

		// Fire stuff
        bool m_bOnFire = false;
        float m_fBurnTimer = 0.0f;
	
        // Vertex and Index
        int m_iVertexId{};
        int m_iIndexId{};
};
