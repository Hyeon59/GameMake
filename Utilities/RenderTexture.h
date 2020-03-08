#pragma once

class RenderTexture
{
public:
	RenderTexture();
	~RenderTexture();

	void Initialize();
	void RenderToTexture();
	ID3D11ShaderResourceView* GetSRV() { return srv; }

private:
	ID3D11Texture2D* rtt = nullptr; //render target texture
	ID3D11RenderTargetView* rtv = nullptr; //render target view
	ID3D11ShaderResourceView* srv = nullptr;

};