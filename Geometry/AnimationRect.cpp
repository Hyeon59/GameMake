#include "stdafx.h"
#include "AnimationRect.h"

#include "Animation/Animator.h"

AnimationRect::AnimationRect(D3DXVECTOR3 position, D3DXVECTOR3 size, float rotation)
	: position(position), size(size), rotation(rotation)
{
	vertices.assign(4, VertexTexture());

	vertices[0].position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertices[0].uv = D3DXVECTOR2(0, 1);

	vertices[1].position = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertices[1].uv = D3DXVECTOR2(0, 0);

	vertices[2].position = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	vertices[2].uv = D3DXVECTOR2(1, 1);

	vertices[3].position = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
	vertices[3].uv = D3DXVECTOR2(1, 0);

	VB = new VertexBuffer();
	VB->Create(vertices, D3D11_USAGE_DYNAMIC);

	indices = { 0,1,2,2,1,3 };

	IB = new IndexBuffer();
	IB->Create(indices, D3D11_USAGE_IMMUTABLE);

	VS = new VertexShader();
	VS->Create(L"./_Shaders/Animation.hlsl", "VS");

	PS = new PixelShader();
	PS->Create(L"./_Shaders/Animation.hlsl", "PS");

	IL = new InputLayout();
	IL->Create(VertexTexture::descs, VertexTexture::count, VS->GetBlob());

	WB = new WorldBuffer();

	D3DXMatrixTranslation(&T, position.x, position.y, position.z);
	D3DXMatrixScaling(&S, size.x, size.y, size.z);
	D3DXMatrixRotationZ(&R, (float)D3DXToRadian(rotation));

	world = S * R * T;

	WB->SetWorld(world);

	rockman = new Texture2D(L"./_Textures/testMonster.png");
	RunR = new AnimationClip(L"RunR", rockman, 10, { 0,0 }, { rockman->GetWidth(), rockman->GetHeight() / 2.0f });
	RunL = new AnimationClip(L"RunL", rockman, 10, { 0, rockman->GetHeight() / 2.0f }, { rockman->GetWidth(), rockman->GetHeight() }, true);
	animator = new Animator(RunR);
	animator->AddAnimClip(RunL);

	{
		D3D11_SAMPLER_DESC desc;
		States::GetSamplerDesc(&desc);
		States::CreateSampler(&desc, &sampler_Disabled);

		desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		States::CreateSampler(&desc, &sampler_Enabled);
	}

	TransformVertices();
}

AnimationRect::~AnimationRect()
{
	SAFE_RELEASE(sampler_Enabled);
	SAFE_RELEASE(sampler_Disabled);

	SAFE_DELETE(animator);
	SAFE_DELETE(RunL);
	SAFE_DELETE(RunR);
	SAFE_DELETE(rockman);

	SAFE_DELETE(WB);

	SAFE_DELETE(IL);

	SAFE_DELETE(PS);
	SAFE_DELETE(VS);

	SAFE_DELETE(IB);
	SAFE_DELETE(VB);
}

void AnimationRect::Update()
{
	animator->Update();

	{
		D3D11_MAPPED_SUBRESOURCE subResource;
		DC->Map(VB->GetResource(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);

		vertices[0].uv = D3DXVECTOR2(animator->GetCurrentFrame().x, animator->GetCurrentFrame().y + animator->GetTexelFrameSize().y);
		vertices[1].uv = animator->GetCurrentFrame();
		vertices[2].uv = animator->GetCurrentFrame() + animator->GetTexelFrameSize();
		vertices[3].uv = D3DXVECTOR2(animator->GetCurrentFrame().x + animator->GetTexelFrameSize().x, animator->GetCurrentFrame().y);
	
		memcpy(subResource.pData, vertices.data(), sizeof(VertexTexture) * vertices.size());

		DC->Unmap(VB->GetResource(), 0);
	}
}

void AnimationRect::Render()
{
	VB->SetIA();
	IB->SetIA();

	IL->SetIA();
	DC->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	VS->SetShader();
	WB->SetVSBuffer(0);

	PS->SetShader();
	if (animator->GetCurrentSRV())
	{
		srv = animator->GetCurrentSRV();
		DC->PSSetShaderResources(4, 1, &srv);
	}

	DC->PSSetSamplers(0, 1, &sampler_Enabled);
	DC->DrawIndexed(IB->GetCount(), 0, 0);
	DC->PSSetSamplers(0, 1, &sampler_Disabled);
}

void AnimationRect::Move(D3DXVECTOR3 position)
{
	this->position = position;
	D3DXMatrixTranslation(&T, this->position.x, this->position.y, this->position.z);
	world = S * R * T;

	WB->SetWorld(world);

	TransformVertices();
}

void AnimationRect::AnimationController(D3DXVECTOR2 dir)
{
	if (dir.x < 0)
		animator->SetCurrentAnimClip(L"RunL");
	else if (dir.x > 0)
		animator->SetCurrentAnimClip(L"RunR");
}

void AnimationRect::TransformVertices()
{
	D3DXVec3TransformCoord(&r.LT, &vertices[1].position, &world);
	D3DXVec3TransformCoord(&r.RB, &vertices[2].position, &world);
}
