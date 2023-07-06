#include "PlayerBullet.h"
#include<cassert>
#include"Util.h"

void PlayerBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity)
{
	assert(model);

	//object3dの初期化
	Object3d::Initialize();
	//モデルのセット
	SetModel(model);

	//引数の座標をセット
	this->position = position;
	this->velocity = velocity;
}

void PlayerBullet::Update()
{
	//時間経過で死亡
	if (--deathTimer <= 0) {
		isAlive = false;
	}

	//移動
	position += velocity;

	//Z軸回転
	rotation.z += (float)PI / 180.0f * 36;

	//object3dの更新
	Object3d::Update();
}
