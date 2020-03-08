#include "stdafx.h"
#include "Window.h"
#include "Program.h"

WinDesc Window::desc;
Program* Window::program = nullptr;

HINSTANCE instance;
Window::Window(WinDesc desc)
{
	WNDCLASSEX wndClass;

	wndClass.cbClsExtra = 0; //예약영역 사용안함
	wndClass.cbWndExtra = 0; //예약영역 사용안함
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); //윈도우 바탕색
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW); //윈도우 마우스 커서
	wndClass.hIcon = LoadIcon(NULL, IDI_WINLOGO); //윈도우 아이콘
	wndClass.hIconSm = wndClass.hIcon; //윈도우 아이콘 작은거
	wndClass.hInstance = desc.instance; // 윈도우 인스턴스 핸들
	wndClass.lpfnWndProc = (WNDPROC)WndProc; // 윈도우 메세지 처리함수 지정
	wndClass.lpszClassName = desc.AppName.c_str(); //윈도우 클래스이름
	wndClass.lpszMenuName = NULL; //윈도우 메뉴 이름
	wndClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC; //윈도우 스타일
	wndClass.cbSize = sizeof(WNDCLASSEX); //버전확인용

	WORD wHr = RegisterClassEx(&wndClass);
	
	desc.handle = CreateWindowEx
	(
		WS_EX_APPWINDOW //확장 스타일 옵션
		, desc.AppName.c_str() //윈도우 클래스 이름
		, desc.AppName.c_str() //윈도우 타이틀바 이름
		, WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_OVERLAPPEDWINDOW //윈도우 스타일
		, CW_USEDEFAULT			//윈도우를 생성할 x좌표
		, CW_USEDEFAULT			//윈도우를 생성할 y좌표
		, CW_USEDEFAULT			//윈도우 넓이
		, CW_USEDEFAULT			//윈도우 높이
		, NULL					//윈도우 생성시 이 윈도우를 가질 부모의 핸들
		, (HMENU)NULL			//메뉴 혹은 자식 윈도우의 식별자 ID
		, desc.instance			//윈도우를 생성할 인스턴스 핸들
		, NULL					//MDI 클라이언트 윈도우용. 사용 x
	);

	RECT rect = { 0,0,(LONG)desc.width, (LONG)desc.height };

	UINT centerX = (GetSystemMetrics(SM_CXSCREEN) - (UINT)desc.width ) / 2;
	UINT centerY = (GetSystemMetrics(SM_CYSCREEN) - (UINT)desc.height) / 2;

	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);
	
	//윈도우의 크기와 위치를 동시에 변경하는 함수
	MoveWindow
	(
		desc.handle //이동대상 윈도우 핸들
		, centerX, centerY //화면 중심의 좌표
		, rect.right - rect.left, rect.bottom - rect.top//윈도우 크기(단위:픽셀)
		, TRUE
	);

	ShowWindow(desc.handle, SW_SHOWNORMAL); //윈도우의 보이는 상태 설정
	SetForegroundWindow(desc.handle); //제일 전면에 나올 윈도우 설정
	SetFocus(desc.handle); //윈도우 포커스 설정

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
