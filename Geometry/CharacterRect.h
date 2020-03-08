#pragma once


class CharacterRect
{
public:
	CharacterRect(D3DXVECTOR3 position, D3DXVECTOR3 size, float rotation = 0.0f);
	~CharacterRect();

	void Update();
	void Render();
	void TransformVertices();

	void Move(D3DXVECTOR3 position);
	void Reposition(D3DXVECTOR3 position);
	void Resize(D3DXVECTOR3 size);

	// �ִϰ���
	void SetAnimation(wstring path); // �� ó�� Json �ҷ�����
	void AddAnimation(wstring path); // �߰� Json �ҷ�����
	void ChangeClip(wstring clipname); // ����� Clip ����
	void SetPlayRate(float playlate);
	void SetLoop(bool bLoop);
	void SetStop(bool bStop);
	bool GetLoop();
	bool GetStop();


	D3DXVECTOR3 GetPosition() { return position; }
	D3DXVECTOR3 GetCenterPos() { return D3DXVECTOR3(position.x + size.x / 2, position.y + size.y / 2, 0); }
	D3DXVECTOR3 GetSize() { return size; }
	RectEdges* GetEdges() { return &edge; }

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

	ID3D11ShaderResourceView* srv = nullptr;

	ID3D11SamplerState* sampler_Enabled = nullptr;
	ID3D11SamplerState* sampler_Disabled = nullptr;

private:
	D3DXVECTOR3 position = Value::Zero;
	D3DXVECTOR3 size = Value::One;
	float rotation = 0.0f;

	RectEdges edge;

	// �ִ�
	class Animator* animator = nullptr;
	class AnimationClip* animClip = nullptr;
};