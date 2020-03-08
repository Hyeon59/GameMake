#pragma once

struct WinDesc
{
	wstring AppName;//윈도우창 이름
	HINSTANCE instance;
	HWND handle;
	UINT width; //윈도우창의 넓이
	UINT height;//윈도우창의 높이
};

class Window
{
public:
	Window(WinDesc desc);
	~Window();

	WPARAM Run(); 

private:
	static LRESULT CALLBACK WndProc(
		HWND handle, UINT message, WPARAM wParam, LPARAM lParam);

	static WinDesc desc;

	static class Program* program;
};

