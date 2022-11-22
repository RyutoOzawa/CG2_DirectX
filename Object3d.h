#pragma once

#include<DirectXMath.h>
#include<d3d12.h>
#include<wrl.h>
#include"Transform.h"
#include"DirectX.h"
#include<vector>
#include<string>
#include"WorldTransform.h"
#include "ViewProjection.h"
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
		//DirectX::XMFLOAT4 color;
		DirectX::XMMATRIX mat;
	};

	//�萔�o�b�t�@�p�f�[�^�\����b1
	struct ConstBufferDatab1 {
		DirectX::XMFLOAT3 ambient;	//�A���r�G���g�W��
		float pad1;			//�p�f�B���O
		DirectX::XMFLOAT3 diffuse;	//�f�B�t���[�Y�W��
		float pad2;			//�p�f�B���O
		DirectX::XMFLOAT3 specular;	//�X�y�L�����[�W��
		float alpha;		//�A���t�@
	};

	//�}�e���A��
	struct Material {
		std::string name;	//�}�e���A����
		DirectX::XMFLOAT3 ambient;	//�A���r�G���g�e���x
		DirectX::XMFLOAT3 diffuse;	//�f�B�t���[�Y�e���x
		DirectX::XMFLOAT3 specular;	//�X�y�L�����[�e���x
		float alpha;		//�A���t�@
		std::string textureFileName;//�e�N�X�`���t�@�C����
		//�R���X�g���N�^
		Material() {
			ambient = { 0.3f,0.3f,0.3f };
			diffuse = { 0.0f,0.0f,0.0f };
			specular = { 0.0f,0.0f,0.0f };
			alpha = 1.0f;
		}
	};

	static Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;
	static Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
	static ReDirectX* directX;

	//���_���W���
	std::vector<Vertex> vertices;		//���_���W�f�[�^
	std::vector<unsigned short>indices;	//���_�C���f�b�N�X�f�[�^
	D3D12_VERTEX_BUFFER_VIEW vbView;	//���_�o�b�t�@�r���[
	D3D12_INDEX_BUFFER_VIEW ibView;		//�C���f�b�N�X�o�b�t�@�r���[
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff; 	//�萔�o�b�t�@�}�b�v�i�s��p�j
	Microsoft::WRL::ComPtr<ID3D12Resource> indexBuff;	//�萔�o�b�t�@�}�b�v�i�s��p�j
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffB1;	//�萔�o�b�t�@�}�b�v�i�s��p�j�j
	//ConstBufferDataTransform* constMapTransform = nullptr;	//�萔�o�b�t�@�}�b�v�i�s��p�j


	DirectX::XMFLOAT4 color = { 1,1,1,1 };
	DirectX::XMFLOAT3 scale = { 1,1,1 };	//�A�t�B���ϊ����
	DirectX::XMFLOAT3 rotation = { 0,0,0 };	//�A�t�B���ϊ����
	DirectX::XMFLOAT3 position = { 0,0,0 };	//�A�t�B���ϊ����
	DirectX::XMMATRIX matWorld;	//���[���h�ϊ��s��
	Object3d* parent = nullptr;	//�e�I�u�W�F�N�g�ւ̃|�C���^
	uint32_t textureIndex = 0;
	static Material material;	//�}�e���A��

	
public:
	Object3d();
	~Object3d();

	//�ÓI�����o�֐�
	static void StaticInitialize(ReDirectX* directX_);
	static void BeginDraw();

	void CreateModel(const std::string& modelname);
	void Initialize(const std::string& filename = "NULL");
	void Update(DirectX::XMMATRIX& matView, DirectX::XMMATRIX& matProjection);
	void Draw(const WorldTransform& worldTransform,const ViewProjection &viewProjection);
	void LoadTexture(const std::string& directoryPath,const std::string& filename);

	void LoadMaterial(const std::string& directoryPath, const std::string& filename);

private:
	static void CreatePipeline3D();
};

