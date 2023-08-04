#pragma once
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

	struct Particle {
		//���W
		Vector3 position = {};
		//���x
		Vector3 velocity = {};
		//�����x
		Vector3 accel = {};
		//���݃t���[��
		int frame = 0;
		//�I���t���[��
		int num_frame = 0;
		//�X�P�[�����ݒl
		float scale = 1.0f;
		//�X�P�[���J�n�l
		float scaleStart = 1.0f;
		//�X�P�[���I���l
		float scaleEnd = 0.0f;

		//��Ԃ��ǂ���
		float isInter = false;
		//�n�_
		Vector3 start = {};
		//�I�_
		Vector3 end = {};
		//����
		float time = 0.0f;

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
	static void StaticInitialize(ReDirectX* directX_);
	static void BeginDraw(Camera* camera);

	void Initialize(TextureData* texData);

	void Update();

	void Draw();

	void SetTexture(TextureData* texData) { this->texData = texData; }

	/// <summary>
	/// �p�[�e�B�N���̒ǉ�
	/// </summary>
	/// <param name="life">��������</param>
	/// <param name="position">�������W</param>
	/// <param name="velocity">���x</param>
	/// <param name="accel">�����x</param>
	/// <param name="scaleStart">�J�n���X�P�[��</param>
	/// <param name="scaleEnd">�I�����X�P�[��</param>
	void Add(int life, const Vector3& position, const Vector3& velocity, const Vector3& accel,float scaleStart,float scaleEnd);

	/// <summary>
	/// �p�[�e�B�N���̒ǉ�(���`���ver)
	/// </summary>
	/// <param name="t">����</param>
	/// <param name="start">�n�_</param>
	/// <param name="end">�I�_</param>
	/// <param name="scaleStart">�J�n���X�P�[��</param>
	/// <param name="scaleEnd">�I�����X�P�[��</param>
	void AddLerp(int t, const Vector3& start, const Vector3& end, float scaleStart, float scaleEnd,InterType interType = InterType::Lerp);

private:
	static void CreatePipeline3D();

};

