#pragma once

class Gui
{
public:
	static void Create();
	static void Delete();

	static Gui* Get();

	LRESULT MsgProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
	void Resize();

	void Update();
	void Render();

private:
	Gui();
	~Gui();

private:
	void ApplyStyle();

private:
	static Gui* instance;
};