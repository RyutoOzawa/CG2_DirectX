#pragma once
#include<d3d12.h>
#include"DirectX.h"
#include<DirectXMath.h>

const int spriteSRVCount = 512;

struct VertexPosUv {
	DirectX::XMFLOAT3 pos;	//xyz座標
	DirectX::XMFLOAT2 uv;	//uv座標
};

class SpriteManager
{
	
public:
	ReDirectX* directX = nullptr;
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;		//パイプラインステート
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;		//ルートシグネチャ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeap;			//デスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12Resource> texbuff[spriteSRVCount];	//テクスチャリソースの配列
	DirectX::XMMATRIX matProjection;//射影行列


public:
	//初期化
	void Initialize(ReDirectX* directX,int windowWidth,int windowHeight);

	//描画前処理
	void beginDraw();
private:
	//スプライト用パイプラインステートとルートシグネチャの生成
	void CreatePipeline2D(ID3D12Device* dev);
};

