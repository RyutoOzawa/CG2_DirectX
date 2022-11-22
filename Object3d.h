#pragma once

#include<DirectXMath.h>
#include<d3d12.h>
#include<wrl.h>
#include"Transform.h"
#include"DirectX.h"
#include<vector>
#include<string>
#include"WorldTransform.h"
#include "ViewProjection.h"
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
		//DirectX::XMFLOAT4 color;
		DirectX::XMMATRIX mat;
	};

	//定数バッファ用データ構造体b1
	struct ConstBufferDatab1 {
		DirectX::XMFLOAT3 ambient;	//アンビエント係数
		float pad1;			//パディング
		DirectX::XMFLOAT3 diffuse;	//ディフューズ係数
		float pad2;			//パディング
		DirectX::XMFLOAT3 specular;	//スペキュラー係数
		float alpha;		//アルファ
	};

	//マテリアル
	struct Material {
		std::string name;	//マテリアル名
		DirectX::XMFLOAT3 ambient;	//アンビエント影響度
		DirectX::XMFLOAT3 diffuse;	//ディフューズ影響度
		DirectX::XMFLOAT3 specular;	//スペキュラー影響度
		float alpha;		//アルファ
		std::string textureFileName;//テクスチャファイル名
		//コンストラクタ
		Material() {
			ambient = { 0.3f,0.3f,0.3f };
			diffuse = { 0.0f,0.0f,0.0f };
			specular = { 0.0f,0.0f,0.0f };
			alpha = 1.0f;
		}
	};

	static Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;
	static Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
	static ReDirectX* directX;

	//頂点座標情報
	std::vector<Vertex> vertices;		//頂点座標データ
	std::vector<unsigned short>indices;	//頂点インデックスデータ
	D3D12_VERTEX_BUFFER_VIEW vbView;	//頂点バッファビュー
	D3D12_INDEX_BUFFER_VIEW ibView;		//インデックスバッファビュー
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff; 	//定数バッファマップ（行列用）
	Microsoft::WRL::ComPtr<ID3D12Resource> indexBuff;	//定数バッファマップ（行列用）
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffB1;	//定数バッファマップ（行列用））
	//ConstBufferDataTransform* constMapTransform = nullptr;	//定数バッファマップ（行列用）


	DirectX::XMFLOAT4 color = { 1,1,1,1 };
	DirectX::XMFLOAT3 scale = { 1,1,1 };	//アフィン変換情報
	DirectX::XMFLOAT3 rotation = { 0,0,0 };	//アフィン変換情報
	DirectX::XMFLOAT3 position = { 0,0,0 };	//アフィン変換情報
	DirectX::XMMATRIX matWorld;	//ワールド変換行列
	Object3d* parent = nullptr;	//親オブジェクトへのポインタ
	uint32_t textureIndex = 0;
	static Material material;	//マテリアル

	
public:
	Object3d();
	~Object3d();

	//静的メンバ関数
	static void StaticInitialize(ReDirectX* directX_);
	static void BeginDraw();

	void CreateModel(const std::string& modelname);
	void Initialize(const std::string& filename = "NULL");
	void Update(DirectX::XMMATRIX& matView, DirectX::XMMATRIX& matProjection);
	void Draw(const WorldTransform& worldTransform,const ViewProjection &viewProjection);
	void LoadTexture(const std::string& directoryPath,const std::string& filename);

	void LoadMaterial(const std::string& directoryPath, const std::string& filename);

private:
	static void CreatePipeline3D();
};

