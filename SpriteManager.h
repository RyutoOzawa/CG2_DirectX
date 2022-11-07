#pragma once
#include<d3d12.h>
#include"DirectX.h"
#include<DirectXMath.h>

const int spriteSRVCount = 512;

struct VertexPosUv {
	DirectX::XMFLOAT3 pos;	//xyz���W
	DirectX::XMFLOAT2 uv;	//uv���W
};

class SpriteManager
{
	
public:
	ReDirectX* directX = nullptr;
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;		//�p�C�v���C���X�e�[�g
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;		//���[�g�V�O�l�`��
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeap;			//�f�X�N���v�^�q�[�v
	Microsoft::WRL::ComPtr<ID3D12Resource> texbuff[spriteSRVCount];	//�e�N�X�`�����\�[�X�̔z��
	DirectX::XMMATRIX matProjection;//�ˉe�s��


public:
	//������
	void Initialize(ReDirectX* directX,int windowWidth,int windowHeight);

	//�`��O����
	void beginDraw();
private:
	//�X�v���C�g�p�p�C�v���C���X�e�[�g�ƃ��[�g�V�O�l�`���̐���
	void CreatePipeline2D(ID3D12Device* dev);
};

