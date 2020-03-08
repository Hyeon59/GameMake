#include "stdafx.h"
#include "States.h"

D3D11_RASTERIZER_DESC States::rsDesc;
D3D11_SAMPLER_DESC States::ssDesc;
D3D11_BLEND_DESC States::bsDesc;
D3D11_DEPTH_STENCIL_DESC States::dsDesc;

void States::Create()
{
	CreateRasterizerDesc();
	CreateSamplerDesc();
	CreateBlendDesc();
	CreateDepthStencilDesc();
}

void States::Delete()
{
}

void States::GetRasterizerDesc(D3D11_RASTERIZER_DESC * desc)
{
	memcpy(desc, &rsDesc, sizeof(D3D11_RASTERIZER_DESC));
}

void States::GetSamplerDesc(D3D11_SAMPLER_DESC * desc)
{
	memcpy(desc, &ssDesc, sizeof(D3D11_SAMPLER_DESC));
}

void States::GetBlendDesc(D3D11_BLEND_DESC * desc)
{
	memcpy(desc, &bsDesc, sizeof(D3D11_BLEND_DESC));
}

void States::GetDepthStencilDesc(D3D11_DEPTH_STENCIL_DESC * desc)
{
	memcpy(desc, &dsDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
}

void States::CreateRasterizer(D3D11_RASTERIZER_DESC * desc, ID3D11RasterizerState ** state)
{
	HRESULT hr;
	hr = DEVICE->CreateRasterizerState(desc, state);
	ASSERT(hr);
}

void States::CreateSampler(D3D11_SAMPLER_DESC * desc, ID3D11SamplerState ** state)
{
	HRESULT hr;
	hr = DEVICE->CreateSamplerState(desc, state);
	ASSERT(hr);
}

void States::CreateBlend(D3D11_BLEND_DESC * desc, ID3D11BlendState ** state)
{
	HRESULT hr;
	hr = DEVICE->CreateBlendState (desc, state);
	ASSERT(hr);
}

void States::CreateDepthStencil(D3D11_DEPTH_STENCIL_DESC * desc, ID3D11DepthStencilState ** state)
{
	HRESULT hr;
	hr = DEVICE->CreateDepthStencilState(desc, state);
	ASSERT(hr);
}

void States::CreateRasterizerDesc()
{
	ZeroMemory(&rsDesc, sizeof(D3D11_RASTERIZER_DESC));

	rsDesc.AntialiasedLineEnable = false;
	rsDesc.CullMode = D3D11_CULL_BACK;
	rsDesc.DepthBias = 0;
	rsDesc.DepthBiasClamp = 0.0f;
	rsDesc.DepthClipEnable = true;
	rsDesc.FillMode = D3D11_FILL_SOLID;
	rsDesc.FrontCounterClockwise = false;
	rsDesc.MultisampleEnable = false;
	rsDesc.ScissorEnable = false;
	rsDesc.SlopeScaledDepthBias = false;
}

void States::CreateSamplerDesc()
{
	ZeroMemory(&ssDesc, sizeof(D3D11_SAMPLER_DESC));
	ssDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	ssDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	ssDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	ssDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	ssDesc.MaxAnisotropy = 1;
	ssDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	ssDesc.MinLOD = numeric_limits<float>::min();
	ssDesc.MaxLOD = numeric_limits<float>::max();
}

void States::CreateBlendDesc()
{
	ZeroMemory(&bsDesc, sizeof(D3D11_BLEND_DESC));
	
	bsDesc.AlphaToCoverageEnable = false;
	bsDesc.IndependentBlendEnable = false;

	bsDesc.RenderTarget[0].BlendEnable = false;
	bsDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA; //깔려있는 색상
	bsDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;	//이제 바를 색상
	bsDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

	bsDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	bsDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
	bsDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

	bsDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
}

void States::CreateDepthStencilDesc()
{
	ZeroMemory(&dsDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));

	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;

	dsDesc.StencilEnable = true;
	dsDesc.StencilReadMask = 0xFF;
	dsDesc.StencilWriteMask = 0xFF;

	dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
}
