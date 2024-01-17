#pragma once

//レールカメラクラス
//任意に決めたスプライン曲線の軌道上を移動するカメラオブジェクトクラス

#include"Camera.h"
#include"Object3d.h"
#include"SplineCurve.h"


class RailCamera
{

public://メンバ関数

	~RailCamera();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const Vector3& position,const Vector3& rotation);

	//レール開始
	void Start(float time = 300.0f);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	//getter
	Object3d* GetObject3d()const { return world; }
	Camera* GetCamera()const { return camera; }
	float GetProgress() const { return spline.GetProgress(); }

	//動線描画
	void DrawMoveLine(const Vector4& color);


private://メンバ変数

	//ワールド変換データ
	Object3d* world = nullptr;

	//カメラ
	Camera* camera = nullptr;

	//カメラ挙動用スプライン曲線
	SplineCurve spline;

	SplineCurve targetSpline;
};

