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

	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffTransform;	//�萔�o�b�t�@�}�b�v�i�s��p�j
	ConstBufferDataTransform* constMapTransform = nullptr;	//�萔�o�b�t�@�}�b�v�i�s��p�j
	DirectX::XMFLOAT3 scale = { 1,1,1 };	//�A�t�B���ϊ����
	DirectX::XMFLOAT3 rotation = { 0,0,0 };	//�A�t�B���ϊ����
	DirectX::XMFLOAT3 position = { 0,0,0 };	//�A�t�B���ϊ����
	DirectX::XMMATRIX matWorld = {};	//���[���h�ϊ��s��
	Object3d* parent = nullptr;	//�e�I�u�W�F�N�g�ւ̃|�C���^

public:
	Object3d();
	~Object3d();
	void Initialize(ID3D12Device* device);
	void Update(DirectX::XMMATRIX& matView, DirectX::XMMATRIX& matProjection);
	void Draw(ID3D12GraphicsCommandList* commandList, D3D12_VERTEX_BUFFER_VIEW& vbView, D3D12_INDEX_BUFFER_VIEW& ibView, UINT numIndices);
};

