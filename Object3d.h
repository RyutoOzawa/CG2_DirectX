#pragma once

#include<DirectXMath.h>
#include<d3d12.h>
#include<wrl.h>
#include"Transform.h"
#include"DirectX.h"
#include<vector>
#include<string>

class Object3d
{
public:

	//サブクラス
		//頂点データ構造体
	struct Vertex {
		DirectX::XMFLOAT3 pos;//xyz座標
		DirectX::XMFLOAT3 normal;//法線ベクトル
		DirectX::XMFLOAT2 uv;//uv座標
	};

	struct ConstBufferData {
		DirectX::XMFLOAT4 color;
		DirectX::XMMATRIX mat;
	};

	static Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;
	static Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
	static ReDirectX* directX;

	//頂点座標情報
	std::vector<Vertex> vertices;		//頂点座標データ
	std::vector<unsigned short>indices;	//頂点インデックスデータ
	D3D12_VERTEX_BUFFER_VIEW vbView;	//頂点バッファビュー
	D3D12_INDEX_BUFFER_VIEW ibView;		//インデックスバッファビュー
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff = nullptr;	//定数バッファマップ（行列用）
	Microsoft::WRL::ComPtr<ID3D12Resource> indexBuff = nullptr;	//定数バッファマップ（行列用）
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffTransform = nullptr;	//定数バッファマップ（行列用）
	//ConstBufferDataTransform* constMapTransform = nullptr;	//定数バッファマップ（行列用）

	ConstBufferData* constMap = nullptr;

	DirectX::XMFLOAT4 color = { 1,1,1,1 };
	DirectX::XMFLOAT3 scale = { 1,1,1 };	//アフィン変換情報
	DirectX::XMFLOAT3 rotation = { 0,0,0 };	//アフィン変換情報
	DirectX::XMFLOAT3 position = { 0,0,0 };	//アフィン変換情報
	DirectX::XMMATRIX matWorld;	//ワールド変換行列
	Object3d* parent = nullptr;	//親オブジェクトへのポインタ

public:
	Object3d();
	~Object3d();

	//静的メンバ関数
	static void StaticInitialize(ReDirectX* directX_);
	static void BeginDraw();

	void CreateModel(std::string filename = "NULL");
	void Initialize();
	void Update(DirectX::XMMATRIX& matView, DirectX::XMMATRIX& matProjection);
	void Draw();

private:
	static void CreatePipeline3D();
};

