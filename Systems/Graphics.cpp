#include "stdafx.h"
#include "Graphics.h"

Graphics* Graphics::instance = nullptr;

void Graphics::SetBackBufferToRTV()
{

}

Graphics::Graphics()
{
	SetGPUInfo();
	CreateSwapChain();
}

Graphics::~Graphics()
{
	SAFE_RELEASE(rtv);
	SAFE_RELEASE(swapChain);
	SAFE_RELEASE(deviceContext);
	SAFE_RELEASE(device);
}

void Graphics::CreateSwapChain()
{
	SAFE_RELEASE(device);
	SAFE_RELEASE(deviceContext);
	SAFE_RELEASE(swapChain);

	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(DXGI_SWAP_CHAIN_DESC));

	desc.BufferDesc.Width = 0;
	desc.BufferDesc.Height = 0;

	if (bVsync)
	{
		desc.BufferDesc.RefreshRate.Numerator = adapterInfos[0]->outputInfo->numerator;
		desc.BufferDesc.RefreshRate.Denominator = adapterInfos[0]->outputInfo->denominator;
	}
	else
	{
		desc.BufferDesc.RefreshRate.Numerator = 0;
		desc.BufferDesc.RefreshRate.Denominator = 1;
	}

	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; //unsigned normalize
	desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	desc.BufferCount = 1;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.SampleDesc.Count = 1; 
	desc.SampleDesc.Quality = 0;
	desc.OutputWindow = handle;
	desc.Windowed = true;
	desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	desc.Flags = 0;

	UINT createFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
	
#if defined(_DEBUG)
	createFlags = D3DCOMPILE_PREFER_FLOW_CONTROL | D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	vector<D3D_FEATURE_LEVEL> featureLevel
	{
		D3D_FEATURE_LEVEL_11_1,
	};

	UINT maxVideoMemory = 0;
	for (UINT i = 0; i < adapterInfos.size(); i++)
	{
		if (adapterInfos[i]->adapterDesc.DedicatedVideoMemory > maxVideoMemory)
		{
			selectedAdapterIndex = i;
			maxVideoMemory = adapterInfos[i]->adapterDesc.DedicatedVideoMemory;
		}
	}

	{
		numerator = adapterInfos[0]->outputInfo->numerator;
		denominator = adapterInfos[0]->outputInfo->denominator;

		gpuMemorySize = adapterInfos[selectedAdapterIndex]->adapterDesc.DedicatedVideoMemory / 1024 / 1024;
		gpuDescription = adapterInfos[selectedAdapterIndex]->adapterDesc.Description;

		cout << "DedicatedVideoMemory : " << gpuMemorySize << endl;
		wcout << "GPU Description : " << gpuDescription << endl;

		cout << "Numerator : " << numerator << endl;
		cout << "Denominator : " << denominator << endl;
	}
	
	HRESULT hr = D3D11CreateDeviceAndSwapChain
	(
		adapterInfos[selectedAdapterIndex]->adapter,
		D3D_DRIVER_TYPE_UNKNOWN,
		nullptr,
		createFlags,
		featureLevel.data(),
		featureLevel.size(),
		D3D11_SDK_VERSION,
		&desc,
		&swapChain,
		&device,
		nullptr,
		&deviceContext
	);
	assert(SUCCEEDED(hr));

	Resize(WinMaxWidth, WinMaxHeight);
}

void Graphics::CreateRenderTargetView()
{
	ID3D11Texture2D* backBuffer = nullptr;
	HRESULT hr = swapChain->GetBuffer
	(
		0,
		__uuidof(ID3D11Texture2D),
		(void**)&backBuffer
	);
	assert(SUCCEEDED(hr));

	hr = device->CreateRenderTargetView
	(
		backBuffer,
		nullptr,
		&rtv
	);
	assert(SUCCEEDED(hr));
	SAFE_RELEASE(backBuffer);
}

void Graphics::DeleteSurface()
{
	SAFE_RELEASE(rtv);
}

