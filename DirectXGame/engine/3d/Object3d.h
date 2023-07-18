#pragma once

#include<DirectXMath.h>
#include<d3d12.h>
#include<wrl.h>
#include"Transform.h"
#include"DirectX.h"
#include<vector>
#include<string>
#include"Model.h"
#include"Camera.h"
#include"Matrix4.h"
#include"CollisionInfo.h"
#include"CollisionManager.h"

class BaseCollider;

class Object3d
{
public:



	struct ConstBufferData {
		//DirectX::XMFLOAT4 color;
		Matrix4 mat;
	};


	static Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;
	static Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
	static ReDirectX* directX;
	static Camera* camera;

	//���_���W���
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffB0;	//�萔�o�b�t�@�}�b�v�i�s��p�j
	
	ConstBufferData* constMap = nullptr;

	DirectX::XMFLOAT4 color = { 1,1,1,1 };
	Vector3 scale = { 1,1,1 };	//�A�t�B���ϊ����
	Vector3 rotation = { 0,0,0 };	//�A�t�B���ϊ����
	Vector3 position = { 0,0,0 };	//�A�t�B���ϊ����
	Matrix4 matRotation = matRotation.identity();
	Matrix4 matWorld;	//���[���h�ϊ��s��

	Object3d* parent = nullptr;	//�e�I�u�W�F�N�g�ւ̃|�C���^

	Model* model = nullptr;	//���f���f�[�^

	
	
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Object3d() = default;

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~Object3d();

	//�ÓI�����o�֐�
	static void StaticInitialize(ReDirectX* directX_);
	static void BeginDraw(Camera* camera);
	static void SetCamera(Camera* camera) { Object3d::camera = camera; }

	/// <summary>
	/// ������
	/// </summary>
	virtual void Initialize();

	virtual void Update();

	virtual void Draw();

	//getter
	Vector3 GetWorldPosition()const { return { matWorld.m[3][0],matWorld.m[3][1] ,matWorld.m[3][2] }; }
	Vector3 GetLocalPosition()const { return { position }; }

	//setter
	void SetModel(Model* model_) { model = model_; }
	
	/// <summary>
	/// ���[���h�s��̎擾
	/// </summary>
	/// <returns>���[���h�s��</returns>
	const Matrix4& GetMatWorld() { return matWorld; }

	/// <summary>
	/// �R���C�_�[�̃Z�b�g
	/// </summary>
	/// <param name="collider">�R���C�_�[</param>
	void SetCollider(BaseCollider* collider);

	virtual void OnCollision(const CollisionInfo& info) {}

protected:
	//�N���X��
	const char* name = nullptr;
	//�R���C�_�[
	BaseCollider* collider = nullptr;

private:
	static void CreatePipeline3D();


};

