#include "Sprite.h"
using namespace DirectX;
#include<wrl.h>
using namespace Microsoft::WRL;

void Sprite::Initialize(SpriteManager* spriteManager)
{
	this->spriteManager = spriteManager;
	HRESULT result;

	ComPtr<ID3D12Device> dev = spriteManager->directX->GetDevice();

	//���_�f�[�^
	XMFLOAT3 vertices_[] = {
		{-0.5f,-0.5f,0.0f},//����
		{-0.5f,+0.5f,0.0f},//����si
		{+0.5f,-0.5f,0.0f},//�E��
		{+0.5f,+0.5f,0.0f},//�E��
	};

	for (int i = 0; i < _countof(vertices_); i++) {
		vertices[i] = vertices_[i];
	}

	//���_�f�[�^�S�̂̃T�C�Y = ���_�f�[�^����̃T�C�Y * ���_�f�[�^�v�f��
	UINT sizeVB = static_cast<UINT>(sizeof(XMFLOAT3) * _countof(vertices_));

	//���_�o�b�t�@�̐ݒ�
	D3D12_HEAP_PROPERTIES heapProp{};		//�q�[�v�ݒ�
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	//GPU�ւ̓]���p
	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeVB;
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//���_�o�b�t�@����
	result = dev.Get()->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	assert(SUCCEEDED(result));

	//GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
	XMFLOAT3* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	//�S���_�ɑ΂���
	for (int i = 0; i < _countof(vertices_); i++) {
		vertMap[i] = vertices_[i];
	}
	//�Ȃ��������
	vertBuff->Unmap(0, nullptr);


	//GPU���z�A�h���X
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	//���_�o�b�t�@�̃T�C�Y
	vbView.SizeInBytes = sizeVB;
	//���_�P���̃f�[�^�T�C�Y
	vbView.StrideInBytes = sizeof(XMFLOAT3);


}

void Sprite::Draw()
{
	//���_�o�b�t�@�r���[�̐ݒ�
	spriteManager->directX->GetCommandList()->IASetVertexBuffers(0, 1, &vbView);
	//�`��R�}���h
	spriteManager->directX->GetCommandList()->DrawInstanced(_countof(vertices), 1, 0, 0);
}
