#pragma once
#include<DirectXMath.h>
#include<wrl.h>
#include<d3d12.h>

//パイプラインステートとルートシグネチャのセット
struct PipelineSet {
	//パイプラインステートオブジェクト
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;
	//ルートシグネチャ
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
};

struct VertexPosUv {
	DirectX::XMFLOAT3 pos;	//xyz座標
	DirectX::XMFLOAT2 uv;	//uv座標
};

//定数バッファ用データ構造体
struct ConstBufferData {
	DirectX::XMFLOAT4 color;	//色(RGBA)
	DirectX::XMMATRIX mat;		//3D変換行列
};

class Sprite
{
public:
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexBuff;	//頂点バッファ
	D3D12_VERTEX_BUFFER_VIEW vbView;	//頂点バッファビュー
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff;	//定数バッファ

public:
	/// <summary>
	/// スプライトの生成
	/// </summary>
	/// <param name="device"></param>
	/// <param name="window_width"></param>
	/// <param name="window_height"></param>
	void Create(ID3D12Device* device, int window_width, int window_height);

	/// <summary>
	/// 描画前処理
	/// </summary>
	/// <param name="cmdList"></param>
	/// <param name="pipelineSet"></param>
	/// <param name="descHeap"></param>
	void BeginDraw(ID3D12GraphicsCommandList* cmdList, const PipelineSet& pipelineSet,
		ID3D12DescriptorHeap* descHeap);

	void EndDraw();

	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="cmdList"></param>
	void Draw(ID3D12GraphicsCommandList* cmdList);

};

