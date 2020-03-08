#pragma once

class VertexShader : public IShader
{
public:
	VertexShader();
	~VertexShader();

	void Create(wstring path, string entryName) override;
	void Clear() override;
	void SetShader() override;

	ID3D11VertexShader* GetResource() { return shader; }
	ID3D10Blob* GetBlob() { return blob; }

private:
	ID3D11VertexShader* shader = nullptr;
	ID3DBlob* blob = nullptr;
};