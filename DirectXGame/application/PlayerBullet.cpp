//自機の弾クラス
//自機が生成する弾

#include "PlayerBullet.h"
#include<cassert>
#include"Util.h"
#include"SphereCollider.h"
#include"CollisionAttribute.h"

void PlayerBullet::Initialize(Model* model_, const Vector3& position_, const Vector3& velocity_)
{
	assert(model_);

	//object3dの初期化
	Object3d::Initialize();
	//モデルのセット
	SetModel(model_);

	//引数の座標をセット
	position = position_;
	velocity = velocity_;

	SetCollider(new SphereCollider({ 0,0,0 }, 1.0f));
	collider->SetAttribute(COLLISION_ATTR_ALLIES);
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
