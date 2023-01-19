#pragma once
#include<imgui.h>
#include<imgui_impl_win32.h>
#include<imgui_impl_dx12.h>
#include<d3d12.h>
#include<dxgi1_4.h>
#include<tchar.h>

#ifdef _DEBUG
#define DX12_ENABLE_DEBUG_LAYER
#endif // _DEBUG

#ifdef DX12_ENABLE_DEBUG_LAYER
#include<dxgidebug.h>
#pragma comment(lib,"dxguid.lib")
#endif // DX12_ENABLE_DEBUG_LAYER



class ImguiManager
{
};

