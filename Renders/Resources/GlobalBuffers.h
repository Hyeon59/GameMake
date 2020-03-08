#pragma once

#include "stdafx.h"

class WorldBuffer : public ShaderBuffer
{
public:
	WorldBuffer() : ShaderBuffer(&data, sizeof(Struct))
	{
		D3DXMatrixIdentity(&data.world);
	}

	void SetWorld(D3DXMATRIX world)
	{
		D3DXMatrixTranspose(&data.world, &world);
	}

	struct Struct
	{
		D3DXMATRIX world;
	};

private:
	Struct data;
};

class VPBuffer : public ShaderBuffer
{
public:
	VPBuffer() : ShaderBuffer(&data, sizeof(Struct))
	{
		D3DXMatrixIdentity(&data.view);
		D3DXMatrixIdentity(&data.proj);
	}

	void SetView(D3DXMATRIX view)
	{
		D3DXMatrixTranspose(&data.view, &view);
	}

	void SetProj(D3DXMATRIX proj)
	{
		D3DXMatrixTranspose(&data.proj, &proj);
	}

	struct Struct
	{
		D3DXMATRIX view;
		D3DXMATRIX proj;
	};

private:
	Struct data;
};