#include "stdafx.h"
#include "Texture2D.h"

using namespace DirectX;

vector<TextureDesc> Textures::descs;

Texture2D::Texture2D(wstring filePath, D3DX11_IMAGE_LOAD_INFO * loadInfo)
	: filePath(filePath)
{
	Textures::Load(this, loadInfo);
}

Texture2D::~Texture2D()
{
}

D3D11_TEXTURE2D_DESC Texture2D::ReadPixel(DXGI_FORMAT readFormat, vector<D3DXCOLOR>& pixels)
{
	ID3D11Texture2D* srcTexture;
	srv->GetResource((ID3D11Resource**)&srcTexture);
	return ReadPixel(srcTexture, readFormat, pixels);
}

D3D11_TEXTURE2D_DESC Texture2D::ReadPixel(ID3D11Texture2D * src, DXGI_FORMAT readFormat, vector<D3DXCOLOR>& pixels)
{
	D3D11_TEXTURE2D_DESC srcDesc;
	src->GetDesc(&srcDesc);

	D3D11_TEXTURE2D_DESC destDesc;
	ZeroMemory(&destDesc, sizeof(D3D11_TEXTURE2D_DESC));
	destDesc.Width = srcDesc.Width;
	destDesc.Height = srcDesc.Height;
	destDesc.MipLevels = 1;
	destDesc.ArraySize = 1;
	destDesc.Format = readFormat;
	destDesc.SampleDesc = srcDesc.SampleDesc;
	destDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	destDesc.Usage = D3D11_USAGE_STAGING;

	ID3D11Texture2D* destTexture;
	HRESULT hr = DEVICE->CreateTexture2D(&destDesc, NULL, &destTexture);
	ASSERT(hr);

	hr = D3DX11LoadTextureFromTexture(DC, src, NULL, destTexture);
	ASSERT(hr);

	UINT* colors = new UINT[destDesc.Width * destDesc.Height];
	D3D11_MAPPED_SUBRESOURCE subResource;
	DC->Map(destTexture, 0, D3D11_MAP_READ, NULL, &subResource);
	{
		memcpy(colors, subResource.pData, sizeof(UINT) * destDesc.Width * destDesc.Height);
	}
	DC->Unmap(destTexture, 0);

	for (UINT y = 0; y < destDesc.Height; y++)
	{
		for (UINT x = 0; x < destDesc.Width; x++)
		{
			UINT index = destDesc.Width * y + x;

			float f = 1.0f / 255.0f;

			//png
			//bgra
			//float b = f * (float)((0x000000FF & colors[index]) >> 0); // ">>" result / 2^0 
			//float g = f * (float)((0x0000FF00 & colors[index]) >> 8); // result / 2^8
			//float r = f * (float)((0x00FF0000 & colors[index]) >> 16);
			//float a = f * (float)((0xFF000000 & colors[index]) >> 24);

			//pixels.push_back(D3DXCOLOR(r, g, b, a));
			pixels.push_back(colors[index]);
		}
	}

	SAFE_DELETE_ARRAY(colors);
	SAFE_RELEASE(destTexture);

	return destDesc;
}


void Textures::Create()
{
}

void Textures::Delete()
{
	for (TextureDesc desc : descs)
		SAFE_RELEASE(desc.srv);
}

void Textures::Load(Texture2D * texture, D3DX11_IMAGE_LOAD_INFO * loadInfo)
{
	HRESULT hr;

	TexMetadata metaData;
	wstring ext = Path::GetExtension(texture->filePath);
	if (ext == L"tga")
	{
		hr = GetMetadataFromTGAFile(texture->filePath.c_str(), metaData);
		ASSERT(hr);
	}
	else if (ext == L"dds")
	{
		hr = GetMetadataFromDDSFile(texture->filePath.c_str(), DDS_FLAGS_NONE, metaData);
		ASSERT(hr);
	}
	else if (ext == L"hdr")
	{
		ASSERT(false);
	}
	else
	{
		hr = GetMetadataFromWICFile(texture->filePath.c_str(), WIC_FLAGS_NONE, metaData);
		ASSERT(hr);
		//bmp, png, tiff, jpeg(jpg), gif
	}

	UINT width = metaData.width;
	UINT height = metaData.height;

	if (loadInfo != NULL)
	{
		width = loadInfo->Width;
		height = loadInfo->Height;

		metaData.width = loadInfo->Width;
		metaData.height = loadInfo->Height;

		
	}

	TextureDesc desc;
	desc.file = texture->filePath;
	desc.width = width;
	desc.height = height;

	TextureDesc exist;
	bool bExist = false;
	for (TextureDesc temp : descs)
	{
		if (desc == temp)
		{
			bExist = true;
			exist = temp;

			break;
		}
	}

	if (bExist == true)
	{
		texture->metaData = exist.metaData;
		texture->srv = exist.srv;
	}
	else
	{
		ScratchImage image;
		//이미지를 불러올때 메모리 관리를 도와주는 헬퍼 클래스

		if (ext == L"tga")
		{
			hr = LoadFromTGAFile(texture->filePath.c_str(), &metaData, image);
			ASSERT(hr);
		}
		else if (ext == L"dds")
		{
			hr = LoadFromDDSFile(texture->filePath.c_str(), DDS_FLAGS_NONE, &metaData, image);
			ASSERT(hr);
		}
		else if (ext == L"hdr")
		{
			ASSERT(false);
		}
		else
		{
			hr = LoadFromWICFile(texture->filePath.c_str(), WIC_FLAGS_NONE, &metaData, image);
			ASSERT(hr);
		}

		ID3D11ShaderResourceView* srv;
		hr = CreateShaderResourceView(DEVICE, image.GetImages(), image.GetImageCount(), metaData, &srv);
		ASSERT(hr);

		desc.file = texture->filePath;
		desc.width = metaData.width;
		desc.height = metaData.height;
		desc.srv = srv;
		desc.metaData = metaData;

		texture->srv = srv;
		texture->metaData = metaData;

		descs.push_back(desc);
	}
}

