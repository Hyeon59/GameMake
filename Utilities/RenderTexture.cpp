#include "stdafx.h"
#include "RenderTexture.h"

RenderTexture::RenderTexture()
{
}

RenderTexture::~RenderTexture()
{
	SAFE_RELEASE(rtv);
	SAFE_RELEASE(srv);
	SAFE_RELEASE(rtt);
}

void RenderTexture::Initialize()
{
	D3D11_TEXTURE2D_DESC textureDesc;
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	HRESULT hr;

	ZeroMemory(&textureDesc, sizeof(D3D11_TEXTURE2D_DESC));
	ZeroMemory(&rtvDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
	ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));

	textureDesc.Width = WinMaxWidth;
	textureDesc.Height = WinMaxHeight;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	hr = DEVICE->CreateTexture2D(&textureDesc, NULL, &rtt);
	ASSERT(hr);

	rtvDesc.Format = textureDesc.Format;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Texture2D.MipSlice = 0;

	hr = DEVICE->CreateRenderTargetView(rtt, &rtvDesc, &rtv);
	ASSERT(hr);

	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	hr = DEVICE->CreateShaderResourceView(rtt, &srvDesc, &srv);
	ASSERT(hr);

}

void RenderTexture::RenderToTexture()
{
	DC->OMSetRenderTargets(1, &rtv, nullptr);
	DC->ClearRenderTargetView(rtv, Graphics::Get()->GetClearColor());
}
