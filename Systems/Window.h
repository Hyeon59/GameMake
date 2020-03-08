#pragma once

struct WinDesc
{
	wstring AppName;//������â �̸�
	HINSTANCE instance;
	HWND handle;
	UINT width; //������â�� ����
	UINT height;//������â�� ����
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

