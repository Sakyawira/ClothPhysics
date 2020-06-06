#include "Input.h"

void Input::process_input(GameManager* Game, Audio& audio)
{

	if (key_state_['w'] == INPUT_DOWN_FIRST)
	{
		key_state_['w'] = INPUT_DOWN;
	}

	if (key_state_['s'] == INPUT_DOWN_FIRST)
	{
		key_state_['s'] = INPUT_DOWN;
	}

	if (key_state_['a'] == INPUT_DOWN_FIRST)
	{
		key_state_['a'] = INPUT_DOWN;
	}

	if (key_state_['d'] == INPUT_DOWN_FIRST)
	{
		key_state_['d'] = INPUT_DOWN;
	}
	if (key_state_['q'] == INPUT_DOWN_FIRST)
	{
		key_state_['q'] = INPUT_DOWN;
	}

	if (key_state_['e'] == INPUT_DOWN_FIRST)
	{
		key_state_['e'] = INPUT_DOWN;
	}
	if (key_state_['z'] == INPUT_DOWN_FIRST)
	{
		key_state_['z'] = INPUT_DOWN;
	}

	if (key_state_['c'] == INPUT_DOWN_FIRST)
	{
		key_state_['c'] = INPUT_DOWN;
	}

	if (key_state_['w'] == INPUT_DOWN)
	{
		Game->camera.move_pos_z(15.0f, Game->get_clock()->GetDeltaTick());
		if (!Game->is_started())
		{
			Game->sphere->Move(MOVE_FRONT, 12.0f * Game->get_clock()->GetDeltaTick());
		}
	}
	if (key_state_['s'] == INPUT_DOWN)
	{
		if (!Game->is_started())
		{
			Game->sphere->Move(MOVE_BACK, 12.0f * Game->get_clock()->GetDeltaTick());
		}
		else
		{
			Game->camera.move_pos_z(-15.0f, Game->get_clock()->GetDeltaTick());
		}
	}
	if (key_state_['d'] == INPUT_DOWN)
	{
		if (!Game->is_started())
		{
			Game->sphere->Move(MOVE_RIGHT, 12.0f * Game->get_clock()->GetDeltaTick());
		}
		else
		{
			Game->camera.move_pos_x(-15.0f, Game->get_clock()->GetDeltaTick());
		}
	}
	if (key_state_['a'] == INPUT_DOWN)
	{
		if (!Game->is_started())
		{
			Game->sphere->Move(MOVE_LEFT, 12.0f * Game->get_clock()->GetDeltaTick());
		}
		else
		{
			Game->camera.move_pos_x(15.0f, Game->get_clock()->GetDeltaTick());
		}
	}
	if (key_state_['z'] == INPUT_DOWN)
	{
		if (!Game->is_started())
		{
			Game->sphere->Move(MOVE_DOWN, 12.0f * Game->get_clock()->GetDeltaTick());
		}
	}
	if (key_state_['c'] == INPUT_DOWN)
	{
		if (!Game->is_started())
		{
			Game->sphere->Move(MOVE_UP, 12.0f * Game->get_clock()->GetDeltaTick());
		}
	}

	if (key_state_['q'] == INPUT_DOWN)
	{
		Game->m_mesh_cloth->Squish(-1);
	}

	if (key_state_['e'] == INPUT_DOWN)
	{
		Game->m_mesh_cloth->Squish(1);
	}

	if (key_state_['r'] == INPUT_DOWN_FIRST)
	{
		if (!Game->is_started() /*&& !Game->IsEnded()*/)
		{
			Game->start_game(true);
		}
		else
		{
			// Start working on initializing the game on restart instead of making a new one
			Game->start_game(false);
			Game->initialize();
		}
		key_state_['r'] = INPUT_DOWN;
	}
	if (key_state_['f'] == INPUT_DOWN_FIRST)
	{
		//if (!Game->is_started() /*&& !Game->IsEnded()*/)
		//{
		//	Game->start_game(true);
		//}
		//else
		//{
		//	// Start working on initializing the game on restart instead of making a new one
		//	Game->start_game(false);
		//	Game->initialize();
		//}
		if (Game->sphere->GetMesh() == Game->m_mesh_sphere)
		{
			Game->sphere->SetMesh(Game->m_mesh_cube);
		}
		else
		{
			Game->sphere->SetMesh(Game->m_mesh_sphere);
		}
		
		key_state_['f'] = INPUT_DOWN;
	}

	if (key_state_[32] == INPUT_DOWN_FIRST)
	{
	/*	if (Game->m_b_wireframe == false)
		{
			Game->m_b_wireframe = true;
		}
		else
		{
			Game->m_b_wireframe = false;
		}*/
		Game->m_mesh_cloth->Unpin();
		key_state_[32] = INPUT_DOWN;
	}

	if (special_key_state_[GLUT_KEY_UP] == INPUT_DOWN_FIRST)
	{
		Game->wind_force.z += 0.5f;
		special_key_state_[GLUT_KEY_UP] = INPUT_DOWN;
	}
	if (special_key_state_[GLUT_KEY_DOWN] == INPUT_DOWN_FIRST)
	{
		Game->wind_force.z -= 0.5f;
		special_key_state_[GLUT_KEY_DOWN] = INPUT_DOWN;
	}
	if (special_key_state_[GLUT_KEY_LEFT] == INPUT_DOWN_FIRST)
	{
		Game->wind_force.x -= 0.5f;
		special_key_state_[GLUT_KEY_LEFT] = INPUT_DOWN;
	}
	if (special_key_state_[GLUT_KEY_RIGHT] == INPUT_DOWN_FIRST)
	{
		Game->wind_force.x += 0.5f;
		special_key_state_[GLUT_KEY_RIGHT] = INPUT_DOWN;
	}

	if (key_state_['o'] == INPUT_DOWN_FIRST)
	{
		Game->wind_force.y -= 0.5f;
		key_state_['o'] = INPUT_DOWN;
	}

	if (key_state_['p'] == INPUT_DOWN_FIRST)
	{
		Game->wind_force.y += 0.5f;
		key_state_['p'] = INPUT_DOWN;
	}

	if (mouse_state_[0] == INPUT_DOWN)
	{
		Game->set_click(true);
	}
	else
	{
		Game->set_click(false);
	}
}

