#pragma once
#include"Vector3.h"
#include"Matrix4.h"
#include<d3d12.h>
#include<wrl.h>
#include"DirectX.h"

//�萔�o�b�t�@�p�f�[�^�\����
struct ConstBufferDataWorldTransform {
	Matrix4 matWorld;
};

/// <summary>
/// ���[���h�ϊ��f�[�^
/// </summary>
class WorldTransform
{
public:
	//directX�̃C���X�^���X
	static ReDirectX* directX;
	// �萔�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff;
	// �}�b�s���O�ς݃A�h���X
	ConstBufferDataWorldTransform* constMap = nullptr;
	// ���[�J���X�P�[��
	Vector3 scale_ = { 1, 1, 1 };
	// X,Y,Z�����̃��[�J����]�p
	Vector3 rotation_ = { 0, 0, 0 };
	// ���[�J�����W
	Vector3 translation_ = { 0, 0, 0 };
	// ���[�J�� �� ���[���h�ϊ��s��
	Matrix4 matWorld_;
	// �e�ƂȂ郏�[���h�ϊ��ւ̃|�C���^
	const WorldTransform* parent_ = nullptr;

	static void StaticInitialize(ReDirectX* directX_);

	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	/// <summary>
	/// �萔�o�b�t�@����
	/// </summary>
	void CreateConstBuffer();

	/// <summary>
	/// �}�b�s���O
	/// </summary>
	void Map();

	/// <summary>
	/// �s��̓]��
	/// </summary>
	void TransferMatrix();

};

