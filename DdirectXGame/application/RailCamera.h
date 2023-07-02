#pragma once
#include"Camera.h"
#include"Object3d.h"

class RailCamera
{

public://メンバ関数

	~RailCamera();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const Vector3& position,const Vector3& rotation);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	Object3d* GetObject3d()const { return world; }

	Camera* GetCamera()const { return camera; }

private://メンバ変数

	//ワールド変換データ
	Object3d* world = nullptr;

	//カメラ
	Camera* camera = nullptr;

};