void Input::mouse_click(int button, int state, int x, int y)
{
	if (button >= 3)
	{
		return;
	}

	mouse_state_[button] = (state == GLUT_DOWN) ? INPUT_DOWN : INPUT_UP;
	//std::cout << "Clicked Once x: " << x << " | y: " << y << std::endl;
}

void Input::mouse_passive_move(int x, int y, GameManager * Game)
{
	/*For Mouse Picking*/
	const float currentMX = (2.0f * x) / static_cast<float>(800) - 1.0f;
	const float currentMY = 1.0f - (2.0f * y) / static_cast<float>(800);
	Game->set_mouse_pos(glm::vec2(currentMX, currentMY));

	/*For Free Moving Camera*/
	//Convert current screen width and height mouse co-ords to 
	x -= 400;
	y -= 400;

	//Invert y axis
	y *= -1;

	if (Game->is_started())
	{
		Game->camera.update_look_dir(x, y);
	}
}

void Input::mouse_move(int x, int y, GameManager * Game)
{
	/*For Mouse Picking*/
	const float currentMX = (2.0f * x) / static_cast<float>(800) - 1.0f;
	const float currentMY = 1.0f - (2.0f * y) / static_cast<float>(800);
	Game->set_mouse_pos(glm::vec2(currentMX, currentMY));

	/*For Free Moving Camera*/
	//Convert current screen width and height mouse co-ords to 
	x -= 400;
	y -= 400;

	//Invert y axis
	y *= -1;

	if (Game->is_started())
	{
		Game->camera.update_look_dir(x, y);
	}
	//std::cout << "Clicked x: " << x << " | y: " << y << std::endl;
}

void Input::keyboard_down(unsigned char key, int x, int y)
{
	if (key_state_[key] != INPUT_DOWN || key_state_[key] != INPUT_DOWN_FIRST)
	{
		key_state_[key] = INPUT_DOWN_FIRST;
	}
}

void Input::keyboard_up(unsigned char key, int x, int y)
{
	key_state_[key] = INPUT_UP;
}

void Input::special_keyboard_down(int key, int x, int y)
{
	special_key_state_[key] = INPUT_DOWN_FIRST;
}

void Input::special_keyboard_up(int key, int x, int y)
{
	special_key_state_[key] = INPUT_UP;
}
