#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#include <vector>
#include <string>
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#include"WindowsAPI.h"


class ReDirectX
{
public:
	//�e�평�����p�ϐ�
	ID3D12Debug* debugController;
	HRESULT result;
	ID3D12Device* device = nullptr;
	IDXGIFactory7* dxgiFactory = nullptr;
	IDXGISwapChain4* swapChain = nullptr;
	ID3D12CommandAllocator* commandAllocator = nullptr;
	ID3D12GraphicsCommandList* commandList = nullptr;
	ID3D12CommandQueue* commandQueue = nullptr;
	ID3D12DescriptorHeap* rtvHeap = nullptr;
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};
	std::vector<ID3D12Resource*> backBuffers;
	ID3D12Fence* fence = nullptr;
	UINT64 fenceVal = 0;
	D3D12_RESOURCE_DESC depthResourceDesc{  };
	D3D12_HEAP_PROPERTIES depthHeapProp{};
	D3D12_CLEAR_VALUE depthClearValue{};
	ID3D12Resource* depthBuff = nullptr;
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	ID3D12DescriptorHeap* dsvHeap = nullptr;
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
public:
	//������
	void Initialize(WindowsAPI windowsAPI);
	//�X�V����(���t���[������)
	void Update();
};

