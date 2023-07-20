#include "EnemyBullet.h"
#include"SphereCollider.h"
#include"CollisionAttribute.h"
#include"Util.h"

void EnemyBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity)
{
	assert(model);

	//objの初期化
	Object3d::Initialize();
	//モデルセット
	SetModel(model);

	//引数の座標をセット
	this->position = position;
	this->velocity = velocity;

	SetCollider(new SphereCollider({ 0,0,0 }, 1.0f));
	collider->SetAttribute(COLLISION_ATTR_ENEMYS);
}

void EnemyBullet::Update()
{
	//時間経過で死亡
	if (--deathTimer <= 0) {
		isAlive = false;
	}

	//移動
	position += velocity;

	//Z軸回転
//	rotation.z += (float)PI / 180.0f * 36;

	//object3dの更新
	Object3d::Update();
}
