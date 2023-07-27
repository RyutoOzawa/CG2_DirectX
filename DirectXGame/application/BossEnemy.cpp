#include "BossEnemy.h"
#include"ImguiManager.h"
#include"Util.h"
#include"DebugLine.h"
#include"Input.h"

void BossEnemy::Initialize(Model* bodyModel, Model* barrelModel)
{
	//モデルのnullチェックとメンバへのセット
	assert(bodyModel);
	assert(barrelModel);
	this->bodyModel = bodyModel;
	this->BarrelModel = barrelModel;
	uint32_t tex = Texture::LoadTexture("reticle.png");

	for (size_t i = 0; i < 4; i++) {
		sp[i].Initialize(tex);
		sp[i].SetAnchorPoint({ 0.5f,0.5f });
	}

	//本体オブジェクト(継承元)の初期化
	Object3d::Initialize();
	SetModel(bodyModel);

	//TODO:砲台オブジェクトの初期化、モデルセット
	for (int i = 0; i < barrelObject.size(); i++) {
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

	//各クールタイムの初期化
	moveCooltime[(INT32)BossAct::Spawn] = 30;
	moveCooltime[(INT32)BossAct::Move] = 30;
	moveCooltime[(INT32)BossAct::AttackShot] = 30;
	moveCooltime[(INT32)BossAct::AttackLaser] = 30;
	moveCooltime[(INT32)BossAct::Death] = 30;

	//行動時間初期化
	actTime[(INT32)BossAct::Spawn] = 30;
	actTime[(INT32)BossAct::Move] = 120;
	actTime[(INT32)BossAct::AttackShot] = 120;
	actTime[(INT32)BossAct::AttackLaser] = 30;
	actTime[(INT32)BossAct::Death] = 30;

	ChangeAct(BossAct::Move);
}

void BossEnemy::Update(const Vector3& playerPos)
{

	//デバッグ用:キーで各行動開始
	if (Input::GetInstance()->IsKeyTrigger(DIK_1)) {
		ChangeAct(BossAct::AttackShot);
	}

	for (size_t i = 0; i < shotPosMax; i++) {
		ImGui::Text("pos screen[%d] %f:%f", i, shotPosScreen[i].x, shotPosScreen[i].y);
	}

	//行動時間を減らす
	if (nowActTime > 0) {
		nowActTime--;

		ImGui::Text("act index %d", (INT32)bossAct);
		ImGui::Text("position %f,%f,%f", position.x, position.y, position.z);

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
	else if (nowActTime == 0) {
		BossAct nextAct = BossAct::Move;

		switch (bossAct)
		{
		case BossAct::Spawn:
			break;
		case BossAct::Move:

			//TODO:ランダムで射撃かレーザーか決める

			//今はとりあえず確定で射撃
			nextAct = BossAct::AttackShot;

			break;
		case BossAct::AttackShot:
			break;
		case BossAct::AttackLaser:
			break;
		case BossAct::Death:
			break;
		case BossAct::BossActMax:
			break;
		default:
			break;
		}

		ChangeAct(nextAct);
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

void BossEnemy::DrawSprite()
{
	for (size_t i = 0; i < 4; i++) {
		sp[i].Draw();
	}
}

void BossEnemy::DrawDebugLine()
{

	curvePoints.clear();
	for (size_t i = 0; i < 360; i++) {
		Vector3 point;
		float theta = (float)PI / 180.0f * i;

		point.x = sinf(theta * radianX) * amplitudeX;
		point.y = sinf(theta * radianY) * amplitudeY;
		point.z = 240.0f;

		curvePoints.push_back(point);
	}

	DebugLine::Draw(curvePoints, { 0,1,0,1 });
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

	lissajousTheta += lThetaSpd;
	if (lissajousTheta > 360.0f) {
		lissajousTheta -= 360.0f;
	}


	ImGui::SliderFloat("amplitude X", &amplitudeX, 0.0f, 200.0f);
	ImGui::SliderFloat("amplitude Y", &amplitudeY, 0.0f, 200.0f);

	if (ImGui::Button("X+")) {
		radianX++;
	}
	ImGui::SameLine();
	if (ImGui::Button("X-")) {
		radianX--;
	}
	ImGui::SameLine();
	ImGui::Text("radX:%1f", radianX);


	if (ImGui::Button("Y+")) {
		radianY++;
	}
	ImGui::SameLine();
	if (ImGui::Button("Y-")) {
		radianY--;
	}
	ImGui::SameLine();
	ImGui::Text("radY:%1f", radianY);

	Vector3 point;
	point.x = sinf(lissajousTheta * radianX * (float)PI / 180.0f) * amplitudeX;
	point.y = sinf(lissajousTheta * radianY * (float)PI / 180.0f) * amplitudeY;
	point.z = position.z;
	position = point;

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
	//TODO:現在の座標をｽｸﾘｰﾝ座標に変換

	
	//30f間隔で敵を動かす
	size_t currentPosIndex = shotPosMax - (INT32)nowActTime / 30 - 1;
	if (fmodf((float)nowActTime, 30.0f) == 15.0f) {
		
		float posZ = position.z;
		Matrix4 matViewPort;
		matViewPort.identity();
		matViewPort.m[0][0] = WindowsAPI::winW / 2.0f;
		matViewPort.m[1][1] = -(WindowsAPI::winH / 2.0f);
		matViewPort.m[3][0] = WindowsAPI::winW / 2.0f;
		matViewPort.m[3][1] = WindowsAPI::winH / 2.0f;

		Matrix4 matVBV = Object3d::camera->GetViewProjection() * matViewPort;

		position = ConvertScreenToWorld( shotPosScreen[currentPosIndex],240.0f,matVBV);
		position.z = posZ;
		Object3d::Update();

	}

	ImGui::Text("time / 30 = %d", shotPosMax- (INT32)nowActTime / 30 - 1);
	


	//砲台座標は常に親に追従
	for (size_t i = 0; i < barrelMax; i++) {
		barrelObject[i].Update();
	}

}

void BossEnemy::UpdateAtkLaser()
{
}

void BossEnemy::UpdateDeath()
{
}

void BossEnemy::InitSpawn()
{
}

void BossEnemy::InitMove()
{
}

void BossEnemy::InitAtkShot()
{
	//ランダム座標のエリア
	float randomPosAreaX = WindowsAPI::winW / 2;
	float randomPosAreaY = WindowsAPI::winH / 2;

	for (size_t i = 0; i < shotPosMax; i++) {
		//x,y座標のみ決める(スクリーン座標で決めるので)
		shotPosScreen[i].x = Random(0, randomPosAreaX);
		shotPosScreen[i].y = Random(0, randomPosAreaY);
	}

	//オフセット
	shotPosScreen[1].y += randomPosAreaY;
	shotPosScreen[2].x += randomPosAreaX;
	shotPosScreen[3].x += randomPosAreaX;
	shotPosScreen[3].y += randomPosAreaY;
	for (size_t i = 0; i < 4; i++) {
		sp[i].SetPos(shotPosScreen[i]);
	}

	//行動変更

}

void BossEnemy::InitAtkLaser()
{
}

void BossEnemy::InitDeath()
{
}

void BossEnemy::ChangeAct(BossAct nextAct)
{
	bossAct = nextAct;
	nowActTime = actTime[(INT32)nextAct];
	switch (nextAct)
	{
	case BossAct::Spawn:
		InitSpawn();
		break;
	case BossAct::Move:
		InitMove();
		break;
	case BossAct::AttackShot:
		InitAtkShot();
		break;
	case BossAct::AttackLaser:
		InitAtkLaser();
		break;
	case BossAct::Death:
		InitDeath();
		break;
	default:
		break;
	}
}
