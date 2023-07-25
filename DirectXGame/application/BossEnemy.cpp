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
	Vector3 center = { 0,0,0 };
	Vector3 leftEdge = { -100,0,0 };
	Vector3 leftTop = { -50,50,0 };
	Vector3 leftBottom = { -50,-50,0 };
	Vector3 rightEdge = { 100,0,0 };
	Vector3 rightTop = { 50,50,0 };
	Vector3 rightBottom = { 50,-50,0 };


	std::vector<Vector3> movePoints{ center,leftTop,leftEdge,leftBottom,center,rightTop,rightEdge,rightBottom };
	for (Vector3& p : movePoints) {
		p += position;
	}

	moveSpline.SetPositions(movePoints);
	moveSpline.Start(240.0f, true);

	moveBezier.SetPositions(movePoints);
	moveBezier.Start(240.0f, true);

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
	for (Object3d& barrel : barrelObject) {
		barrel.Draw();
	}
}

void BossEnemy::DrawDebugLine()
{

	moveSpline.DrawCurve({ 0,1,1,1 });
	moveBezier.DrawCurve({ 1,0,0,1 });
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

	moveSpline.Update();
	position = moveSpline.GetPosition();

	moveBezier.Update();
	position = moveBezier.GetPosition();

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
