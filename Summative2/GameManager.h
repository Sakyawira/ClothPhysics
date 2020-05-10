/***********************
  Bachelor of Software Engineering
  Media Design School
  Auckland
  New Zealand

  (c) 2018 Media Design School

  File Name   :   GameObject.h
  Description :   contains declaration of GameManager class which control the gameplay, rendering of all game objects
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
#include "Texture.h"
#include "Camera.h"
#include "TextLabel.h"
#include "clock.h"
#include "Audio.h"
#include "Sphere.h"
#include "CubeMap.h" 
#include "Model.h"
#include "Terrain.h"
#include "GeometryModel.h"
#include "TessModel.h"
#include "FrameBuffer.h"

const int WINDOW_WIDHT = 800;
const int WINDOW_HEIGHT = 800;

class GameManager
{
public:
	GameManager();
	~GameManager();

	void initialize();
	
	void process_game(Audio& audio);

	void render();

	bool is_started() const;

	void start_game(bool _isStart);

	CClock* get_clock() const;

	GameObject* update_mouse_picking();

	Camera camera;

	void set_mouse_pos(glm::vec2 mousePos_);

	void set_click(bool newState);

	void cube_follow_terrain();

	void all_mouse_pick(float delta_t);

	GameObject* stencilCube;

	// Control polygon mode
	bool m_b_wireframe = false;
	
private:

	// Is click
	bool m_is_clicked_ = false;
	
	// Ray direction
	glm::vec3 m_ray_direction_;
	glm::vec2 m_mouse_pos_;
	
	// Utility functions
	void create_spheres(int _number_coins, int _border);
	
	// Clock
	CClock* m_clock_;

	// Check whether or not m_program has been initialized
	bool m_b_initialized_ = false;
	
	// Control whether the game has been started
	bool m_b_start_ = false;
	
	// Declaring Strings
	std::string m_string_score_;
	TextLabel* m_text_instruction_;
	std::string m_string_lives_;
	TextLabel* m_text_lives_;
	std::string m_string_level_;
	TextLabel* m_text_level_;

	// Menu text
	char m_c_bg_ = 65;
	std::string m_string_bg_;
	TextLabel* m_text_bg_;

	// Menu Text
	std::string m_string_menu_ = "Collect All Crystal Balls to Win: The Game";
	TextLabel* m_text_menu_;

	// Instruction Text
	std::string m_string_instruction_ = "Press 'R' to start the game...";
	TextLabel* m_text_collision_;

	// Uniforms
	GLfloat current_time_;
	float frame_counts_ = 0;

	Shader* m_sh_fogBox;
	Shader* m_sh_lod_;
	Shader* m_sh_fog_;
	Shader* m_sh_phong_diffuse_;
	Shader* m_sh_phong_specular_;
	Shader* m_sh_phong_rim_;
	Shader* m_sh_cube_map_;
	Shader* m_sh_reflective_;
	Shader* m_sh_geometry_;
	Shader* m_sh_star_geo_;
	Shader* m_sh_tess_;
	Shader* m_sh_chromatical;

	FrameBuffer* m_frameBuffer;

	Texture* m_tr_down;
	Texture* m_tr_plain;
	Texture* m_tr_up;
	Texture* m_tr_water;
	Texture* m_tr_grass;
	CubeMap* m_tr_cube_map;

	Mesh* m_mesh_animate;
	Mesh* m_mesh_frameBuffer;
	Mesh* m_mesh_scroll;
	Mesh* m_mesh_player;
	Mesh* m_mesh_static;
	Mesh* m_mesh_pyramid;
	Mesh* m_mesh_cube;
	Mesh* m_mesh_cube_map;
	Sphere* m_mesh_sphere;
	Terrain* m_mesh_terrain;

	Model* m_mdl_tank;
	Model* m_mdl_cat;
	GeometryModel* geomModel;
	GeometryModel* starModel;
	TessModel* tessModel;
	TessModel* lod_tessModel;

	GameObject* tank;

	GameObject* m_coin;

	GameObject* button_down;
	GameObject* button_up;
	GameObject* sphere;
	GameObject* sky_box;

	GameObject* stencilCube2;

	GameObject* transparentCube;

	GameObject* terrain;
	
	// Vectors
	std::vector<GameObject*> m_v_geometry;
	std::vector<GameObject*> m_v_cubes;
	std::vector<GameObject*> m_v_sphere;
	std::vector<Shader*> m_v_sh;
	std::vector<Mesh*> m_v_mesh;
	std::vector<TextLabel*> m_v_text;

	std::vector<GLfloat> static_vertices =
	{
		// Position				// Color			// Texture Coords
		-1.0f,  1.0f, 0.0f,		0.0f, 1.0f,	1.0f,	0.0f, 0.0f,			// Top-Left
		 1.0f,  1.0f, 0.0f,		0.0f, 0.0f,	1.0f,	1.0f, 0.0f,			// Top-Right
		-1.0f, -1.0f, 0.0f,		1.0f, 0.0f, 0.0f,	0.0f, 1.0f,			// Bottom-Left
		 1.0f, -1.0f, 0.0f,		0.0f, 1.0f, 0.0f,	1.0f, 1.0f,			// Bottom-Right
	};

	std::vector<GLfloat> player_animation_vertices =
	{
		// Position				// Color			// Texture Coords
		-0.5f,  0.5f, 0.0f,		0.0f, 1.0f,	1.0f,	0.0f, 0.001f,		// Top-Left
		 0.5f,  0.5f, 0.0f,		0.0f, 0.0f,	1.0f,	0.125f, 0.001f,		// Top-Right
		-0.5f, -0.5f, 0.0f,		1.0f, 0.0f, 0.0f,	0.0f, 0.2f,			// Bottom-Left
		 0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f,	0.125f, 0.2f,		// Bottom-Right
	};

	std::vector<GLfloat> enemy2_vertices =
	{
		// Position				// Color			// Texture Coords
		-0.5f,  0.5f, 0.0f,		0.0f, 1.0f,	1.0f,	0.0f, 0.2f,			// Top-Left
		 0.5f,  0.5f, 0.0f,		0.0f, 0.0f,	1.0f,	0.125f, 0.2f,		// Top-Right
		-0.5f, -0.5f, 0.0f,		1.0f, 0.0f, 0.0f,	0.0f, 0.4f,			// Bottom-Left
		 0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f,	0.125f, 0.4f,		// Bottom-Right
	};

	// Animation has 4 textures in one image
	std::vector<GLfloat> animation_vertices =
	{
		// Position				// Color			// Texture Coords
		-0.5f,  0.5f, 0.0f,		0.0f, 1.0f,	1.0f,	0.0f, 0.4f,			// Top-Left
		 0.5f,  0.5f, 0.0f,		0.0f, 0.0f,	1.0f,	0.125f, 0.4f,		// Top-Right
		-0.5f, -0.5f, 0.0f,		1.0f, 0.0f, 0.0f,	0.0f, 0.6f,			// Bottom-Left
		 0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f,	0.125f, 0.6f,		// Bottom-Right
	};

	std::vector<GLfloat> water_scrolling_vertices =
	{
		// Position				// Color			// Texture Coords
		-0.5f,  0.5f, 0.0f,		0.0f, 1.0f,	1.0f,	0.0f, 0.4f,			// Top-Left
		 0.5f,  0.5f, 0.0f,		0.0f, 0.0f,	1.0f,	1.0f, 0.4f,			// Top-Right
		-0.5f, -0.5f, 0.0f,		1.0f, 0.0f, 0.0f,	0.0f, 0.6f,			// Bottom-Left
		 0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f,	1.0f, 0.6f,			// Bottom-Right
	};

	// Pyramid
	std::vector<GLfloat> pyramid_vertices =
	{
		// Position				// Color			// Texture Coords
		-0.5f,  0.0f, -0.5f,	0.0f, 1.0f,	0.0f,	0.0f, 1.0f,			// Top-Left
		-0.5f,  0.0f,  0.5f,	0.0f, 1.0f,	0.0f,	0.0f, 0.0f,			// Top-Right
		 0.5f,  0.0f,  0.5f,	0.0f, 1.0f, 0.0f,	1.0f, 0.0f,			// Bottom-Left
		 0.5f,  0.0f, -0.5f,	0.0f, 1.0f, 0.0f,	1.0f, 0.0f,			// Bottom-Right

		 // Side 1
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f, 0.0f,	1.0f, 1.0f,			// Middle-Top
		 0.5f, 0.0f, -0.5f,		0.0f, 1.0f, 0.0f,	0.0f, 1.0f,

		 // Side 2
		 0.5f, 0.0f, -0.5f,		0.0f, 1.0f, 0.0f,	1.0f, 1.0f,
		 0.5f, 0.0f,  0.5f,		0.0f, 1.0f, 0.0f,	0.0f, 1.0f,

		 // Side 3
		 0.5f, 0.0f,  0.5f,		0.0f, 1.0f, 0.0f,	1.0f, 1.0f,
		-0.5f, 0.0f,  0.5f,		0.0f, 1.0f, 0.0f,	0.0f, 1.0f,

		// Side 4
		-0.5f, 0.0f,  0.5f,		0.0f, 1.0f, 0.0f,	1.0f, 1.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f, 0.0f,	0.0f, 1.0f,

		// Top
		0.0f, 0.5f,  0.0f,		0.0f, 1.0f, 1.0f,	0.5f, 0.0f,
	};

	// Cube Vertices
	std::vector <GLfloat> cube_vertices =
	{
		// Position				// Color			// Texture Coords
		// Front
		-0.5f,  0.5f,  0.5f,	0.0f, 1.0f,	0.0f,	0.0f, 0.0f,			// Top-Left
		-0.5f, -0.5f,  0.5f,	0.0f, 1.0f,	0.0f,	0.0f, 1.0f,			// Top-Right
		 0.5f, -0.5f,  0.5f,	0.0f, 1.0f, 0.0f,	1.0f, 1.0f,			// Bottom-Left
		 0.5f,  0.5f,  0.5f,	0.0f, 1.0f, 0.0f,	1.0f, 0.0f,			// Bottom-Right

		 // Side 1
		-0.5f, 0.5f, -0.5f,		0.0f, 1.0f, 0.0f,	1.0f, 0.0f,			// Middle-Top
		-0.5f,-0.5f, -0.5f,		0.0f, 1.0f, 0.0f,	1.0f, 1.0f,
		 0.5f,-0.5f, -0.5f,		0.0f, 1.0f, 0.0f,	0.0f, 1.0f,			// Middle-Top
		 0.5f, 0.5f, -0.5f,		0.0f, 1.0f, 0.0f,	0.0f, 0.0f,

		 // Left
		-0.5f, 0.5f, -0.5f,		0.0f, 1.0f, 0.0f,	0.0f, 0.0f,			// Middle-Top
		-0.5f,-0.5f, -0.5f,		0.0f, 1.0f, 0.0f,	0.0f, 1.0f,
		-0.5f,-0.5f,  0.5f,		0.0f, 1.0f, 0.0f,	1.0f, 1.0f,			// Middle-Top
		-0.5f, 0.5f,  0.5f,		0.0f, 1.0f, 0.0f,	1.0f, 0.0f,

		 // Right
		 0.5f, 0.5f,  0.5f,		0.0f, 1.0f, 0.0f,	0.0f, 0.0f,			// Middle-Top
		 0.5f,-0.5f,  0.5f,		0.0f, 1.0f, 0.0f,	0.0f, 1.0f,
		 0.5f,-0.5f, -0.5f,		0.0f, 1.0f, 0.0f,	1.0f, 1.0f,			// Middle-Top
		 0.5f, 0.5f, -0.5f,		0.0f, 1.0f, 0.0f,	1.0f, 0.0f,

		 // Top
		 -0.5f,  0.5f, -0.5f,	0.0f, 1.0f,	0.0f,	0.0f, 0.0f,			// Top-Left
		 -0.5f,  0.5f,  0.5f,	0.0f, 1.0f,	0.0f,	0.0f, 1.0f,			// Top-Right
		  0.5f,  0.5f,  0.5f,	0.0f, 1.0f, 0.0f,	1.0f, 1.0f,			// Bottom-Left
		  0.5f,  0.5f, -0.5f,	0.0f, 1.0f, 0.0f,	1.0f, 0.0f,			// Bottom-Right

		  // Bottom
		 -0.5f, -0.5f,  0.5f,	0.0f, 1.0f, 0.0f,	0.0f, 0.0f,			// Middle-Top
		 -0.5f, -0.5f, -0.5f,	0.0f, 1.0f, 0.0f,	0.0f, 1.0f,
		  0.5f, -0.5f, -0.5f,	0.0f, 1.0f, 0.0f,	1.0f, 1.0f,			// Middle-Top
		  0.5f, -0.5f,  0.5f,	0.0f, 1.0f, 0.0f,	1.0f, 0.0f,
	};

	// Cube Vertices
	std::vector <GLfloat> cube_map_vertices =
	{
		//// Position			
		// Front
		-1.0f,  1.0f,  1.0f,		// 00
		-1.0f, -1.0f,  1.0f,		// 01
		 1.0f, -1.0f,  1.0f,		// 02
		 1.0f,  1.0f,  1.0f,		// 03

		 // Side 1
		-1.0f, 1.0f, -1.0f,			// 04
		-1.0f,-1.0f, -1.0f,			// 05
		 1.0f,-1.0f, -1.0f,			// 06
		 1.0f, 1.0f, -1.0f,			// 07

		// // Left
		//-1.0f, 1.0f, -1.0f,			// 08
		//-1.0f,-1.0f, -1.0f,			// 09
		//-1.0f,-1.0f,  1.0f,			// 10
		//-1.0f, 1.0f,  1.0f,			// 11

		//// Right
		//1.0f, 1.0f,  1.0f,			// 12
		//1.0f,-1.0f,  1.0f,			// 13
		//1.0f,-1.0f, -1.0f,			// 14
		//1.0f, 1.0f, -1.0f,			// 15

		//// Top
		//-1.0f,  1.0f, -1.0f,		// 16
		//-1.0f,  1.0f,  1.0f,		// 17
		// 1.0f,  1.0f,  1.0f,		// 18
		// 1.0f,  1.0f, -1.0f,		// 19

		// // Bottom
		//-1.0f, -1.0f,  1.0f,		// 20
		//-1.0f, -1.0f, -1.0f,		// 21
		// 1.0f, -1.0f, -1.0f,		// 22
		// 1.0f, -1.0f,  1.0f,		// 23
	};

	// For a button_up
	std::vector <GLuint> cube_indices =
	{
		0, 1, 2,
		0, 2, 3,

		7, 6, 5,
		7, 5, 4,

		8,  9, 10,
		8, 10, 11,

		12, 13, 14,
		12, 14, 15,

		16, 17, 18,
		16, 18, 19,
		
		20, 21, 22,
		20, 22, 23,
	};

	std::vector <GLuint> cube_map_indices =
	{	
		4, 5, 1,
		4, 1, 0,

		3, 2, 6,
		3, 6, 7,

		4, 0, 3,
		4, 3, 7,

		1, 5, 6,
		1, 6, 2,

		0, 1, 2, // works
		0, 2, 3, // works 1

		7, 6, 5, // works 2
		7, 5, 4,
	};

	// Pyramid Indices
	std::vector<GLuint> pyramid_indices =
	{
		1, 0, 3,	// Base Triangle 1
		1, 3, 2,	// Base Triangle 2
		
		4, 12, 5,	// Side 1
		6, 12, 7,	// Side 2

		8, 12, 9,	// Side 3
		10, 12, 11,	// Side 4
	};
	
	// indices reuses the four unique values to create shapes
	std::vector<GLuint> indicesVector =
	{
		0, 4, 3,
		0, 1, 2, // First Triangle
		2, 3, 0, // Second Triangle
		1, 5, 2,
	};

	std::vector<GLuint> animation_indices =
	{
		1, 2, 3, // First Triangle
		1, 0, 2, // Second Triangle
	};

	float m_cube_previous_y = 0.0f;
};

