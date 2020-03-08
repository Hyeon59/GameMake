#pragma once

class Texture2D
{
public:
	friend class Textures;

	Texture2D(wstring filePath, D3DX11_IMAGE_LOAD_INFO* loadInfo = nullptr);
	~Texture2D();

	ID3D11ShaderResourceView* GetSRV() { return srv; }

	D3D11_TEXTURE2D_DESC ReadPixel(DXGI_FORMAT readFormat, vector<D3DXCOLOR>& pixels);
	static D3D11_TEXTURE2D_DESC ReadPixel
	(
		ID3D11Texture2D* src, 
		DXGI_FORMAT readFormat, 
		vector<D3DXCOLOR>& pixels
	);

	DXGI_FORMAT GetImageFormat() { return metaData.format; }
	float GetWidth() { return (float)metaData.width; }
	float GetHeight() { return (float)metaData.height; }

private:
	wstring filePath;
	DirectX::TexMetadata metaData;
	ID3D11ShaderResourceView* srv;
};

struct TextureDesc
{
	wstring file;
	UINT width, height;
	DirectX::TexMetadata metaData;
	ID3D11ShaderResourceView* srv;

	bool operator ==(const TextureDesc& desc)
	{
		bool b = true;
		b &= file == desc.file;
		b &= width == desc.width;
		b &= height == desc.height;

		return b;
	}

};

class Textures
{
public:
	friend class Texture2D;

public:
	static void Create();
	static void Delete();

private:
	static void Load(Texture2D* texture, D3DX11_IMAGE_LOAD_INFO* loadInfo = nullptr);

private:
	static vector<TextureDesc> descs;
};