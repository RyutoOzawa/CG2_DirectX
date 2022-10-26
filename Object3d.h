#pragma once

#include<DirectXMath.h>
#include<d3d12.h>
#include<wrl.h>
#include"Transform.h"



class Object3d
{
public:

	static Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;
	static Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
	static Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeap;
//static Microsoft::WRL::ComPtr<>

	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffTransform;	//定数バッファマップ（行列用）
	ConstBufferDataTransform* constMapTransform = nullptr;	//定数バッファマップ（行列用）
	DirectX::XMFLOAT3 scale = { 1,1,1 };	//アフィン変換情報
	DirectX::XMFLOAT3 rotation = { 0,0,0 };	//アフィン変換情報
	DirectX::XMFLOAT3 position = { 0,0,0 };	//アフィン変換情報
	DirectX::XMMATRIX matWorld = {};	//ワールド変換行列
	Object3d* parent = nullptr;	//親オブジェクトへのポインタ

public:
	Object3d();
	~Object3d();
	void Initialize(ID3D12Device* device);
	void Update(DirectX::XMMATRIX& matView, DirectX::XMMATRIX& matProjection);
	void Draw(ID3D12GraphicsCommandList* commandList, D3D12_VERTEX_BUFFER_VIEW& vbView, D3D12_INDEX_BUFFER_VIEW& ibView, UINT numIndices);
};

