#include "stdafx.h"
#include "InputLayout.h"

InputLayout::~InputLayout()
{
	SAFE_RELEASE(inputLayout);
}

void InputLayout::Create(D3D11_INPUT_ELEMENT_DESC * desc, const UINT & count, ID3DBlob * blob)
{
	if (!desc || !count || !blob)
		ASSERT(false);

	HRESULT hr = DEVICE->CreateInputLayout
	(
		desc,
		count,
		blob->GetBufferPointer(),
		blob->GetBufferSize(),
		&inputLayout
	);

	ASSERT(hr);
}

void InputLayout::SetIA()
{
	DC->IASetInputLayout(inputLayout);
}
