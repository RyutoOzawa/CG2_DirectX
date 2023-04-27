#pragma once

#include"Model.h"
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
	static void SetDevice(ID3D12Device* device) { FbxObject3d::device = device; }
	static void SetCamera(Camera* camera) { FbxObject3d::camera = camera; }

	/// <summary>
	/// �O���t�B�b�N�X�p�C�v���C���̐���
	/// </summary>
	static void CreateGraphicsPipeline();

private://�ÓI�����o�ϐ�
	//�f�o�C�X
	static ID3D12Device* device;
	//�J����
	static Camera* camera;
	//���[�g�V�O�l�`��
	static ComPtr<ID3D12RootSignature> rootsignature;
	//�p�C�v���C���X�e�[�g�I�u�W�F�N�g
	static ComPtr<ID3D12PipelineState> pipelinestate;

public://�T�u�N���X
	//�萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferDataTransform {
		Matrix4 viewProjection;	//�r���[�v���W�F�N�V�����s��
		Matrix4 world;			//���[���h�s��
		Vector3 cameraPos;		//�J�������W(���[���h���W)
	};

public://�����o�֐�

	/// <summary>
	/// ������
	/// </summary>
	void Initialize();


protected://�����o�ϐ�
	//�萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuffTransform;

};

