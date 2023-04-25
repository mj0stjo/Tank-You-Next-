#include "./engine/keyboardinput.h"


bool* KeyboardInput::keyStates = new bool[255]();

KeyboardInput::KeyboardInput()
{
}

KeyboardInput::~KeyboardInput()
{
}


bool KeyboardInput::IsPressed(char key)
{
	return keyStates[key];
}

void KeyboardInput::setKey(char key, bool state)
{
	keyStates[key] = state;
}

