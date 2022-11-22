#pragma once
#include"Vector3.h"
#include"Matrix4.h"
#include<d3d12.h>
#include<wrl.h>
#include"DirectX.h"

//定数バッファ用データ構造体
struct ConstBufferDataWorldTransform {
	Matrix4 matWorld;
};

/// <summary>
/// ワールド変換データ
/// </summary>
class WorldTransform
{
	//directXのインスタンス
	static ReDirectX* directX;
	// 定数バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff;
	// マッピング済みアドレス
	ConstBufferDataWorldTransform* constMap = nullptr;
	// ローカルスケール
	Vector3 scale = { 1, 1, 1 };
	// X,Y,Z軸回りのローカル回転角
	Vector3 rotation = { 0, 0, 0 };
	// ローカル座標
	Vector3 translation = { 0, 0, 0 };
	// ローカル → ワールド変換行列
	Matrix4 matWorld;
	// 親となるワールド変換へのポインタ
	const WorldTransform* parent = nullptr;

	static void StaticInitialize(ReDirectX* directX_);

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialise();

	/// <summary>
	/// 定数バッファ生成
	/// </summary>
	void CreateConstBuffer();

	/// <summary>
	/// マッピング
	/// </summary>
	void Map();

	/// <summary>
	/// 行列の転送
	/// </summary>
	void TransferMatrix();

};

