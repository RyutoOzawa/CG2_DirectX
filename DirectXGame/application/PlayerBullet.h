#pragma once
#include"Object3d.h"
#include"Model.h"
#include"Vector3.h"

class PlayerBullet : public Object3d
{
public:

	//初期化
	void Initialize(Model* model,const Vector3& position,const Vector3& velocity);

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

