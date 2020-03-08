#pragma once

#define NOMINMAX

#include <string>
#include <stdio.h>
#include <stdlib.h> 
#include <time.h>
#include <Windows.h>
#include <assert.h>
#include <vector>
#include <unordered_map>
#include <functional>
#include <iostream>
#include <fstream>

using namespace std;

//Directx
#include <d3dcompiler.h>
#include <d3d11.h>
#include <D3DX10math.h>
#include <D3DX11async.h>

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")
#pragma comment(lib, "d3dcompiler.lib")

//ImGui
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"
#pragma comment(lib, "ImGui/ImGui.lib")

//FMOD
#pragma comment(lib, "FMod/fmodex_vc.lib")

//DirectXTex
#include <DirectTex/DirectXTex.h>
#pragma comment(lib, "DirectTex/directxtex.lib")

//JSONCPP
#include <json/json.h>
#pragma comment(lib, "json/jsoncpp.lib")


#include "./Systems/Keyboard.h"
#include "./Systems/Mouse.h"
#include "./Systems/Time.h"
#include "./Systems/Gui.h"

#define SAFE_DELETE(p) { if(p) { delete(p); p = nullptr; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[](p) ; p = nullptr; } }
#define SAFE_RELEASE(p) { if(p) { p->Release(); p = nullptr; }}

#define DC Graphics::Get()->GetDC()
#define DEVICE Graphics::Get()->GetDevice()

#define ASSERT(p) assert(SUCCEEDED(p))

#define WinMaxWidth 1280
#define WinMaxHeight 720

extern HWND handle;

#include "Utilities/Value.h"
#include "Utilities/String.h"
#include "Utilities/Path.h"
#include "Utilities/Math.h"
#include "Utilities/Singleton.h"

#include "Systems/Graphics.h"

#include "Renders/States.h"

#include "Renders/Resources/VertexType.h"
#include "Renders/Resources/ShaderBuffer.h"
#include "Renders/Resources/GlobalBuffers.h"
#include "Renders/Resources/Texture2D.h"

#include "Renders/IA/VertexBuffer.h"
#include "Renders/IA/IndexBuffer.h"
#include "Renders/IA/InputLayout.h"

#include "Renders/Shaders/IShader.h"
#include "Renders/Shaders/VertexShader.h"
#include "Renders/Shaders/PixelShader.h"

#include "Camera/Camera.h"
