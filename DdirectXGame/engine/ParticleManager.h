#pragma once
#include"Vector2.h"
#include"Vector3.h"
#include"Vector4.h"
#include"Matrix4.h"
#include"Model.h"
#include"Camera.h"
#include<forward_list>

class ParticleManager
{
public:
	//定数バッファ用構造体
	struct ConstBufferData {
		Matrix4 matBillboard;
	};

	//頂点データ構造体
	struct VertexPosScale {
		Vector3 pos;//xyz座標
		float scale;//スケール
	};

	struct Particle {
		//座標
		Vector3 position = {};
		//速度
		Vector3 velocity = {};
		//加速度
		Vector3 accel = {};
		//現在フレーム
		int frame = 0;
		//終了フレーム
		int num_frame = 0;
		//スケール現在値
		float scale = 1.0f;
		//スケール開始値
		float scaleStart = 1.0f;
		//スケール終了値
		float scaleEnd = 0.0f;
	};

private:

	static Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;
	static Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
	static ReDirectX* directX;
	static Camera* camera;

	static const int vertexCount = 1024;

	D3D12_VERTEX_BUFFER_VIEW vbView;	//頂点バッファビュー
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff;	//頂点バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffB0;	//定数バッファマップ（行列用）
	uint32_t textureIndex = 0;	//テクスチャ番号

	ConstBufferData* constMap = nullptr;
	Vector3 position;

	Matrix4 matBillboard = matBillboard.identity();
	Matrix4 matBillBoardY = matBillBoardY.identity();

//パーティクル配列
	std::forward_list<Particle> particles;

public:
	static void StaticInitialize(ReDirectX* directX_);
	static void BeginDraw(Camera* camera);

	void Initialize(uint32_t texIndex);

	void Update();

	void Draw();

	void SetTexture(uint32_t texIndex) { textureIndex = texIndex; }

	/// <summary>
	/// パーティクルの追加
	/// </summary>
	/// <param name="life">生存時間</param>
	/// <param name="position">初期座標</param>
	/// <param name="velocity">速度</param>
	/// <param name="accel">加速度</param>
	/// <param name="scaleStart">開始時スケール</param>
	/// <param name="scaleEnd">終了時スケール</param>
	void Add(int life, const Vector3& position, const Vector3& velocity, const Vector3& accel,float scaleStart,float scaleEnd);
private:
	static void CreatePipeline3D();

};

