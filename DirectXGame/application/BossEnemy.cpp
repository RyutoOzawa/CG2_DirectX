#include "BossEnemy.h"

void BossEnemy::Initialize(Model* bodyModel, Model* barrelModel)
{
	//モデルのnullチェックとメンバへのセット
	assert(bodyModel);
	assert(barrelModel);
	this->bodyModel = bodyModel;
	this->BarrelModel = barrelModel;

	//本体オブジェクト(継承元)の初期化
	Object3d::Initialize();
	SetModel(bodyModel);

	//TODO:砲台オブジェクトの初期化、モデルセット

}

void BossEnemy::Update(const Vector3& playerPos)
{

	//各actで更新分ける
	switch (bossAct)
	{
	case BossAct::Spawn:
		UpdateSpawn();
		break;
	case BossAct::Move:
		UpdateMove();
		break;
	case BossAct::AttackShot:
		UpdateAtkShot();
		break;
	case BossAct::AttackLaser:
		UpdateAtkLaser();
		break;
	case BossAct::Death:
		UpdateDeath();
		break;
	default:
		break;
	}
	

}

void BossEnemy::Draw()
{

	//モデルの描画
	Object3d::Draw();

	//TODO: 砲台の描画

}

void BossEnemy::Finalize()
{
}

void BossEnemy::UpdateSpawn()
{
}

void BossEnemy::UpdateMove()
{
}

void BossEnemy::UpdateAtkShot()
{
}

void BossEnemy::UpdateAtkLaser()
{
}

void BossEnemy::UpdateDeath()
{
}
