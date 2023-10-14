#pragma once
//FBXファイルローダー
//FBXファイルの読み込みが煩雑なため、objと違いローダーとモデルでわかれている

#include "fbxsdk.h"

#include <d3d12.h>
#include <d3dx12.h>
#include<string>
#include"FbxModel.h"
#include<memory>

class FbxLoader
{
private:
	//std::を省略
	using string = std::string;


/// <summary>
/// メンバ関数
/// </summary>
public:
	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns>インスタンス</returns>
	static FbxLoader* GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="device">D3D12デバイス</param>
	void Initialize(ID3D12Device* device_);

	/// <summary>
	/// 後始末
	/// </summary>
	void Finalize();

	/// <summary>
	/// ファイルからFBXモデル読み込み
	/// </summary>
	/// <param name="modelName">モデルのファイル名</param>
	std::unique_ptr< FbxModel> LoadModelFromFile(const string& modelName);

	/// <summary>
	/// 再帰的にノード構成を解析
	/// </summary>
	/// <param name="fbxModel"></param>
	/// <param name="fbxNode"></param>
	void ParseNodeRecursive(FbxModel* fbxModel, FbxNode* fbxNode,Node* parent = nullptr);

	/// <summary>
	/// メッシュ読み取り
	/// </summary>
	/// <param name="fbxModel">読み込み先モデルオブジェクト</param>
	/// <param name="fbxNode">解析対象のノード</param>
	void ParseMesh(FbxModel* fbxModel, FbxNode* fbxNode);

	//ディレクトリを含んだファイルパスからファイル名を抽出する
	std::string ExtractFileName(const std::string& path);

	/// <summary>
/// FBXの行列をmatrix4に変換
/// </summary>
/// <param name="">書き込み先</param>
/// <param name="">元となるFBX行列</param>
	static void ConvertMatrixFromFbx(Matrix4* dst, const FbxAMatrix& src);

/// <summary>
/// メンバ変数
/// </summary>
public:

	//モデル格納ルートパス
	static const string baseDirectory;

private:
	// privateなコンストラクタ（シングルトンパターン）
	FbxLoader() = default;
	// privateなデストラクタ（シングルトンパターン）
	~FbxLoader() = default;
	// コピーコンストラクタを禁止（シングルトンパターン）
	FbxLoader(const FbxLoader& obj) = delete;
	// コピー代入演算子を禁止（シングルトンパターン）
	void operator=(const FbxLoader& obj) = delete;

	//D3D12デバイス
	ID3D12Device* device = nullptr;
	//FBXマネージャ
	FbxManager* fbxManager = nullptr;
	//FBXインポータ
	FbxImporter* fbxImporter = nullptr;

	//テクスチャがない場合の標準テクスチャファイル名
	static const string defaultTextureFileName;

	//頂点座標読み取り
	void ParseMeshVertices(FbxModel* fbxModel, FbxMesh* fbxMesh);
	//面情報読み取り
	void ParseMeshFaces(FbxModel* fbxModel, FbxMesh* fbxMesh);
	//マテリアル読み取り
	void ParseMaterial(FbxModel* fbxModel, FbxNode* fbxNode);
	//テクスチャ読み込み
	void LoadTexture(FbxModel* fbxModel, const std::string& fullPath);
	//スキニング情報の読み取り
	void ParseSkin(FbxModel* fbxModel,FbxMesh* fbxMesh);
};