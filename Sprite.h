#pragma once
#include<DirectXMath.h>
#include<wrl.h>
#include<d3d12.h>
#include"Texture.h"

//�p�C�v���C���X�e�[�g�ƃ��[�g�V�O�l�`���̃Z�b�g
struct PipelineSet {
	//�p�C�v���C���X�e�[�g�I�u�W�F�N�g
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;
	//���[�g�V�O�l�`��
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
};

struct VertexPosUv {
	DirectX::XMFLOAT3 pos;	//xyz���W
	DirectX::XMFLOAT2 uv;	//uv���W
};

//�萔�o�b�t�@�p�f�[�^�\����
struct ConstBufferData {
	DirectX::XMFLOAT4 color;	//�F(RGBA)
	DirectX::XMMATRIX mat;		//3D�ϊ��s��
};

const int spriteSRVCount = 512;						//�e�N�X�`���̍ő喇��

class Sprite
{
public:
	static Microsoft::WRL::ComPtr<ID3D12Resource> vertexBuff;	//���_�o�b�t�@
	static D3D12_VERTEX_BUFFER_VIEW vbView;					//���_�o�b�t�@�r���[
	static Microsoft::WRL::ComPtr<ID3D12Resource> constBuff;	//�萔�o�b�t�@
	static PipelineSet pipelineSet;							//�p�C�v���C���Z�b�g
	static DirectX::XMMATRIX matProjection;					//�ˉe�s��
	float rotatiton = 0.0f;								//Z������̉�]�p
	DirectX::XMFLOAT3 position = { 0,0,0 };				//���W
	DirectX::XMMATRIX matworld;							//���[���h�s��
	DirectX::XMFLOAT4 color = { 1,1,1,1 };				//�F(RGBA)
	static Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeap;			//�e�N�X�`���p�f�X�N���v�^�q�[�v
	static Microsoft::WRL::ComPtr<ID3D12Resource> texbuff[spriteSRVCount];	//�e�N�X�`�����\�[�X(�e�N�X�`���o�b�t�@)�̔z��
	UINT texNumber = 0;									//�e�N�X�`���ԍ�


public:
	/// <summary>
	/// �X�v���C�g�̐���
	/// </summary>
	/// <param name="device"></param>
	/// <param name="window_width"></param>
	/// <param name="window_height"></param>
	static void Create(ID3D12Device* device, int window_width, int window_height);

	/// <summary>
	/// �`��O����
	/// </summary>
	/// <param name="cmdList"></param>
	/// <param name="pipelineSet"></param>
	/// <param name="descHeap"></param>
	static void BeginDraw(ID3D12GraphicsCommandList* cmdList,ID3D12Device* dev);

	void EndDraw();

	/// <summary>
	/// �`�揈��
	/// </summary>
	/// <param name="cmdList"></param>
	void Draw(ID3D12GraphicsCommandList* cmdList,ID3D12Device* dev);

	/// <summary>
	/// �e�N�X�`���ǂݍ���
	/// </summary>
	/// <param name="texnumber"></param>
	/// <param name=""></param>
	/// <param name="dev"></param>
	void LoadTexture(UINT texnumber, const wchar_t* fineName, ID3D12Device* dev);

private:
	//�X�v���C�g�p�p�C�v���C������
	static PipelineSet CreatePipeline2D(ID3D12Device* dev);
};

