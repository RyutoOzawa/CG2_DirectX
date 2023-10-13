#pragma once

//自機の弾クラス
//自機が生成する弾

#include"Object3d.h"
#include"Model.h"
#include"Vector3.h"

class PlayerBullet : public Object3d
{
public:

	//初期化
	void Initialize(Model* model_,const Vector3& position_,const Vector3& velocity_);

	void Update();

	bool IsAlive()const { return isAlive; }

private:

	static const int32_t lifeTime = 60 * 5;

	//速度
	Vector3 velocity;
	//デスタイマー
	int32_t deathTimer = lifeTime;
	//生存フラグ
	bool isAlive = true;



};

