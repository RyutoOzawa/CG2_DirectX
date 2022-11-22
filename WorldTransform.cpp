#include "WorldTransform.h"
#include<cassert>
#include<DirectXMath.h>
using namespace DirectX;

ReDirectX* WorldTransform::directX = nullptr;

void WorldTransform::StaticInitialize(ReDirectX* directX_)
{
	//NULL�`�F�b�N
	assert(directX_);

	//�����o�ɃR�s�[
	directX = directX_;
}

void WorldTransform::Initialise()
{
	CreateConstBuffer();
	Map();
	TransferMatrix();
}

void WorldTransform::CreateConstBuffer()
{
	HRESULT result;
	ID3D12Device* device = directX->GetDevice();
	D3D12_HEAP_PROPERTIES cbHeapProp{  };
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	//GPU�ւ̓]���p
	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC cbResourceDesc{};
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(ConstBufferDataWorldTransform) + 0xff) & ~0xff;	//256�o�C�g�A���C�������g
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	//�萔�o�b�t�@�̐���
	result = device->CreateCommittedResource(
		&cbHeapProp,//�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,//���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff));
	assert(SUCCEEDED(result));
}

void WorldTransform::Map()
{
	//�萔�o�b�t�@�̃}�b�s���O
	HRESULT result = constBuff->Map(0, nullptr, (void**)&constMap);//�}�b�s���O
	assert(SUCCEEDED(result));
}

void WorldTransform::TransferMatrix()
{
	Matrix4 matScale, matRot, matTrans;

	//�e�s��v�Z
	matScale = matScale.identity();
	matRot = matRot.identity();
	matTrans = matTrans.identity();
	matWorld = matWorld.identity();

	matScale = matScale.scale(scale);
	matRot *= matRot.rotateZ(rotation.z);
	matRot *= matRot.rotateX(rotation.x);
	matRot *= matRot.rotateY(rotation.y);
	matTrans = matTrans.translate(translation);

	matWorld *= matScale;
	matWorld *= matRot;
	matWorld *= matTrans;

	//�萔�o�b�t�@�ɓ]��
	constMap->matWorld = matWorld;

}
