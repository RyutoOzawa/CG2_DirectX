#pragma once
#include "Sprite.h"
class PostEffect :
    public Sprite
{
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

private://�����o�ϐ�
    Microsoft::WRL::ComPtr<ID3D12Resource> texBuff;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeapSRV;
};

