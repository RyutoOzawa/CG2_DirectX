#pragma once
//ポストエフェクト
//画面に様々な効果をかける為のクラス

#include "Sprite.h"
#include"GpPipeline.h"

class PostEffect
{
public:
    //画面クリアカラー
    static const float clearColor[4];
public://サブクラス

    struct VertexPosUv {
        Vector3 pos;	//xyz座標
        Vector2 uv;	//uv座標
    };

    struct ConstBufferData {
        Vector4 color;
        Matrix4 mat;
    };

public://メンバ関数


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
    void Initialize(ID3D12Device* device_);

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
    ID3D12Device* device = nullptr;

    Microsoft::WRL::ComPtr<ID3D12Resource> texBuff;
    Microsoft::WRL::ComPtr<ID3D12Resource> depthBuff;
    Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff;
    Microsoft::WRL::ComPtr<ID3D12Resource> constBuff;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeapSRV;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeapRTV;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeapDSV;

    Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;		//パイプラインステート
    Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;		//ルートシグネチャ

    D3D12_VERTEX_BUFFER_VIEW vbView;
    //GpPipeline pipeline;

    VertexPosUv vertices[4] = {};		//頂点座標データ
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

