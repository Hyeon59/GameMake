#include "stdafx.h"
#include "IShader.h"

void IShader::CompileShader(wstring path, string entryName, string profile, ID3D10Blob ** blob)
{
	ID3D10Blob* error = nullptr;
	HRESULT hr = D3DCompileFromFile
	(
		path.c_str(),
		nullptr,
		nullptr,
		entryName.c_str(),
		profile.c_str(),
		D3DCOMPILE_ENABLE_STRICTNESS,
		0,
		blob,
		&error
	);

	CheckShaderError(hr, error);
	SAFE_RELEASE(error);
}

void IShader::CheckShaderError(HRESULT hr, ID3D10Blob * error)
{
	if (FAILED(hr))
	{
		if (error)
		{
			string str = (const char*)error->GetBufferPointer();
			MessageBoxA(NULL, str.c_str(), "Shader Error", MB_OK);
		}
		ASSERT(false);
	}
}