void Graphics::EnumerateAdapaters()
{
	IDXGIFactory1* factory = nullptr;
	if (FAILED(CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)&factory)))
		return;

	UINT index = 0;
	while (true)
	{
		IDXGIAdapter1* adapter;
		//그래픽 카드 관련 기능을 담당하는 인터페이스
		//독립된 외장 그래픽 카드가 아닌 내장 그래픽카드인 경우도 있다

		HRESULT hr = factory->EnumAdapters1(index, &adapter);
		if (hr == DXGI_ERROR_NOT_FOUND)
			break;
		ASSERT(hr);

		D3DEnumAdapterInfo* adapterInfo = new D3DEnumAdapterInfo();
		ZeroMemory(adapterInfo, sizeof(D3DEnumAdapterInfo));
		adapterInfo->adapterOrdinal = index;
		adapter->GetDesc1(&adapterInfo->adapterDesc);
		adapterInfo->adapter = adapter;

		EnumerateAdapterOutput(adapterInfo);
		adapterInfos.push_back(adapterInfo);

		index++;
	}
	SAFE_RELEASE(factory);
}

bool Graphics::EnumerateAdapterOutput(D3DEnumAdapterInfo * adapterInfo)
{
	IDXGIOutput* output = nullptr;
	HRESULT hr = adapterInfo->adapter->EnumOutputs(0, &output);
	//연결되어 있는 디스플레이를 받아온다.

	if (DXGI_ERROR_NOT_FOUND == hr)
		return false;
	ASSERT(hr);

	D3DEnumOutputInfo* outputInfo = new D3DEnumOutputInfo();

	hr = output->GetDesc(&outputInfo->outputDesc);
	ASSERT(hr);

	outputInfo->output = output;

	UINT numModes = 0;
	DXGI_MODE_DESC* displayModes = nullptr;
	DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM;

	hr = output->GetDisplayModeList(format, DXGI_ENUM_MODES_INTERLACED, &numModes, nullptr);
	ASSERT(hr);

	displayModes = new DXGI_MODE_DESC[numModes];
	hr = output->GetDisplayModeList(format, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModes);
	ASSERT(hr);

	for (UINT i = 0; i < numModes; i++)
	{
		bool bCheck = true;
		bCheck &= displayModes[i].Width == WinMaxWidth;
		bCheck &= displayModes[i].Height == WinMaxHeight;

		if (bCheck)
		{
			outputInfo->numerator = displayModes[i].RefreshRate.Numerator;
			outputInfo->denominator = displayModes[i].RefreshRate.Denominator;
		}
	}
	adapterInfo->outputInfo = outputInfo;
	SAFE_DELETE_ARRAY(displayModes);

	return true;
}

void Graphics::SetGPUInfo()
{
	EnumerateAdapaters();
}

void Graphics::Create()
{
	if (instance == nullptr)
		instance = new Graphics();
}

void Graphics::Delete()
{
	SAFE_DELETE(instance);
}

Graphics * Graphics::Get()
{
	return instance;
}

void Graphics::Begin()
{
	deviceContext->OMSetRenderTargets(1, &rtv, nullptr); //최대 8개
	deviceContext->ClearRenderTargetView(rtv, clearColor);
	deviceContext->RSSetViewports(1, &viewport);
}

void Graphics::End()
{
	HRESULT hr = swapChain->Present(1, 0);
	assert(SUCCEEDED(hr));
}

void Graphics::Resize(const UINT & width, const UINT & height)
{
	DeleteSurface();
	{
		HRESULT hr = swapChain->ResizeBuffers
		(
			0,
			width,
			height,
			DXGI_FORMAT_UNKNOWN,
			0
		);
		ASSERT(hr);
	}
	CreateRenderTargetView();
	SetViewport(width, height);
}

void Graphics::SetViewport(const UINT & width, const UINT & height)
{
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	viewport.Width = (float)width;
	viewport.Height = (float)height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
}

D3DEnumAdapterInfo::~D3DEnumAdapterInfo()
{
	SAFE_RELEASE(adapter);
	SAFE_DELETE(outputInfo);
}

D3DEnumOutputInfo::~D3DEnumOutputInfo()
{
	SAFE_RELEASE(output);
}
