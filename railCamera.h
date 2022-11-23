#pragma once
#include"WorldTransform.h"
#include"ViewProjection.h"

#include "Input.h"
/// <summary>
/// 自キャラ
/// </summary>
class RailCamera {

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Vector3 translation, Vector3 rot,const WorldTransform* worldTransform);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	//シェイク関数
	Vector3 Shake();

	void PlayerOnCollision();

	void Rset();

	ViewProjection& GetViewProjection() { return viewProjection_; };

	WorldTransform* GetWorldTransform() { return &worldTransform_; };
private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//ビュープロジェクション
	ViewProjection viewProjection_;

	Input* input_ = nullptr;

	float PI = 3.1415927f;

	const int maxShakeCount=5;
	int shakeCount= maxShakeCount;

	bool shakeFrg = false;
};