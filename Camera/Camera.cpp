#include "stdafx.h"
#include "Camera.h"

Camera* Camera::instance = nullptr;

//void Camera::Create()
//{
//	if (instance == nullptr)
//		instance = new Camera();
//}
//
//void Camera::Delete()
//{
//	SAFE_DELETE(instance);
//}
//
//Camera * Camera::Get()
//{
//	return instance;
//}

Camera::Camera()
{
	D3DXMatrixIdentity(&view);
	D3DXMatrixIdentity(&proj);

	VP = new VPBuffer();
	UpdateView();
	UpdateProj();

	VP->SetView(view);
	VP->SetProj(proj);

	mapBoundary.LT = Value::Zero;
	mapBoundary.RB = Value::Zero;
}

Camera::~Camera()
{
	SAFE_DELETE(VP);
}



void Camera::Update()
{
	if (Keyboard::Get()->Press(VK_LSHIFT) && KeyPress_Up)
		position.y += 100 * Time::Delta();
	else if (Keyboard::Get()->Press(VK_LSHIFT) && KeyPress_Down)
		position.y -= 100 * Time::Delta();

	if (Keyboard::Get()->Press(VK_LSHIFT) && KeyPress_Left)
		position.x -= 100 * Time::Delta();
	else if (Keyboard::Get()->Press(VK_LSHIFT) && KeyPress_Right)
		position.x += 100 * Time::Delta();

	if (Mouse::Get()->GetMoveValue().z > 0)
		ZoomIn();
	else if (Mouse::Get()->GetMoveValue().z < 0)
		ZoomOut();

	UpdateView();
}

void Camera::Render()
{
	VP->SetVSBuffer(1);
}

void Camera::SetPosition(D3DXVECTOR3 position, bool bOffCenter)
{
	if (mapBoundary.LT != mapBoundary.RB)
	{
		if (bOffCenter)
		{
			if (!(position.x - WinMaxWidth / 2 < mapBoundary.LT.x
				|| position.x + WinMaxWidth / 2 > mapBoundary.RB.x))
			{
				tempX = position.x - WinMaxWidth / 2;
			}
			else if (position.x > mapBoundary.LT.x && position.x < mapBoundary.LT.x + WinMaxWidth / 2)//주원
				tempX = mapBoundary.LT.x;//주원
			else if (position.x < mapBoundary.RB.x && position.x > mapBoundary.RB.x - WinMaxWidth / 2)//주원
				tempX = mapBoundary.RB.x - WinMaxWidth;//주원

			if (!(position.y - WinMaxHeight / 2 < mapBoundary.RB.y
				|| position.y + WinMaxHeight / 2 > mapBoundary.LT.y))
			{
				tempY = position.y - WinMaxHeight / 2;
			}
			this->position = { tempX, tempY, 0 };
		}
		else
			this->position = position;
	}

	

	UpdateView();
}

void Camera::UnProject(D3DXVECTOR3 * out, D3DXVECTOR3 source, D3DXMATRIX world)
{
	D3DXVECTOR3 position = source;
	/*
		local * world * view * projection * viewport = 화면공간
		NDC to viewport
		x = (NDC.x + 1) * viewport.width * 0.5f + viewport.TopLeftX;
		y = (1 - NDC.y) * viewport.height * 0.5f + viewport.TopLeftY;
	*/

	out->x = (position.x * 2.0f / WinMaxWidth) - 1.0f;
	out->y = ((position.y * 2.0f / WinMaxHeight) - 1.0f);
	out->z = position.z;

	D3DXMATRIX wvp = world * view * proj;
	D3DXMatrixInverse(&wvp, NULL, &wvp);
	D3DXVec3TransformCoord(out, out, &wvp);
}

void Camera::ZoomIn()
{
	screenSize.x += zoomSpeed * ratio;
	screenSize.y -= zoomSpeed;
	screenSize.z -= zoomSpeed * ratio;
	screenSize.w += zoomSpeed;

	UpdateProj();
}

void Camera::ZoomOut()
{
	screenSize.x -= zoomSpeed * ratio;
	screenSize.y += zoomSpeed;
	screenSize.z += zoomSpeed * ratio;
	screenSize.w -= zoomSpeed;

	UpdateProj();
}

void Camera::UpdateView()
{
	D3DXMatrixLookAtLH(&view, &position, &(position + Value::Front), &Value::Up);
	VP->SetView(view);
}

void Camera::UpdateProj()
{
	D3DXMatrixOrthoOffCenterLH(&proj, screenSize.x, screenSize.z, screenSize.w, screenSize.y, 0, 1);
	VP->SetProj(proj);
}
