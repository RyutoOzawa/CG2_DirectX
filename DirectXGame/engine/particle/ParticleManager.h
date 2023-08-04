#pragma once
#include"Vector2.h"
#include"Vector3.h"
#include"Vector4.h"
#include"Matrix4.h"
#include"Model.h"
#include"Camera.h"
#include<forward_list>
#include"Texture.h"
#include"BaseParticle.h"
#include"LerpParticle.h"


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

		//補間かどうか
		float isInter = false;
		//始点
		Vector3 start = {};
		//終点
		Vector3 end = {};
		//時間
		float time = 0.0f;

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
	TextureData* texData = nullptr;

	ConstBufferData* constMap = nullptr;
	Vector3 position;

	Matrix4 matBillboard = matBillboard.identity();
	Matrix4 matBillBoardY = matBillBoardY.identity();

//パーティクル配列
	std::forward_list<BaseParticle*> particles;

public:
	static void StaticInitialize(ReDirectX* directX_);
	static void BeginDraw(Camera* camera);

	void Initialize(TextureData* texData);

	void Update();

	void Draw();

	void SetTexture(TextureData* texData) { this->texData = texData; }

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

	/// <summary>
	/// パーティクルの追加(線形補間ver)
	/// </summary>
	/// <param name="t">時間</param>
	/// <param name="start">始点</param>
	/// <param name="end">終点</param>
	/// <param name="scaleStart">開始時スケール</param>
	/// <param name="scaleEnd">終了時スケール</param>
	void AddLerp(int t, const Vector3& start, const Vector3& end, float scaleStart, float scaleEnd,InterType interType = InterType::Lerp);

private:
	static void CreatePipeline3D();

};

