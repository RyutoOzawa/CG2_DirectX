#pragma once
#include "Sprite.h"
class PostEffect :
    public Sprite
{
public:
    //��ʃN���A�J���[
    static const float clearColor[4];
public:

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
    void Initialize(uint32_t textureHandle);

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
    Microsoft::WRL::ComPtr<ID3D12Resource> texBuff;
    Microsoft::WRL::ComPtr<ID3D12Resource> depthBuff;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeapSRV;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeapRTV;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeapDSV;

private:
    void InitCrreteTexBuff();
    void InitCreateSRV();
    void InitCreateRTV();
    void InitCreateDepthBuff();
    void InitCreateDSV();
    void InitVertbuff();
    

};

