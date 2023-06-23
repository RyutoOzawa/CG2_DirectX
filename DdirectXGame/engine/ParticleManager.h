#pragma once
#include"Vector2.h"
#include"Vector3.h"
#include"Vector4.h"
#include"Matrix4.h"
#include"Model.h"
#include"Camera.h"
#include<forward_list>

class ParticleManager
{
public:
	//�萔�o�b�t�@�p�\����
	struct ConstBufferData {
		Matrix4 matBillboard;
	};

	//���_�f�[�^�\����
	struct VertexPos {
		Vector3 pos;
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
	uint32_t textureIndex = 0;	//�e�N�X�`���ԍ�

	ConstBufferData* constMap = nullptr;
	Vector3 position;

	Matrix4 matBillboard = matBillboard.identity();
	Matrix4 matBillBoardY = matBillBoardY.identity();

//�p�[�e�B�N���z��
	std::forward_list<Particle> particles;

public:
	static void StaticInitialize(ReDirectX* directX_);
	static void BeginDraw(Camera* camera);

	void Initialize(uint32_t texIndex);

	void Update();

	void Draw();

	void SetTexture(uint32_t texIndex) { textureIndex = texIndex; }

	/// <summary>
	/// �p�[�e�B�N���̒ǉ�
	/// </summary>
	/// <param name="life">��������</param>
	/// <param name="position">�������W</param>
	/// <param name="velocity">���x</param>
	/// <param name="accel">�����x</param>
	void Add(int life, Vector3 position, Vector3 velocity, Vector3 accel);
private:
	static void CreatePipeline3D();

};

