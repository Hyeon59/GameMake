#include "stdafx.h"
#include "Window.h"
#include "Program.h"

WinDesc Window::desc;
Program* Window::program = nullptr;

HINSTANCE instance;
Window::Window(WinDesc desc)
{
	WNDCLASSEX wndClass;

	wndClass.cbClsExtra = 0; //���࿵�� ������
	wndClass.cbWndExtra = 0; //���࿵�� ������
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); //������ ������
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW); //������ ���콺 Ŀ��
	wndClass.hIcon = LoadIcon(NULL, IDI_WINLOGO); //������ ������
	wndClass.hIconSm = wndClass.hIcon; //������ ������ ������
	wndClass.hInstance = desc.instance; // ������ �ν��Ͻ� �ڵ�
	wndClass.lpfnWndProc = (WNDPROC)WndProc; // ������ �޼��� ó���Լ� ����
	wndClass.lpszClassName = desc.AppName.c_str(); //������ Ŭ�����̸�
	wndClass.lpszMenuName = NULL; //������ �޴� �̸�
	wndClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC; //������ ��Ÿ��
	wndClass.cbSize = sizeof(WNDCLASSEX); //����Ȯ�ο�

	WORD wHr = RegisterClassEx(&wndClass);
	
	desc.handle = CreateWindowEx
	(
		WS_EX_APPWINDOW //Ȯ�� ��Ÿ�� �ɼ�
		, desc.AppName.c_str() //������ Ŭ���� �̸�
		, desc.AppName.c_str() //������ Ÿ��Ʋ�� �̸�
		, WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_OVERLAPPEDWINDOW //������ ��Ÿ��
		, CW_USEDEFAULT			//�����츦 ������ x��ǥ
		, CW_USEDEFAULT			//�����츦 ������ y��ǥ
		, CW_USEDEFAULT			//������ ����
		, CW_USEDEFAULT			//������ ����
		, NULL					//������ ������ �� �����츦 ���� �θ��� �ڵ�
		, (HMENU)NULL			//�޴� Ȥ�� �ڽ� �������� �ĺ��� ID
		, desc.instance			//�����츦 ������ �ν��Ͻ� �ڵ�
		, NULL					//MDI Ŭ���̾�Ʈ �������. ��� x
	);

	RECT rect = { 0,0,(LONG)desc.width, (LONG)desc.height };

	UINT centerX = (GetSystemMetrics(SM_CXSCREEN) - (UINT)desc.width ) / 2;
	UINT centerY = (GetSystemMetrics(SM_CYSCREEN) - (UINT)desc.height) / 2;

	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);
	
	//�������� ũ��� ��ġ�� ���ÿ� �����ϴ� �Լ�
	MoveWindow
	(
		desc.handle //�̵���� ������ �ڵ�
		, centerX, centerY //ȭ�� �߽��� ��ǥ
		, rect.right - rect.left, rect.bottom - rect.top//������ ũ��(����:�ȼ�)
		, TRUE
	);

	ShowWindow(desc.handle, SW_SHOWNORMAL); //�������� ���̴� ���� ����
	SetForegroundWindow(desc.handle); //���� ���鿡 ���� ������ ����
	SetFocus(desc.handle); //������ ��Ŀ�� ����

	ShowCursor(true);

	Window::desc = desc;
	instance = desc.instance;
}

Window::~Window()
{
	DestroyWindow(desc.handle);
	UnregisterClass(desc.AppName.c_str(), desc.instance);
}

WPARAM Window::Run()
{
	MSG msg = { 0 };

	Graphics::Create();
	Keyboard::Create();
	Mouse::Create();
	Time::Create();
	Gui::Create();

	Time::Get()->Start();

	program = new Program();

	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			Gui::Get()->Update();

			Time::Get()->Update();
			if (ImGui::GetIO().WantCaptureMouse == false)
			{
				Keyboard::Get()->Update();
				Mouse::Get()->Update();
			}

			program->Update();

			program->PreRender();
			Graphics::Get()->Begin();
			{
				program->Render();
				program->GUI();
				Gui::Get()->Render();
			}
			Graphics::Get()->End();
		}
	}

	SAFE_DELETE(program);

	Textures::Delete();
	Gui::Delete();
	Time::Delete();
	Keyboard::Delete();
	Mouse::Delete();
	Graphics::Delete();

	return msg.wParam;
}

HWND handle;

LRESULT Window::WndProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
	Mouse::Get()->InputProc(message, wParam, lParam);
	Gui::Get()->MsgProc(handle, message, wParam, lParam);

	if (message == WM_CREATE)
		::handle = handle;

	if (message == WM_CLOSE || message == WM_DESTROY)
	{
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(handle, message, wParam, lParam);
}
