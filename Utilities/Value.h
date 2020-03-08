#pragma once

struct RectEdges
{
	D3DXVECTOR3 LT;
	D3DXVECTOR3 RB;
};

class Value
{
public:
	static D3DXVECTOR2 Zero2;
	static D3DXVECTOR3 Zero;
	static D3DXVECTOR3 One;

	static D3DXVECTOR3 Up;
	static D3DXVECTOR3 Right;
	static D3DXVECTOR3 Left;
	static D3DXVECTOR3 Down;
	static D3DXVECTOR3 Front;

	static D3DXCOLOR Red;
	static D3DXCOLOR Green;
	static D3DXCOLOR Blue;
	static D3DXCOLOR Yellow;
	static D3DXCOLOR Black;
	static D3DXCOLOR White;

	static D3DXMATRIX Identity;
};