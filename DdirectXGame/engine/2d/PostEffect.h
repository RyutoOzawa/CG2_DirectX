#pragma once
#include "Sprite.h"
class PostEffect :
    public Sprite
{
public:
    /// <summary>
    /// コンストラクタ
    /// </summary>
    PostEffect();

    /// <summary>
    /// 描画コマンドの発行
    /// </summary>
    /// <param name="cmdList"></param>
    void Draw(ID3D12GraphicsCommandList* cmdList);

    /// <summary>
    /// 初期化
    /// </summary>
    void Initialize(uint32_t textureHandle);

private://メンバ変数
    Microsoft::WRL::ComPtr<ID3D12Resource> texBuff;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeapSRV;
};

