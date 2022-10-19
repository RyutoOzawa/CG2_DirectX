#include "Sprite.h"
#include<d3dx12.h>
using namespace DirectX;

void Sprite::Create(ID3D12Device* device, int window_width, int window_height)
{
	HRESULT result = S_FALSE;

	//���_�f�[�^
	VertexPosUv vertices[] = {
		//  x       y      z      u     v
		{{  0.0f, 100.0f, 0.0f},{0.0f,1.0f}},	//����
		{{  0.0f,   0.0f, 0.0f},{0.0f,0.0f}},	//����
		{{100.0f, 100.0f, 0.0f},{1.0f,1.0f}},	//�E��
		{{100.0f,   0.0f, 0.0f},{1.0f,0.0f}},	//�E��
	};


	//�q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES heapProp{};
	heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC resDesc{};
	resDesc = CD3DX12_RESOURCE_DESC::Buffer(sizeof(vertices));

	//���_�o�b�t�@����
	result = device->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&vertexBuff));

	//���_�o�b�t�@�ւ̃f�[�^�]��
	VertexPosUv* vertMap = nullptr;
	result = vertexBuff->Map(0, nullptr, (void**)&vertMap);
	memcpy(vertMap, vertices, sizeof(vertices));
	vertexBuff->Unmap(0, nullptr);

	//���_�o�b�t�@�r���[�̐���
	vbView.BufferLocation = vertexBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeof(vertices);
	vbView.StrideInBytes = sizeof(vertices[0]);

	//�q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	cbHeapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);

	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC cbResourceDesc{};
	cbResourceDesc = CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) * 0xff) & ~0xff);

	//�萔�o�b�t�@
	result = device->CreateCommittedResource(
		&cbHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,nullptr,
		IID_PPV_ARGS(&constBuff));

	//�萔�o�b�t�@�Ƀf�[�^�]��
	ConstBufferData* constMap = nullptr;
	result = constBuff->Map(0, nullptr, (void**)&constMap);
	constMap->color = XMFLOAT4(1, 1, 1, 1);	//�F�w��(RGBA)
	//���s���e�s��
	constMap->mat = XMMatrixOrthographicOffCenterLH(
		0.0f, window_width, window_height, 0.0f, 0.0f, 1.0f);
	constBuff->Unmap(0, nullptr);
}

void Sprite::BeginDraw(ID3D12GraphicsCommandList* cmdList, const PipelineSet& pipelineSet, ID3D12DescriptorHeap* descHeap)
{
	//�p�C�v���C���X�e�[�g�̐ݒ�
	cmdList->SetPipelineState(pipelineSet.pipelineState.Get());
	//���[�g�V�O�l�`���̐ݒ�
	cmdList->SetGraphicsRootSignature(pipelineSet.rootSignature.Get());
	//�v���~�e�B�u�`��̐ݒ�
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//�e�N�X�`���p�f�X�N���v�^�q�[�v�̐ݒ�
	ID3D12DescriptorHeap* ppHeaps[] = { descHeap };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
}

void Sprite::Draw(ID3D12GraphicsCommandList* cmdList)
{
	//���_�o�b�t�@�Z�b�g
	cmdList->IASetVertexBuffers(0, 1, &vbView);
	//�萔�o�b�t�@�Z�b�g
	cmdList->SetGraphicsRootConstantBufferView(0, constBuff.Get()->GetGPUVirtualAddress());
	//�`��
	cmdList->DrawInstanced(4, 1, 0, 0);
}
