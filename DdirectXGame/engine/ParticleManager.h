#pragma once
#include"Vector2.h"
#include"Vector3.h"
#include"Vector4.h"
#include"Matrix4.h"
#include"Model.h"
#include"Camera.h"

class ParticleManager
{
public:
	struct ConstBufferData {
		Matrix4 matBillboard;
	};

private:

	static Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;
	static Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
	static ReDirectX* directX;
	static Camera* camera;

	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffB0;	//定数バッファマップ（行列用）

	ConstBufferData* constMap = nullptr;
	Vector3 position;

	Matrix4 matBillboard = matBillboard.identity();
	Matrix4 matBillBoardY = matBillBoardY.identity();
	Model* model = nullptr;

public:
	static void StaticInitialize(ReDirectX* directX_);
	static void BeginDraw(Camera* camera);

	void Initialize();

	void Update();

	void Draw();

	void SetModel(Model* model) { this->model = model; }

private:
	static void CreatePipeline3D();

};

