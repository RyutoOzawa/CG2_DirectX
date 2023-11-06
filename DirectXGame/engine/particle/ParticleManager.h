#pragma once
//�p�[�e�B�N���}�l�[�W���N���X
//�p�[�e�B�N���̐����A�������ꂽ�p�[�e�B�N���̍X�V��폜�A�`����s��

#include"Vector2.h"
#include"Vector3.h"
#include"Vector4.h"
#include"Matrix4.h"
#include"Model.h"
#include"Camera.h"
#include<forward_list>
#include"Texture.h"
#include"BaseParticle.h"
#include"LerpParticle.h"


class ParticleManager
{
public:
	//�萔�o�b�t�@�p�\����
	struct ConstBufferData {
		Matrix4 matBillboard;
	};

	//���_�f�[�^�\����
	struct VertexPosScale {
		Vector3 pos;//xyz���W
		float scale;//�X�P�[��
	};

private:

	static Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;
	static Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
	static ReDirectX* directX;
	static Camera* camera;

	static const int vertexCount = 1024;

	D3D12_VERTEX_BUFFER_VIEW vbView;	//���_�o�b�t�@�r���[
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff;	//���_�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffB0;	//�萔�o�b�t�@�}�b�v�i�s��p�j
	TextureData* texData = nullptr;

	ConstBufferData* constMap = nullptr;
	Vector3 position;

	Matrix4 matBillboard = matBillboard.identity();
	Matrix4 matBillBoardY = matBillBoardY.identity();

//�p�[�e�B�N���z��
	std::forward_list<BaseParticle*> particles;

public:
	//�ÓI������
	static void StaticInitialize(ReDirectX* directX_);
	
	//�`��O����
	static void BeginDraw(Camera* camera);

	//������
	void Initialize(TextureData* texData_);

	//�X�V
	void Update();

	//�`��
	void Draw();

	//�e�N�X�`���f�[�^�Z�b�g
	void SetTexture(TextureData* texData_) { texData = texData_; }

	/// <summary>
	/// �p�[�e�B�N���̒ǉ�
	/// </summary>
	/// <param name="life">��������</param>
	/// <param name="position">�������W</param>
	/// <param name="velocity">���x</param>
	/// <param name="accel">�����x</param>
	/// <param name="scaleStart">�J�n���X�P�[��</param>
	/// <param name="scaleEnd">�I�����X�P�[��</param>
	void Add(int life, const Vector3& position_, const Vector3& velocity, const Vector3& accel,float scaleStart,float scaleEnd,Vector4 color = {1,1,1,1});

	/// <summary>
	/// �p�[�e�B�N���̒ǉ�(���`���ver)
	/// </summary>
	/// <param name="t">����</param>
	/// <param name="start">�n�_</param>
	/// <param name="end">�I�_</param>
	/// <param name="scaleStart">�J�n���X�P�[��</param>
	/// <param name="scaleEnd">�I�����X�P�[��</param>
	void AddLerp(int t, const Vector3& start, const Vector3& end, float scaleStart, float scaleEnd,InterType interType = InterType::Lerp, Vector4 color = { 1,1,1,1 });

private:
	//�p�C�v���C������
	static void CreatePipeline3D();

};

