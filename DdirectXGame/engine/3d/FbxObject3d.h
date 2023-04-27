#pragma once

#include"Model.h"
#include"Camera.h"

#include<Windows.h>
#include<wrl.h>
#include<d3d12.h>
#include<d3dx12.h>
#include<DirectXMath.h>
#include<string>
#include"Vector3.h"
#include"Matrix4.h"

class FbxObject3d
{
protected:
	//Microsoft::WRL::を省略
	template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;

public://静的メンバ関数
	//setter
	static void SetDevice(ID3D12Device* device) { FbxObject3d::device = device; }
	static void SetCamera(Camera* camera) { FbxObject3d::camera = camera; }

	/// <summary>
	/// グラフィックスパイプラインの生成
	/// </summary>
	static void CreateGraphicsPipeline();

private://静的メンバ変数
	//デバイス
	static ID3D12Device* device;
	//カメラ
	static Camera* camera;
	//ルートシグネチャ
	static ComPtr<ID3D12RootSignature> rootsignature;
	//パイプラインステートオブジェクト
	static ComPtr<ID3D12PipelineState> pipelinestate;

public://サブクラス
	//定数バッファ用データ構造体
	struct ConstBufferDataTransform {
		Matrix4 viewProjection;	//ビュープロジェクション行列
		Matrix4 world;			//ワールド行列
		Vector3 cameraPos;		//カメラ座標(ワールド座標)
	};

public://メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();


protected://メンバ変数
	//定数バッファ
	ComPtr<ID3D12Resource> constBuffTransform;

};

