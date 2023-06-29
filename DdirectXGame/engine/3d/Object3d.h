#pragma once

#include<DirectXMath.h>
#include<d3d12.h>
#include<wrl.h>
#include"Transform.h"
#include"DirectX.h"
#include<vector>
#include<string>
#include"Model.h"
#include"Camera.h"
#include"Matrix4.h"
#include"CollisionInfo.h"

class BaseCollider;

class Object3d
{
public:



	struct ConstBufferData {
		//DirectX::XMFLOAT4 color;
		Matrix4 mat;
	};


	static Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;
	static Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
	static ReDirectX* directX;

	//頂点座標情報
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffB0;	//定数バッファマップ（行列用）
	
	ConstBufferData* constMap = nullptr;

	DirectX::XMFLOAT4 color = { 1,1,1,1 };
	Vector3 scale = { 1,1,1 };	//アフィン変換情報
	Vector3 rotation = { 0,0,0 };	//アフィン変換情報
	Vector3 position = { 0,0,0 };	//アフィン変換情報
	Matrix4 matWorld;	//ワールド変換行列

	Matrix4 matBillboard = matBillboard.identity();	//ビルボード行列
	Matrix4 matBillboardY = matBillboardY.identity();	//Y軸周りビルボード行列
	bool isBillboard = false;
	bool isBillboardY = false;

	Object3d* parent = nullptr;	//親オブジェクトへのポインタ

	Model* model = nullptr;	//モデルデータ

	Camera* camera = nullptr;//ビルボード用のカメラ
	
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Object3d() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~Object3d();

	//静的メンバ関数
	static void StaticInitialize(ReDirectX* directX_);
	static void BeginDraw(Camera* camera);

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize();

	virtual void Update();

	virtual void Draw();

	void SetModel(Model* model_) { model = model_; }
	
	/// <summary>
	/// ワールド行列の取得
	/// </summary>
	/// <returns>ワールド行列</returns>
	const Matrix4& GetMatWorld() { return matWorld; }

	/// <summary>
	/// コライダーのセット
	/// </summary>
	/// <param name="collider">コライダー</param>
	void SetCollider(BaseCollider* collider);

	virtual void OnCollision(const CollisionInfo& info) {}

	//ビルボード行列の更新
	void UpdateBillBoard();

	//Y軸ビルボード行列の更新
	void UpdatebillboardY();

protected:
	//クラス名
	const char* name = nullptr;
	//コライダー
	BaseCollider* collider = nullptr;

private:
	static void CreatePipeline3D();


};

