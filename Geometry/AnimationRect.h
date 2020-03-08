#pragma once

//AnimtionRect - animator�� ���⿡ ����ְ�, ������ �����̰ų� ũ�� ���������͵�
//AnimationClip - ���ϸ��̼� ����
//Animator - ���ϸ��̼� ���

class AnimationRect
{
public:
	AnimationRect(D3DXVECTOR3 position, D3DXVECTOR3 size, float rotation = 0.0f);
	~AnimationRect();

	void Update();
	void Render();

	D3DXVECTOR3 GetPosition() { return position; }
	D3DXVECTOR3 GetCenterPos() { return D3DXVECTOR3(position.x + size.x / 2, position.y + size.y / 2, 0); }

	void Move(D3DXVECTOR3 position);
	void AnimationController(D3DXVECTOR2 dir);

	void TransformVertices();
	RectEdges* GetRectEdges() { return &r; }

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

	class Animator* animator = nullptr;
	class AnimationClip* RunR = nullptr;
	class AnimationClip* RunL = nullptr;
	Texture2D* rockman = nullptr;

	ID3D11ShaderResourceView* srv = nullptr;

	ID3D11SamplerState* sampler_Enabled = nullptr;
	ID3D11SamplerState* sampler_Disabled = nullptr;

	RectEdges r;
};