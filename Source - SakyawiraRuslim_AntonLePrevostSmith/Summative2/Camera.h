/***********************
  Bachelor of Software Engineering
  Media Design School
  Auckland
  New Zealand

  (c) 2018 Media Design School

  File Name   :   Camera.h
  Description :   declare and define the Camera class
  Author      :   Sakyawira Nanda Ruslim
  Mail        :   Sakyawira.Rus8080@mediadesign.school.nz
********************/
#pragma once
#include <glew.h>
#include <vector>
#include <mat4x2.hpp>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_transform.hpp>

class Camera
{
public:
	Camera() = default;
	~Camera() = default;
	void something();

	void use_camera(GLuint program/*, glm::mat4 view, glm::mat4 proj*/);

	glm::mat4 get_vp();

	glm::vec3 get_position();

	void update(bool isGameStarted, float deltaTime, glm::vec3 _center);

	void move_pos_x(float i_magnitude, float deltaTime);

	void move_pos_y(float i_magnitude, float deltaTime);

	void move_pos_z(float i_magnitude, float deltaTime);

	void set_pos_x(float i_magnitude);

	void set_pos_y(float i_magnitude);

	void set_pos_z(float i_magnitude);

	void set_look_dir(glm::vec3 new_look_dir);

	void calculate_zoom();

	void calculate_pitch();

	void calculate_angle_around_player();

	void update_look_dir(int currentX, int currentY);

	float calculate_horizontal_distance();

	float calculate_vertical_distance();

	void move(glm::vec3 _center);
	
	void calculate_camera_position(float h_distance, float v_distance, glm::vec3 _center);

	glm::mat4 get_projection();

	glm::mat4 get_view();

	glm::vec3 get_look_dir();
	
private:

	// Distance from player
	float m_distance_to_player = -65.0f;

	// angle around player
	float m_angle_around_player = 0.0f;

	// Pitch
	float pitch_ = 0.0f;

	// Yaw
	float yaw_ = - 90.0f;

	// Time Elapse
	float time_elapsed_ = 0.0f;

	float last_x_ = 400;
	float last_y_ = 400;
	
	// Camera
	 glm::vec3 camPos = glm::vec3(0.0f, 0.0f, 0.0f);
	 glm::vec3 camLookDir = glm::vec3(0.0f, 0.0f, -1.0f);
	 glm::vec3 camUpDir = glm::vec3(0.0f, 1.0f, 0.0f);
	 glm::vec3 cameraRight = glm::normalize(glm::cross(camUpDir, camLookDir));

	// View Matrix 
	glm::mat4 view = glm::lookAt(camPos, glm::vec3(0.0f, 0.0f, 0.0f), camUpDir);

	// Screen Size
	 unsigned int SCR_WIDTH = 800;
	 unsigned int SCR_HEIGHT = 800;

	 float halfScreenWidth = static_cast<float>(SCR_WIDTH) * 0.5f;
	 float halfScreenHeight = static_cast<float>(SCR_HEIGHT) * 0.5f;

	// Projection Space

	// Perspective
	glm::mat4 proj = glm::perspective(45.0f, static_cast<float>(SCR_WIDTH) / static_cast<float>(SCR_HEIGHT), 0.1f, 40000.0f);

	// Orthographic
	// glm::mat4 proj = glm::ortho(-halfScreenWidth, halfScreenWidth, -halfScreenHeight, halfScreenHeight, 0.1f, 100.0f);
};
