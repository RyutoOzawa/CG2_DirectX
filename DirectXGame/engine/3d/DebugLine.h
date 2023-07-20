#pragma once
#include "Vector3.h"
#include"Vector4.h"
#include"Camera.h"
#include"DirectX.h"
#include<vector>

class DebugLine
{
public://静的メンバ関数

	/// <summary>
	/// 静的初期化
	/// </summary>
	static void StaticInitialize(ReDirectX* directX);

	//描画前処理	
	static void BeginDraw(Camera* camera);

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="points">座標配列</param>
	/// <param name="color">色</param>
	static void Draw(std::vector<Vector3> points,const Vector4& color);

public://静的メンバ変数とサブクラス

	static Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;
	static Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
	static ReDirectX* directX;
	static Camera* camera;

	struct VertexPos {
		Vector3 pos;
	};

	struct ConstBufferData {
		Matrix4 mat;
		Vector4 color;
	};


private:

	/// <summary>
	/// パイプライン設定
	/// </summary>
	static void CreatePipeline();

};
