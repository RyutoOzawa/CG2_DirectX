#pragma once
#include"Vector2.h"
#include"Vector3.h"
#include<string>
#include<vector>
#include<d3d12.h>
#include<wrl.h>
#include<memory>

#define MODEL_CUBE "MODEL_CUBE"
#define MODEL_PLANE "MODEL_PLANE"

class Model {

	//�T�u�N���X
	//���_�f�[�^�\����
	struct Vertex {
		Vector3 pos;	//xyz���W
		Vector3 normal;	//�@���x�N�g��
		Vector2 uv;		//uv���W
	};

	//�萔�o�b�t�@�p�\����
	struct ConstBufferDataMaterial {
		Vector3 ambient;//�A���r�G���g�W��
		float pad1;		//�p�f�B���O
		Vector3 diffuse;//�f�B�t���[�Y�W��
		float pad2;		//�p�f�B���O
		Vector3 specular;//�X�y�L�����[�W��
		float alpha;	//�A���t�@
	};

	//�}�e���A��
	struct Material {
		std::string name;	//�}�e���A����
		Vector3 ambient;	//�A���r�G���g�e���x
		Vector3 diffuse;	//�f�B�t���[�Y�e���x
		Vector3 specular;	//�X�y�L�����[�e���x
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

	//�����o�ϐ�
	std::vector<Vertex> vertices;		//���_�f�[�^�z��
	D3D12_VERTEX_BUFFER_VIEW vbView;	//���_�o�b�t�@�r���[
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff;	//���_�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> indexBuff;	//�C���f�b�N�X�o�b�t�@
	std::vector<unsigned short> indices;//�C���f�b�N�X�f�[�^�z��
	D3D12_INDEX_BUFFER_VIEW ibView;		//�C���f�b�N�X�o�b�t�@�r���[
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffMaterial;	//�萔�o�b�t�@
	Material material;			//�}�e���A��

public:
	static ID3D12Device* device;
	uint32_t textureIndex = 0;	//�e�N�X�`���ԍ�
	//�����o�֐�
	static std::unique_ptr< Model> CreateModel(const std::string& filename = MODEL_CUBE);

	static void SetDevice(ID3D12Device* dev) { device = dev; }

	void Draw(ID3D12GraphicsCommandList* cmdList);

	ID3D12Resource* GetCBMaterial()const { return constBuffMaterial.Get(); }

	//���������p�̔���J�����o�֐�
private:

	//���f������
	void Create(const std::string& modelname);
	void LoadTexture(const std::string& directoryPath, const std::string& filename);
	void LoadMaterial(const std::string& directoryPath, const std::string& filename);
	void CreateBuffers();

	//�t�@�C������ǂݍ���Ń��f������
	void CreateModelLoadFile(const std::string& modelname);

	//�L���[�u�̃��f������
	void CreateModelCube();

	//���ʂ̃��f���𐶐�
	void CreateModelPlane();

};