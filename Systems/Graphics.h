#pragma once

//DirectX
/*
API - Application programming interface
응용 프로그램을 만들기 위한 인터페이스의 집합.

게임 프로그래밍을 위한 인터페이스들의 집합
GPU를 다룰수 있는 API 기능에 집중.

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
	ID3D11Device* device = nullptr; //자원을 생성하는 역활
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

	//ordinal -> 첫번째 두번째(서수)
	//cardinal -> 일 이 삼 사(기수)
	UINT adapterOrdinal = 0; //몇번째 그래픽카드인지.
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

	UINT numerator = 0; //분모
	UINT denominator = 1; //분자

private:
	const D3DEnumOutputInfo &operator = (const D3DEnumOutputInfo& rhs);
};