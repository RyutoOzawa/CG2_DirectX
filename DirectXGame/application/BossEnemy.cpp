#include "BossEnemy.h"
#include"ImguiManager.h"
#include"Util.h"

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
	for (int i = 0; i < barrelObject.size();i++) {
		barrelObject[i].Initialize();
		barrelObject[i].SetModel(barrelModel);
		barrelObject[i].parent = this;

		//オブジェクトから等間隔に離す
		Vector3 pos;
		barrelRadian[i] = 90.0f * i;
		pos.x = sinf((float)PI / 180.0f * barrelRadian[i]) * baseBarrelDistance;
		pos.y = cosf((float)PI / 180.0f * barrelRadian[i]) * baseBarrelDistance;
		barrelObject[i].position = pos;

	}

	position = { 0,0,240.0f };

}

void BossEnemy::Update(const Vector3& playerPos)
{
	static int a = 0;

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
	for (Object3d& barrel : barrelObject) {
		barrel.Draw();
	}
}

void BossEnemy::Finalize()
{
}

void BossEnemy::UpdateSpawn()
{
}

void BossEnemy::UpdateMove()
{
	ImGui::SliderFloat("x", &position.x, -100.0f, 100.0f);
	ImGui::SliderFloat("y", &position.y, -100.0f, 100.0f);
	ImGui::SliderFloat("z", &position.z, -100.0f, 100.0f);

	Object3d::Update();

	for (size_t i = 0; i < barrelObject.size(); i++) {
		barrelRadian[i] += 1.0f;
		if (barrelRadian[i] >= 360.0f) {
			barrelRadian[i] -= 360.0f;
		}

		Vector3 pos;
		pos.x = sinf((float)PI / 180.0f * barrelRadian[i]) * baseBarrelDistance;
		pos.y = cosf((float)PI / 180.0f * barrelRadian[i]) * baseBarrelDistance;
		barrelObject[i].position = pos;

		barrelObject[i].Update();

	}
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
