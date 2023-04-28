#pragma once
#include<string>
#include<DirectXMath.h>
#include<vector>
#include<DirectXTex.h>
#include<Windows.h>
#include<wrl.h>
#include<d3d12.h>
#include<d3dx12.h>
#include"Vector2.h"
#include"Vector3.h"
#include"Vector4.h"
#include"Matrix4.h"

struct Node {
	//名前
	std::string name;
	//ローカルスケール
	Vector4 scaling = { 1,1,1,0 };
	//ローカル回転角
	Vector4 rotation = { 0,0,0,0 };
	//ローカル移動
	Vector4 translation = { 0,0,0,1 };
	//ローカル変形行列
	Matrix4 transform;
	//グローバル変形行列
	Matrix4 grobalTransform;
	//親ノード
	Node* parent = nullptr;
};

class FbxModel
{
private:
	//Microsoft::WRL::を省略
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	//DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using TexMetadata = DirectX::TexMetadata;
	using ScratchImage = DirectX::ScratchImage;
	//std::を省略
	using string = std::string;
	template<class T> using vector = std::vector<T>;

public:
	//フレンドクラス
	friend class FbxLoader;

	//サブクラス
	struct VertexPosNormalUv {
		Vector3 pos;//XYZ座標
		Vector3 normal;//法線ベクトル
		Vector2 uv;//uv座標
	};
private:
	//モデル名
	string name;
	//ノード配列
	vector<Node> nodes;
	//メッシュを持つノード
	Node* meshNode = nullptr;
	//頂点データ配列
	vector < VertexPosNormalUv> vertices;
	//頂点インデックス配列
	vector<unsigned short> indices;
	//アンビエント係数
	Vector3 ambient = { 1,1,1 };
	//ディフューズ係数
	Vector3 diffuse = { 1,1,1 };
	//テクスチャメタデータ
	TexMetadata metadata = {};
	//スクラッチイメージ
	ScratchImage scratchImg = {};
	//頂点バッファ
	ComPtr<ID3D12Resource> vertBuff;
	//インデックスバッファ
	ComPtr<ID3D12Resource> indexBuff;
	//テクスチャバッファ
	ComPtr<ID3D12Resource> texBuff;
	//頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView = {};
	//インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW ibView = {};
	//SRV用でスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeapSRV;


public:
	//バッファ生成
	void CreateBuffers(ID3D12Device* device);

	//描画
	void Draw(ID3D12GraphicsCommandList* cmdList);

	//モデルの変形行列取得
	const Matrix4& GetModelTransform() { return meshNode->grobalTransform; }
};

