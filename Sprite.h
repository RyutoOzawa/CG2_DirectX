#pragma once
#include<DirectXMath.h>
#include<wrl.h>
#include<d3d12.h>

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

class Sprite
{
public:
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexBuff;	//���_�o�b�t�@
	D3D12_VERTEX_BUFFER_VIEW vbView;	//���_�o�b�t�@�r���[
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff;	//�萔�o�b�t�@

public:
	/// <summary>
	/// �X�v���C�g�̐���
	/// </summary>
	/// <param name="device"></param>
	/// <param name="window_width"></param>
	/// <param name="window_height"></param>
	void Create(ID3D12Device* device, int window_width, int window_height);

	/// <summary>
	/// �`��O����
	/// </summary>
	/// <param name="cmdList"></param>
	/// <param name="pipelineSet"></param>
	/// <param name="descHeap"></param>
	void BeginDraw(ID3D12GraphicsCommandList* cmdList, const PipelineSet& pipelineSet,
		ID3D12DescriptorHeap* descHeap);

	void EndDraw();

	/// <summary>
	/// �`�揈��
	/// </summary>
	/// <param name="cmdList"></param>
	void Draw(ID3D12GraphicsCommandList* cmdList);

};

