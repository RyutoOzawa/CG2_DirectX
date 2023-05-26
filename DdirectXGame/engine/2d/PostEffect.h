#pragma once
#include "Sprite.h"
class PostEffect :
    public Sprite
{
public:
    //画面クリアカラー
    static const float clearColor[4];
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

    /// <summary>
    /// シーン描画前処理
    /// </summary>
    /// <param name="cmdList">コマンドリスト</param>
    void BeginDrawScene(ID3D12GraphicsCommandList* cmdList);

    /// <summary>
    /// シーン描画後処理
    /// </summary>
    /// <param name="cmdList">コマンドリスト</param>
    void EndDrawScene(ID3D12GraphicsCommandList* cmdList);

private://メンバ変数
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

