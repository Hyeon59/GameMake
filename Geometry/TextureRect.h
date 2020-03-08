#pragma once

/* ÀÌ°Å °Á RectEdges·Î ¹Ù²ã³õÀ½
struct TextureEdges
{
	D3DXVECTOR3 LT;
	D3DXVECTOR3 RB;
}
*/;

class TextureRect
{
public:
	TextureRect(D3DXVECTOR3 position, D3DXVECTOR3 size, float rotation = 0);
	~TextureRect();

	void TransformVertices();

	void Update();
	void Render();
	bool CheckEnd();
	void Reposition(D3DXVECTOR3 newPos);

	//TextureEdges* GetEdges() { return &edge; }
	RectEdges* GetEdges() { return &edge; }
	D3DXVECTOR3 GetPosition() { return position; }
	D3DXVECTOR3 GetSize() { return size; }

	void Move(D3DXVECTOR3 position);
	bool Intersect(TextureRect* other);
	void CheckIntersect(bool bIntersect) { this->bIntersect = bIntersect; }

	void SetSRV(wstring path);
	void SetSRV(ID3D11ShaderResourceView* srv) { this->srv = srv; }

private:
	vector<VertexTexture> vertices;
	VertexBuffer* VB = nullptr;

	vector<UINT> indices;
	IndexBuffer* IB = nullptr;

	InputLayout* IL = nullptr;

	VertexShader* VS = nullptr;
	PixelShader* PS = nullptr;

	D3DXMATRIX world;

	D3DXMATRIX S, R, T;

	WorldBuffer* WB = nullptr;

	D3DXVECTOR3 position = Value::Zero;
	D3DXVECTOR3 size = Value::One;
	float rotation = 0.0f;

	bool bIntersect = false;
	//TextureEdges edge;
	RectEdges edge;

	ID3D11ShaderResourceView* srv = nullptr;
	ID3D11ShaderResourceView* nullView = nullptr;

	ID3D11BlendState* bsState_disabled;
	ID3D11BlendState* bsState_enabled;
};