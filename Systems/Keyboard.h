#pragma once

#define MAX_INPUT_KEY 255
#define MAX_INPUT_MOUSE 8

#define KeyDown_Up Keyboard::Get()->Down(VK_UP)
#define KeyDown_Down Keyboard::Get()->Down(VK_DOWN)
#define KeyDown_Left Keyboard::Get()->Down(VK_LEFT)
#define KeyDown_Right Keyboard::Get()->Down(VK_RIGHT)
#define KeyDown_Space Keyboard::Get()->Down(VK_SPACE)

#define KeyDown_Ctrl Keyboard::Get()->Down(VK_CONTROL)
#define KeyDown_X Keyboard::Get()->Down('X')



#define KeyPress_Up    Keyboard::Get()->Press(VK_UP)
#define KeyPress_Down  Keyboard::Get()->Press(VK_DOWN)
#define KeyPress_Left  Keyboard::Get()->Press(VK_LEFT)
#define KeyPress_Right Keyboard::Get()->Press(VK_RIGHT)

#define KeyUp_Up    Keyboard::Get()->Up(VK_UP)
#define KeyUp_Down  Keyboard::Get()->Up(VK_DOWN)
#define KeyUp_Left  Keyboard::Get()->Up(VK_LEFT)
#define KeyUp_Right Keyboard::Get()->Up(VK_RIGHT)

class Keyboard
{
public:
	static Keyboard* Get();

	static void Create();
	static void Delete();

	void Update();

	bool Down(DWORD key) { return keyMap[key] == KEY_INPUT_STATUS_DOWN; }
	bool Up(DWORD key) { return keyMap[key] == KEY_INPUT_STATUS_UP; }
	bool Press(DWORD key) { return keyMap[key] == KEY_INPUT_STATUS_PRESS; }

private:
	Keyboard();
	~Keyboard();

	static Keyboard* instance;

	byte keyState[MAX_INPUT_KEY];
	byte keyOldState[MAX_INPUT_KEY];
	byte keyMap[MAX_INPUT_KEY];

	enum
	{
		KEY_INPUT_STATUS_NONE = 0,
		KEY_INPUT_STATUS_DOWN,
		KEY_INPUT_STATUS_UP,
		KEY_INPUT_STATUS_PRESS,
	};
};