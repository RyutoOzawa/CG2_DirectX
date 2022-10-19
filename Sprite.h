#pragma once
#include<DirectXMath.h>
#include<wrl.h>
#include<d3d12.h>
#include"Texture.h"

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

const int spriteSRVCount = 512;						//テクスチャの最大枚数

class Sprite
{
public:
	static Microsoft::WRL::ComPtr<ID3D12Resource> vertexBuff;	//頂点バッファ
	static D3D12_VERTEX_BUFFER_VIEW vbView;					//頂点バッファビュー
	static Microsoft::WRL::ComPtr<ID3D12Resource> constBuff;	//定数バッファ
	static PipelineSet pipelineSet;							//パイプラインセット
	static DirectX::XMMATRIX matProjection;					//射影行列
	float rotatiton = 0.0f;								//Z軸周りの回転角
	DirectX::XMFLOAT3 position = { 0,0,0 };				//座標
	DirectX::XMMATRIX matworld;							//ワールド行列
	DirectX::XMFLOAT4 color = { 1,1,1,1 };				//色(RGBA)
	static Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeap;			//テクスチャ用デスクリプタヒープ
	static Microsoft::WRL::ComPtr<ID3D12Resource> texbuff[spriteSRVCount];	//テクスチャリソース(テクスチャバッファ)の配列
	UINT texNumber = 0;									//テクスチャ番号


public:
	/// <summary>
	/// スプライトの生成
	/// </summary>
	/// <param name="device"></param>
	/// <param name="window_width"></param>
	/// <param name="window_height"></param>
	static void Create(ID3D12Device* device, int window_width, int window_height);

	/// <summary>
	/// 描画前処理
	/// </summary>
	/// <param name="cmdList"></param>
	/// <param name="pipelineSet"></param>
	/// <param name="descHeap"></param>
	static void BeginDraw(ID3D12GraphicsCommandList* cmdList,ID3D12Device* dev);

	void EndDraw();

	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="cmdList"></param>
	void Draw(ID3D12GraphicsCommandList* cmdList,ID3D12Device* dev);

	/// <summary>
	/// テクスチャ読み込み
	/// </summary>
	/// <param name="texnumber"></param>
	/// <param name=""></param>
	/// <param name="dev"></param>
	void LoadTexture(UINT texnumber, const wchar_t* fineName, ID3D12Device* dev);

private:
	//スプライト用パイプライン生成
	static PipelineSet CreatePipeline2D(ID3D12Device* dev);
};

