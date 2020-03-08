#pragma once

//DirectX
/*
API - Application programming interface
���� ���α׷��� ����� ���� �������̽��� ����.

���� ���α׷����� ���� �������̽����� ����
GPU�� �ٷ�� �ִ� API ��ɿ� ����.

OpenGL
*/

class Graphics
{
public:
	static void Create();
	static void Delete();
	static Graphics* Get();

public:
	ID3D11Device* GetDevice() { return device; }
	ID3D11DeviceContext* GetDC() { return deviceContext; }

	void Begin();
	void End();

	void Resize(const UINT& width, const UINT& height);
	void SetViewport(const UINT& width, const UINT& height);

	D3DXCOLOR GetClearColor() { return clearColor; }

	void SetBackBufferToRTV();

private:
	Graphics();
	~Graphics();

	void CreateSwapChain();
	void CreateRenderTargetView();
	void DeleteSurface();

	void EnumerateAdapaters();
	bool EnumerateAdapterOutput(class D3DEnumAdapterInfo* adapterInfo);
	void SetGPUInfo();

	static Graphics* instance;

private:
	ID3D11Device* device = nullptr; //�ڿ��� �����ϴ� ��Ȱ
	ID3D11DeviceContext* deviceContext = nullptr;
	IDXGISwapChain* swapChain = nullptr;

	ID3D11RenderTargetView* rtv = nullptr;
	D3D11_VIEWPORT viewport = { 0 };
	D3DXCOLOR clearColor = 0xff555566;

	UINT numerator = 0;
	UINT denominator = 0;

	UINT gpuMemorySize = 0;
	wstring gpuDescription = L"";

	vector<class D3DEnumAdapterInfo* > adapterInfos;
	int selectedAdapterIndex = 0;

	bool bVsync = true;


};

///////////////////////////////////////////////
class D3DEnumAdapterInfo
{
public:
	D3DEnumAdapterInfo() = default;
	~D3DEnumAdapterInfo();

	//ordinal -> ù��° �ι�°(����)
	//cardinal -> �� �� �� ��(���)
	UINT adapterOrdinal = 0; //���° �׷���ī������.
	IDXGIAdapter1* adapter = nullptr;
	DXGI_ADAPTER_DESC1 adapterDesc = { 0 };

	class D3DEnumOutputInfo* outputInfo = nullptr;

private:
	const D3DEnumAdapterInfo& operator = (const D3DEnumAdapterInfo& rhs);
};

class D3DEnumOutputInfo
{
public:
	D3DEnumOutputInfo() = default;
	~D3DEnumOutputInfo();

	IDXGIOutput* output = nullptr;
	DXGI_OUTPUT_DESC outputDesc = { 0 };

	UINT numerator = 0; //�и�
	UINT denominator = 1; //����

private:
	const D3DEnumOutputInfo &operator = (const D3DEnumOutputInfo& rhs);
};