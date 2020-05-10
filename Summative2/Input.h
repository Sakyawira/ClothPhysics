#pragma once
#include <iostream>
#include "GameManager.h"
#include <freeglut.h>
#include <math.h>
#include "clock.h"
#include "Audio.h"

class Input
{
public:
	Input() = default;
	~Input() = default;

	void process_input(GameManager* Game, Audio& audio);

	void mouse_click(int button, int state, int x, int y);

	void mouse_passive_move(int x, int y, GameManager* Game);

	void mouse_move(int x, int y, GameManager * Game);

	void keyboard_down(unsigned char key, int x, int y);

	void keyboard_up(unsigned char key, int x, int y);

	void special_keyboard_down(int key, int x, int y);

	void special_keyboard_up(int key, int x, int y);

private:

	// Define Input
	enum InputState
	{
		INPUT_UP,
		INPUT_DOWN,
		INPUT_UP_FIRST,
		INPUT_DOWN_FIRST,
	};

	// Input Container
	InputState key_state_[255];

	InputState special_key_state_[255];

	InputState mouse_state_[3];
};


