/***********************
  Bachelor of Software Engineering
  Media Design School
  Auckland
  New Zealand

  (c) 2018 Media Design School

  File Name   :   GameObject.h
  Description :   contains declaration of GameObject class which control the updates and drawing pipeline of an object
  Author      :   Sakyawira Nanda Ruslim
  Mail        :   Sakyawira.Rus8080@mediadesign.school.nz
********************/
#pragma once

// Library Includes
#include <vector>

// Dependency Includes
#include <glew.h>
#include <gtc/type_ptr.hpp>
#include <glm.hpp>
#include <mat4x2.hpp>
//#include <gtc/matrix_transform.inl>

// Local Includes
#include "Shader.h"
#include "Mesh.h"
//#include "CubeMap.h"
#include "Camera.h"
#include "Model.h"

// Move identifiers
#define	MOVE_UP      0
#define	MOVE_DOWN    1
#define MOVE_RIGHT   2
#define	MOVE_LEFT    3
#define MOVE_FRONT	 4
#define MOVE_BACK	 5

// Used for returning coordinates
typedef std::pair<float, float> Coordinate;

// Coordinate identifiers
#define TOP			0
#define BOTTOM		1
#define LEFT		2
#define RIGHT		3

class CubeMap;

class GameObject
{
	public:
		GameObject() = default;
	
		GameObject(Shader* _shader, Mesh* _mesh, std::vector<Texture*>& _textures, float _initial_x, float _initial_y, float _initial_z , std::vector<GameObject*>& objectVector);

		GameObject(Model* _model, float _initial_x, float _initial_y, float _initial_z);
	
		~GameObject() = default;
	
		// OverLoad that takes one uniform
		void Draw(Camera& _camera, const GLchar* s_currentTime, GLfloat f_currentTime, float deltaTime);
	
		// Overload that takes two uniforms
		void Draw(Camera& _camera, const GLchar* s_currentTime, GLfloat f_currentTime, const GLchar* s_frameTime, GLint i_frameTime, float deltaTime);

		// Overload for drawing button_up maps
		void Draw(Camera& _camera, float deltaTime);

		void Draw(Camera& _camera, const GLchar* s_currentTime, GLfloat f_currentTime, const GLchar* s_frameTime, GLint i_frameTime, CubeMap* cube_map, float deltaTime);

		void draw_with_model(float deltaTime);

		bool sphere_sphere_col(GameObject* a, GameObject* b);

		bool ray_box_col(glm::vec3 ray_origin, glm::vec3 ray_direction, float ray_length);

		bool ray_sphere_col(glm::vec3 ray_origin, glm::vec3 ray_direction, float ray_length);
	
	/*	void SetPosition(float _x_pos, float _y_pos);*/
	
		float GetPosition(int COORDINATE_ID);

		glm::vec3 GetLocation();
	
		void Move(int MOVE_ID, float SPEED);

		glm::vec3 GetScale();

		void Scale(float _scale);
		void Scale(float _scaleX, float _scaleY, float _scaleZ);
	
		void Enable();
	
		void Disable();

		bool m_currently_moved = false;

		void Rotate(float _angle);

		float GetRotate();

		void SetPos(glm::vec3 _newPos);

		glm::vec3 GetExtents();

		glm::vec3 GetMin();

		glm::vec3 GetMax();
	
	protected:

		// Enable Drawing
		bool m_bool_enable = true;
		
		// Shader
		Shader* m_shader;
	
		// Mesh
		Mesh* m_mesh;

		// Model
		Model* m_model;
	
		// Texture Vector
		std::vector<Texture*> m_textures;
	
		// Position
		float m_xPos = 0.0f;
		float m_yPos = 0.0f;
		float m_zPos = 0.0f;
	
		// Scale
		glm::vec3 m_scale = glm::vec3(1.0f, 1.0f, 1.0f);
		
		// Translation Matrix
		glm::vec3 m_objPosition = glm::vec3(m_xPos, m_yPos, m_zPos);
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
