#pragma once

#include"FbxModel.h"
#include"Camera.h"

#include<Windows.h>
#include<wrl.h>
#include<d3d12.h>
#include<d3dx12.h>
#include<DirectXMath.h>
#include<string>
#include"Vector3.h"
#include"Matrix4.h"

class FbxObject3d
{
protected:
	//Microsoft::WRL::���ȗ�
	template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;

public://�ÓI�����o�֐�
	//setter
	static void SetDevice(ID3D12Device* device_) { device = device_; }
	static void SetCmdList(ID3D12GraphicsCommandList* cmdList_) { cmdList = cmdList_; }
	static void SetCamera(Camera* camera_) { camera = camera_; }

	/// <summary>
	/// �O���t�B�b�N�X�p�C�v���C���̐���
	/// </summary>
	static void CreateGraphicsPipeline();

private://�ÓI�����o�ϐ�
	//�f�o�C�X
	static ID3D12Device* device;
	//�R�}���h���X�g
	static ID3D12GraphicsCommandList* cmdList;
	//�J����
	static Camera* camera;
	//���[�g�V�O�l�`��
	static ComPtr<ID3D12RootSignature> rootsignature;
	//�p�C�v���C���X�e�[�g�I�u�W�F�N�g
	static ComPtr<ID3D12PipelineState> pipelinestate;

public://�萔
	//�{�[���̍ő吔
	static const int MAX_BONES = 32;



public://�T�u�N���X

	//�萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferDataTransform {
		Matrix4 viewProjection;	//�r���[�v���W�F�N�V�����s��
		Matrix4 world;			//���[���h�s��
		Vector3 cameraPos;		//�J�������W(���[���h���W)
	};

	//�萔�o�b�t�@�p�f�[�^�\����(�X�L�j���O)
	struct ConstBufferDataSkin {
		Matrix4 bones[MAX_BONES];
	};

public://�����o�֐�

	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update();

	/// <summary>
	/// ���f���̃Z�b�g
	/// </summary>
	/// <param name="fbxModel">fbx���f���f�[�^</param>
	void SetModel(FbxModel* fbxModel_) { fbxModel = fbxModel_; }

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �A�j���[�V�����J�n
	/// </summary>
	void PlayAnitimation();

	void SetRot(const Vector3& rotation_) { rotation = rotation_; }

	FbxTime BaGetCurrentTime() { return currentTime; }

protected://�����o�ϐ�
	//�萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuffTransform;
	//�萔�o�b�t�@(�X�L��)
	ComPtr<ID3D12Resource> constBuffSkin;

	//���[�J���X�P�[��
	Vector3 scale = { 1,1,1 };
	//X,Y,Z������̃��[�J����]�p
	Vector3 rotation = { 0,0,0 };
	//���[�J�����W
	Vector3 position = { 0,0,0 };
	//���[�J�����[���h�ϊ��s��
	Matrix4 matWorld;
	//���f��
	FbxModel* fbxModel = nullptr;

	//1�t���[���̎���
	FbxTime frameTime;
	//�A�j���[�V�����J�n����
	FbxTime startTime;
	//�A�j���[�V�����I������
	FbxTime endTime;
	//���ݎ���(�A�j���[�V����)
	FbxTime currentTime;
	//�A�j���[�V�����Đ���
	bool isPlay = false;
};

