#pragma once

#include<DirectXMath.h>
#include<d3d12.h>
#include<wrl.h>
#include"Transform.h"
#include"DirectX.h"
#include<vector>
#include<string>

class Object3d
{
public:

	//�T�u�N���X
		//���_�f�[�^�\����
	struct Vertex {
		DirectX::XMFLOAT3 pos;//xyz���W
		DirectX::XMFLOAT3 normal;//�@���x�N�g��
		DirectX::XMFLOAT2 uv;//uv���W
	};

	struct ConstBufferData {
		DirectX::XMFLOAT4 color;
		DirectX::XMMATRIX mat;
	};

	static Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;
	static Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
	static ReDirectX* directX;

	//���_���W���
	std::vector<Vertex> vertices;		//���_���W�f�[�^
	std::vector<unsigned short>indices;	//���_�C���f�b�N�X�f�[�^
	D3D12_VERTEX_BUFFER_VIEW vbView;	//���_�o�b�t�@�r���[
	D3D12_INDEX_BUFFER_VIEW ibView;		//�C���f�b�N�X�o�b�t�@�r���[
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff = nullptr;	//�萔�o�b�t�@�}�b�v�i�s��p�j
	Microsoft::WRL::ComPtr<ID3D12Resource> indexBuff = nullptr;	//�萔�o�b�t�@�}�b�v�i�s��p�j
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffTransform = nullptr;	//�萔�o�b�t�@�}�b�v�i�s��p�j
	//ConstBufferDataTransform* constMapTransform = nullptr;	//�萔�o�b�t�@�}�b�v�i�s��p�j

	ConstBufferData* constMap = nullptr;

	DirectX::XMFLOAT4 color = { 1,1,1,1 };
	DirectX::XMFLOAT3 scale = { 1,1,1 };	//�A�t�B���ϊ����
	DirectX::XMFLOAT3 rotation = { 0,0,0 };	//�A�t�B���ϊ����
	DirectX::XMFLOAT3 position = { 0,0,0 };	//�A�t�B���ϊ����
	DirectX::XMMATRIX matWorld;	//���[���h�ϊ��s��
	Object3d* parent = nullptr;	//�e�I�u�W�F�N�g�ւ̃|�C���^

public:
	Object3d();
	~Object3d();

	//�ÓI�����o�֐�
	static void StaticInitialize(ReDirectX* directX_);
	static void BeginDraw();

	void CreateModel(std::string filename = "NULL");
	void Initialize();
	void Update(DirectX::XMMATRIX& matView, DirectX::XMMATRIX& matProjection);
	void Draw();

private:
	static void CreatePipeline3D();
};

