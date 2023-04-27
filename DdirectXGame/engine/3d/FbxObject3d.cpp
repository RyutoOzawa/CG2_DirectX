#include "FbxObject3d.h"
#include<d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")

using namespace Microsoft::WRL;

/// <summary>
///静的メンバ変数の実態
/// </summary>
ID3D12Device* FbxObject3d::device = nullptr;
Camera* FbxObject3d::camera = nullptr;
ComPtr<ID3D12RootSignature> FbxObject3d::rootsignature;
ComPtr<ID3D12PipelineState> FbxObject3d::pipelinestate;

void FbxObject3d::Initialize()
{
	HRESULT result;
	//定数バッファの生成
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataTransform) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffTransform));
}
