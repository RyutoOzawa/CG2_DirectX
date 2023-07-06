#include "PlayerBullet.h"
#include<cassert>

void PlayerBullet::Initialize(Model* model, const Vector3& position)
{
	assert(model);

	//object3dの初期化
	Object3d::Initialize();
	//モデルのセット
	SetModel(model);

	//引数の座標をセット
	this->position = position;
}

void PlayerBullet::Update()
{
	//object3dの更新
	Object3d::Update();
}
