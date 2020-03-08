#pragma once

class PixelShader : public IShader
{
public:
	PixelShader();
	~PixelShader();

	void Create(wstring path, string entryName) override;
	void Clear() override;
	void SetShader() override;

	ID3D11PixelShader* GetResource() { return shader; }
	ID3D10Blob* GetBlob() { return blob; }


private:
	ID3D11PixelShader* shader = nullptr;
	ID3D10Blob* blob = nullptr;
};