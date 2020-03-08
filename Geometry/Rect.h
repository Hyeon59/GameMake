#pragma once



class Rect
{
public:
	Rect();
	Rect(D3DXVECTOR3 position, D3DXVECTOR3 size, float rotation);
	~Rect();

	void Initialize(
		vector<VertexColor>& vertices, 
		vector<UINT>& indices, 
		D3DXVECTOR3 position, 
		D3DXVECTOR3 size, 
		float rotation, 
		wstring shaderPath
	);

	void TransformVertices();

	void Update();
	void Render();

	void Move(D3DXVECTOR3 position);

	bool Intersect(Rect* other);
	RectEdges* GetEdges() { return &edge; }

	void CheckIntersect(bool bIntersect) { this->bIntersect = bIntersect; }
	void Reposition(D3DXVECTOR3 newPos);
	void Resize(D3DXVECTOR3 size);
	bool CheckEnd();

	vector<VertexColor>& GetVertices() { return vertices; }
	vector<UINT>& GetIndices() { return indices; }
	D3DXVECTOR3 GetPosition() { return position; }
	D3DXVECTOR3 GetSize() { return size;     }
	float GetRotation()   { return rotation; }
	wstring GetShaderPath() { return shaderPath; }

	void GUI();
	void ChangeColor();
	void ChangeColor(D3DXCOLOR color);

	void Jump();
	bool IsJumping() { return bJump; }

private: //데이터 정렬
	vector<VertexColor> vertices;
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

	RectEdges edge;
	bool bIntersect = false;

	D3DXCOLOR color;
	wstring shaderPath = L"./_Shaders/Color.hlsl";

	float vy = 0.0f;
	bool bJump = false;
	float jumpAccel = -600.0f;
	float jumpSpeed = 600.0f;
	float groundY = 0;

	UINT index;
	static UINT objectIndex;
};