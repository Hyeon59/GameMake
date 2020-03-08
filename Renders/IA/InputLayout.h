#pragma once

class InputLayout
{
public:
	InputLayout() = default;
	~InputLayout();

	void Create(D3D11_INPUT_ELEMENT_DESC* desc, const UINT& count, ID3DBlob* blob);
	void SetIA();

	ID3D11InputLayout* GetResource() { return inputLayout; }

private:
	ID3D11InputLayout* inputLayout = nullptr;


};