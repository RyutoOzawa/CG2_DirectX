#pragma once
//�|�X�g�G�t�F�N�g
//��ʂɗl�X�Ȍ��ʂ�������ׂ̃N���X

#include "Sprite.h"
#include"GpPipeline.h"

class PostEffect
{
public:
    //��ʃN���A�J���[
    static const float clearColor[4];
public://�T�u�N���X

    struct VertexPosUv {
        Vector3 pos;	//xyz���W
        Vector2 uv;	//uv���W
    };

    struct ConstBufferData {
        Vector4 color;
        Matrix4 mat;
    };

public://�����o�֐�


    /// <summary>
    /// �R���X�g���N�^
    /// </summary>
    PostEffect();

    /// <summary>
    /// �`��R�}���h�̔��s
    /// </summary>
    /// <param name="cmdList"></param>
    void Draw(ID3D12GraphicsCommandList* cmdList);

    /// <summary>
    /// ������
    /// </summary>
    void Initialize(ID3D12Device* device_);

    /// <summary>
    /// �V�[���`��O����
    /// </summary>
    /// <param name="cmdList">�R�}���h���X�g</param>
    void BeginDrawScene(ID3D12GraphicsCommandList* cmdList);

    /// <summary>
    /// �V�[���`��㏈��
    /// </summary>
    /// <param name="cmdList">�R�}���h���X�g</param>
    void EndDrawScene(ID3D12GraphicsCommandList* cmdList);

private://�����o�ϐ�
    ID3D12Device* device = nullptr;

    Microsoft::WRL::ComPtr<ID3D12Resource> texBuff;
    Microsoft::WRL::ComPtr<ID3D12Resource> depthBuff;
    Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff;
    Microsoft::WRL::ComPtr<ID3D12Resource> constBuff;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeapSRV;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeapRTV;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeapDSV;

    Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;		//�p�C�v���C���X�e�[�g
    Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;		//���[�g�V�O�l�`��

    D3D12_VERTEX_BUFFER_VIEW vbView;
    //GpPipeline pipeline;

    VertexPosUv vertices[4] = {};		//���_���W�f�[�^
    Vector4 color = { 1,1,1,1 };

private:
    void InitCrreteTexBuff();
    void InitCreateSRV();
    void InitCreateRTV();
    void InitCreateDepthBuff();
    void InitCreateDSV();
    void InitVertBuff();
    void InitConstBuff();

    void CreatePipeline2D();
    

};

