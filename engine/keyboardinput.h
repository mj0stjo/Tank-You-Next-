#ifndef KEYBOARDINPUT_H
#define KEYBOARDINPUT_H

static class KeyboardInput
{
public:
	KeyboardInput();
	~KeyboardInput();
	static bool IsPressed(char key);
	static void setKey(char key, bool state);
private:
	static bool* keyStates;
};
#endif // !INPUT_H
