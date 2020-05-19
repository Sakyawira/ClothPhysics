#include "Camera.h"
#include <freeglut.h>

void Camera::something()
{

}

void Camera::use_camera(GLuint program)
{
	//	view = glm::mat4(glm::mat3(view));
		// Put view matrix into '*program'
	GLuint viewLoc = glGetUniformLocation(program, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	// Put the Projection (Orthographic/Perspective) Matrix into '*program'
	GLuint projLoc = glGetUniformLocation(program, "proj");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));

	GLuint camLoc = glGetUniformLocation(program, "camPos");
	glUniform3fv(camLoc, 1, glm::value_ptr(camPos));
}

glm::mat4 Camera::get_vp()
{
	return proj * view;
}

glm::vec3 Camera::get_position()
{
	return camPos;
}

void Camera::update(bool isGameStarted, float deltaTime, glm::vec3 _center)
{
	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
	direction.y = sin(glm::radians(pitch_));
	direction.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));
	camLookDir = glm::normalize(direction);

	if (isGameStarted)
	{
		//Move(_center);
		//center should be player position
		view = glm::lookAt(camPos, camPos + camLookDir, camUpDir);
		cameraRight = glm::normalize(glm::cross(camUpDir, camLookDir));
	}
	else
	{
		camPos.x = 0.0f;
		camPos.y = 1.5f;
		camPos.z = 26.0f;
		// View
		view = glm::lookAt(camPos, glm::vec3(0.0f, 0.0f, 0.0f), camUpDir);
	}
	// Rotate around a center point
	//else
	//{
	//	timeElapsed += deltaTime;
	//	GLfloat radius = 26.0f;

	//	camPos.x = sin(timeElapsed) * radius;
	//	camPos.y = 1.5f;
	//	camPos.z = cos(timeElapsed) * radius;
	//	// View
	//	view = glm::lookAt(camPos, glm::vec3(0.0f, 0.0f, 0.0f), camUpDir);
	//}
}

void Camera::move_pos_x(float i_magnitude, float deltaTime)
{
	//camPos.x += (1.0f * i_magnitude);
	camPos += i_magnitude * cameraRight * deltaTime;
}

void Camera::move_pos_y(float i_magnitude, float deltaTime)
{
	//camPos.y += (1.0f * i_magnitude);
	camPos -= i_magnitude * cameraRight * deltaTime;
}

void Camera::move_pos_z(float i_magnitude, float deltaTime)
{
	//camPos.z += (1.0f * i_magnitude);
	camPos += i_magnitude * camLookDir * deltaTime;
}



void Camera::set_pos_x(float i_magnitude)
{
	camPos.x = i_magnitude;
}

void Camera::set_pos_y(float i_magnitude)
{
	camPos.y = i_magnitude;
}

void Camera::set_pos_z(float i_magnitude)
{
	camPos.z = i_magnitude;
}

void Camera::set_look_dir(glm::vec3 new_look_dir)
{
	pitch_ = 0.0f;
	yaw_ = -90.0f;
	camLookDir = new_look_dir;
	view = glm::lookAt(camPos, camPos + camLookDir, camUpDir);
	cameraRight = glm::normalize(glm::cross(camUpDir, camLookDir));
}

void Camera::update_look_dir(int currentX, int currentY)
{
	float offsetX = currentX - last_x_;
	float offsetY = currentY - last_y_;

	last_x_ = 0;
	last_y_ = 0;

	const float sensitivity = 0.05f;

	offsetX *= sensitivity;
	offsetY *= sensitivity;

	yaw_ += offsetX;
	pitch_ += offsetY;

	if (pitch_ > 89.0f)
	{
		pitch_ = 89.0f;
	}
	if (pitch_ < -89.0f)
	{
		pitch_ = -89.0f;
	}	
	glutWarpPointer((int)400, (int)400);
}

float Camera::calculate_horizontal_distance()
{
	return static_cast<float>(m_distance_to_player * glm::cos(glm::radians(pitch_)));
}

float Camera::calculate_vertical_distance()
{
	return static_cast<float>(m_distance_to_player * glm::sin(glm::radians(pitch_)));
}

void Camera::move(glm::vec3 _center)
{
	float h_distance = calculate_horizontal_distance();
	float v_distance = calculate_vertical_distance();
	calculate_camera_position(h_distance, v_distance, _center);
}

void Camera::calculate_camera_position(float h_distance, float v_distance, glm::vec3 _center)
{
	float theta = /*player.getRotY() +*/ m_angle_around_player;
	float offsetX = h_distance * glm::sin(glm::radians(theta));
	float offsetZ = h_distance * glm::cos(glm::radians(theta));
	camPos.x = _center.x - offsetX;
	camPos.z = _center.z - offsetZ;
	camPos.y = _center.y + v_distance;
}

glm::mat4 Camera::get_projection()
{
	return proj;
}

glm::mat4 Camera::get_view()
{
	return view;
}

glm::vec3 Camera::get_look_dir()
{
	return camLookDir;
}


