#pragma once
#include<string>
#include<DirectXMath.h>
#include<vector>
#include<DirectXTex.h>
#include<Windows.h>
#include<wrl.h>
#include<d3d12.h>
#include<d3dx12.h>
#include"Vector2.h"
#include"Vector3.h"
#include"Vector4.h"
#include"Matrix4.h"
#include<fbxsdk.h>

struct Node {
	//���O
	std::string name;
	//���[�J���X�P�[��
	Vector4 scaling = { 1,1,1,0 };
	//���[�J����]�p
	Vector4 rotation = { 0,0,0,0 };
	//���[�J���ړ�
	Vector4 translation = { 0,0,0,1 };
	//���[�J���ό`�s��
	Matrix4 transform;
	//�O���[�o���ό`�s��
	Matrix4 grobalTransform;
	//�e�m�[�h
	Node* parent = nullptr;
};



class FbxModel
{
private:
	//Microsoft::WRL::���ȗ�
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	//DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using TexMetadata = DirectX::TexMetadata;
	using ScratchImage = DirectX::ScratchImage;
	//std::���ȗ�
	using string = std::string;
	template<class T> using vector = std::vector<T>;

public:
	//�{�[���C���f�b�N�X�̍ő吔
	static const int MAX_BONE_INDICES = 4;

	//�t�����h�N���X
	friend class FbxLoader;

	//�T�u�N���X
	struct VertexPosNormalUvSkin {
		Vector3 pos;//XYZ���W
		Vector3 normal;//�@���x�N�g��
		Vector2 uv;//uv���W
		UINT boneIndex[MAX_BONE_INDICES];//�{�[���ԍ�
		float boneWeight[MAX_BONE_INDICES];//�{�[���̏d��
	};

	struct Bone {
		//���O
		std::string name;
		//�����p���̋t�s��
		Matrix4 invInitialPose;
		//�N���X�^�[(FBX�K���̃{�[�����)
		FbxCluster* fbxCluster;
		//�R���X�g���N�^
		Bone(const std::string& name) {
			this->name = name;
		}
	};
private:
	//���f����
	string name;
	//�m�[�h�z��
	vector<Node> nodes;
	//���b�V�������m�[�h
	Node* meshNode = nullptr;
	//���_�f�[�^�z��
	vector < VertexPosNormalUvSkin> vertices;
	//���_�C���f�b�N�X�z��
	vector<unsigned short> indices;
	//�A���r�G���g�W��
	Vector3 ambient = { 1,1,1 };
	//�f�B�t���[�Y�W��
	Vector3 diffuse = { 1,1,1 };
	//�e�N�X�`�����^�f�[�^
	TexMetadata metadata = {};
	//�X�N���b�`�C���[�W
	ScratchImage scratchImg = {};
	//���_�o�b�t�@
	ComPtr<ID3D12Resource> vertBuff;
	//�C���f�b�N�X�o�b�t�@
	ComPtr<ID3D12Resource> indexBuff;
	//�e�N�X�`���o�b�t�@
	ComPtr<ID3D12Resource> texBuff;
	//���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView = {};
	//�C���f�b�N�X�o�b�t�@�r���[
	D3D12_INDEX_BUFFER_VIEW ibView = {};
	//SRV�p�ŃX�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descHeapSRV;
	//�{�[���z��
	vector<Bone> bones;
	//FBX�V�[��
	FbxScene* fbxScene = nullptr;

public:

	//�f�X�g���N�^
	~FbxModel();

	//�o�b�t�@����
	void CreateBuffers(ID3D12Device* device);

	//�`��
	void Draw(ID3D12GraphicsCommandList* cmdList);

	//���f���̕ό`�s��擾
	const Matrix4& GetModelTransform() { return meshNode->grobalTransform; }

	//�{�[���̃Q�b�^�[
	vector<Bone>& GetBones() { return bones; }

	//FBX�V�[���̃Q�b�^�[
	FbxScene* GetFbxScene() { return fbxScene; }
};

