#pragma once

struct VertexColor
{
	VertexColor() : position(0, 0, 0), color(0, 0, 0, 0) {}
	VertexColor(const D3DXVECTOR3& position, const D3DXCOLOR& color)
		: position(position)
		, color(color)
	{}

	D3DXVECTOR3 position;
	D3DXCOLOR color;
	static D3D11_INPUT_ELEMENT_DESC descs[];
	static const UINT count = 2;
};

struct VertexTexture
{
	VertexTexture() : position(0, 0, 0), uv(0, 0) {};
	VertexTexture(const D3DXVECTOR3& position, const D3DXVECTOR2& uv)
		: position(position), uv(uv)
	{}

	D3DXVECTOR3 position;
	D3DXVECTOR2 uv;
	static D3D11_INPUT_ELEMENT_DESC descs[];
	static const UINT count = 2;

};

struct VertexTextureColor
{
	VertexTextureColor() : position(0, 0, 0), uv(0, 0), color(0,0,0,1) {};
	VertexTextureColor(const D3DXVECTOR3& position, const D3DXVECTOR2& uv, const D3DXCOLOR& color)
		: position(position), uv(uv), color(color)
	{}

	D3DXVECTOR3 position;
	D3DXVECTOR2 uv;
	D3DXCOLOR color;
	static D3D11_INPUT_ELEMENT_DESC descs[];
	static const UINT count = 3;
};

struct VertexTile
{
	VertexTile() : position(0, 0, 0), uv(0, 0), color(0, 0, 0, 1), tileUV(0,0), index(-1), redcell(0,0,1,0) {};
	VertexTile(const D3DXVECTOR3& position, const D3DXVECTOR2& uv, const D3DXCOLOR& color, const D3DXVECTOR2& tileUV, const UINT& tileIndex, const D3DXCOLOR& redcell)
		: position(position), uv(uv), color(color), tileUV(tileUV), index(tileIndex), redcell(redcell)
	{}

	D3DXVECTOR3 position;
	D3DXVECTOR2 uv;
	D3DXVECTOR2 tileUV;
	D3DXCOLOR color;
	D3DXCOLOR redcell;
	UINT index;
	static D3D11_INPUT_ELEMENT_DESC descs[];
	static const UINT count = 6;
};