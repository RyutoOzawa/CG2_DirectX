#include "Camera.h"
#include"WindowsAPI.h"
using namespace DirectX;
#include"Matrix4.h"
#include"Util.h"

ID3D12Device* Camera::device = nullptr;

void Camera::StaticInitialize(ID3D12Device* dev)
{
	//NULL�`�F�b�N
	assert(dev);
	device = dev;
}

void Camera::Initialize(const Vector3& eye, const Vector3& target, const Vector3& up)
{
	//�����������o�Ɉڂ��čs��X�V
	this->eye = eye;
	this->target = target;
	this->up = up;



	HRESULT result;
	//�q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES cbHeapProp{  };
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	//GPU�ւ̓]���p
	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC cbResourceDesc{};
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(ConstBufferCamera) + 0xff) & ~0xff;	//256�o�C�g�A���C�������g
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
	//�萔�o�b�t�@�̃}�b�s���O
	result = constBuff->Map(0, nullptr, (void**)&constMap);//�}�b�s���O
	assert(SUCCEEDED(result));

	UpdateMatrix();
}

void Camera::UpdateMatrix()
{
	//��p�̍s���錾
	matProjection = matProjection.CreatePerspectiveProjection(45.0f * (float)PI/ 180.0f, (float)WindowsAPI::winW / WindowsAPI::winH, nearZ, farZ);

	matView = matView.CreateViewMat(eye, target, up);
	//�r���[�ϊ��s��̌v�Z

	constMap->view = matView;
	constMap->projection = matProjection;
}