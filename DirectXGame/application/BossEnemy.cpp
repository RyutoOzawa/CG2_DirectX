#include "BossEnemy.h"
#include"ImguiManager.h"
#include"Util.h"
#include"DebugLine.h"
#include"Input.h"

void BossEnemy::Initialize(Model* bodyModel, Model* barrelModel, Object3d* parent)
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

	this->parent = parent;

	bulletModel = std::make_unique<Model>();
	bulletModel = Model::CreateModel("PlayerBullet");

	//砲台オブジェクトの初期化、モデルセット
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

	//弾射出座標のオフセットを設定
	bulletOutOffset = { 0,0,17.0f };

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
	actTime[(INT32)BossAct::Spawn] = 300;
	actTime[(INT32)BossAct::Move] = 120;
	actTime[(INT32)BossAct::AttackShot] = 200;
	actTime[(INT32)BossAct::AttackLaser] = 30;
	actTime[(INT32)BossAct::Death] = 30;

	ChangeAct(BossAct::Spawn);


}

void BossEnemy::Update(const Vector3& playerPos)
{
	targetPos = playerPos;

	//デバッグ用:キーで各行動開始
	if (Input::GetInstance()->IsKeyTrigger(DIK_1)) {
		ChangeAct(BossAct::AttackShot);
	}

	//bossAct = BossAct::Spawn;

	//ImGui::SliderFloat("x", &position.x, -100.0f, 100.0f);
	//ImGui::SliderFloat("y", &position.y, -100.0f, 100.0f);
	//ImGui::SliderFloat("z", &position.z, -100.0f, 100.0f);
	//ImGui::SliderFloat("rotY", &rotation.y, -(float)PI,(float) PI);
	//Object3d::Update();

	for (size_t i = 0; i < shotPosMax; i++) {
		ImGui::Text("pos screen[%d] %f:%f", i, shotPos[i].x, shotPos[i].y);
	}

	//matRotation = Matrix4::CreateMatRot(position, targetPos, camera->up);

	//弾の更新
	//死んでる弾を消す
	bullets.remove_if([](std::unique_ptr<EnemyBullet>& bullet) {
		if (!bullet->IsAlive()) {
			return true;
		}
		return false;
		});


	for (std::unique_ptr<EnemyBullet>& bullet : bullets) {
		bullet->Update();
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
	for (std::unique_ptr<EnemyBullet>& bullet : bullets) {
		bullet->Draw();
	}

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
	if (bossAct == BossAct::Move) {

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
}

void BossEnemy::Finalize()
{
}

void BossEnemy::Spawn(const Matrix4& cameraMatWorld,const Vector3& spawnPos)
{
	//目玉の移動開始地点を設定(カメラ行列と掛ける)
	Vector3 posFirst = Matrix4::transform( spawnPos, cameraMatWorld);
	//生存フラグとライフの設定
	isAlive = true;
	life = lifeMax;

	//フェーズをスポーンにする
	ChangeAct(BossAct::Spawn);
}

void BossEnemy::UpdateSpawn()
{
	eDataMove.Update();

	//イージングの進み具合を取得(計算しやすくするため100倍)
	INT32 easeProgress = (INT32)(eDataMove.GetTimeRate() * 100.0f);

	ImGui::Text("progress %d", easeProgress);



	//最初の40%で砲台で砲台を動かす(10%ずつ)
	if (easeProgress < 40) {
		//10%ごとにイージングを開始していく
		ImGui::Text("mod %d", easeProgress % 10);
		ImGui::Text("index %d",easeProgress / 10);
		ImGui::Text("count %d", count);

		position = movePosBefore;
		matRotation.identity();
		//rotation.y = -PI;

		if (easeProgress % 10 == 0) {
			size_t index = (size_t)easeProgress / 10;	
			eDataBarrelMove[index].Start(actTime[(INT32)BossAct::Spawn] / 10.0f);
			count++;

			if (index == 3) {
				int a = 0;
				a++;
			}
		}


		for (size_t i = 0; i < barrelMax; i++) {
		eDataBarrelMove[i].Update();
		Vector3 pos = Vector3::Lerp(movePosBeforeBarrel[i], movePosAfterBarrel[i], eDataBarrelMove[i].GetTimeRate());
		barrelObject[i].position = pos;
		}

	}
	else if (easeProgress < 80) {
		//最初の移動
		float t = (easeProgress - 40.0f) /40.0f;

		Vector3 pos = Vector3::Lerp(movePosBefore, movePosAfter, EaseOut(t));
		position = pos;
	}
	else {
		//親を戻してzをリセット
		for (size_t i = 0; i < barrelMax; i++) {
			barrelObject[i].parent = this;
			Vector3 pos{};
			barrelRadian[i] = 90.0f * i;
			pos.x = sinf((float)PI / 180.0f * barrelRadian[i]) * baseBarrelDistance;
			pos.y = cosf((float)PI / 180.0f * barrelRadian[i]) * baseBarrelDistance;
			barrelObject[i].position = pos;
		}

		//回転
		float t = (easeProgress - 80) / 20.0f;
		rotation.y = Lerp(0, PI, EaseOut(t));

	}



	//親と砲台オブジェクト更新
	Object3d::Update();
	for (size_t i = 0; i < barrelMax;i++) {



		barrelObject[i].Update();

		ImGui::Text("pos %f,%f,%f", barrelObject[i].position.x, barrelObject[i].position.y, barrelObject[i].position.z);
	}



	if (easeProgress >= 100.0f) {
		ChangeAct(BossAct::Move);
	}

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
	//射撃ごとの時間
	INT32 shotTimeOnce = (actTime[(INT32)BossAct::AttackShot]-30) / shotPosMax;
	//何度目の射撃か
	size_t currentPosIndex = shotPosMax - (INT32)nowActTime / shotTimeOnce;
	if (nowActTime % shotTimeOnce == 0) {
		if (currentPosIndex < 3) {
			//movePosAfter = shotPos[currentPosIndex];
			movePosBefore = shotPos[currentPosIndex];
			movePosAfter = shotPos[currentPosIndex+1];
			eDataMove.Start(20.0f);
		}
		else if(currentPosIndex == 3){
			movePosBefore = shotPos[currentPosIndex];
			movePosAfter = lastPosActMove;
			eDataMove.Start(30.0f);
		}


	}
	else if (nowActTime % shotTimeOnce == 22) {
		//砲台の数だけ行う
		for (size_t i = 0; i < barrelMax; i++) {
			
			Vector3 outPos = Matrix4::transform(bulletOutOffset, barrelObject[i].matWorld) + barrelObject[i].GetWorldPosition();
			float bulletSpd = 4.0f;
			Vector3 vel = targetPos - outPos;
			vel.normalize();
			vel *= bulletSpd;
			std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
			newBullet->Initialize(bulletModel.get(), outPos, vel);

			bullets.push_back(std::move(newBullet));
		}

	}

	eDataMove.Update();
	if (eDataMove.GetTimeRate() < 1.0f) {
		float t = EaseOut(eDataMove.GetTimeRate());
		position = Vector3::Lerp(movePosBefore, movePosAfter, t);
	}

	ImGui::Text("current index %d",currentPosIndex);
	ImGui::Text("before %f,%f,%f",movePosBefore.x,movePosBefore.y,movePosBefore.z);
	ImGui::Text("after %f,%f,%f", movePosAfter.x, movePosAfter.y, movePosAfter.z);


	Object3d::Update();

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
	//親(カメラのワールド情報)がセットされてないならreturn
	if (parent == nullptr) {
		return;
	}


	//ボスのスポーン地点はカメラの後ろ
	spawnPosOffsetCamera = Matrix4::transform({ 0,0,-50 }, parent->matWorld);

	//移動用イージングを開始
	movePosBefore = spawnPosOffsetCamera;
	movePosAfter = Matrix4::transform({ 0,0,240 }, parent->matWorld);
	eDataMove.Start((float)actTime[(INT32)BossAct::Spawn]);

	//砲台オブジェクトも親は一旦カメラobjにする(移動終わったら目玉objに戻す)
	for (size_t i = 0; i < barrelMax;i++) {
		barrelObject[i].parent = parent;
		//Zは目玉と同じ
		//x,yは移動後とつじつまを合わせる
		//オブジェクトから等間隔に離す
		Vector3 pos;
		barrelRadian[i] = 90.0f * i;
		pos.x = sinf((float)PI / 180.0f * barrelRadian[i]) * baseBarrelDistance;
		pos.y = cosf((float)PI / 180.0f * barrelRadian[i]) * baseBarrelDistance;
		pos.z = movePosBefore.z;
		movePosBeforeBarrel[i] = pos;
		pos.z = movePosAfter.z;
		movePosAfterBarrel[i] = pos;

		barrelObject[i].position = pos;
		//barrelObject[i].rotation.y = -PI;
		barrelObject[i].Update();
	}

	eDataBarrelMove[0].Start(actTime[(INT32)BossAct::Spawn] / 10.0f);

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
		shotPos[i].x = Random(0, randomPosAreaX);
		shotPos[i].y = Random(0, randomPosAreaY);

		shotPos[i].x = randomPosAreaX / 2;
		shotPos[i].y = randomPosAreaY /2;
	}
	//オフセット
	shotPos[1].y += randomPosAreaY;
	shotPos[2].x += randomPosAreaX;
	shotPos[3].x += randomPosAreaX;
	shotPos[3].y += randomPosAreaY;
	for (size_t i = 0; i < 4; i++) {
		sp[i].SetPos({ shotPos[i].x,shotPos[i].y });
	}

	//ｽｸﾘｰﾝ座標をワールドに
	float posZ = position.z;
	Matrix4 matViewPort;
	matViewPort.identity();
	matViewPort.m[0][0] = WindowsAPI::winW / 2.0f;
	matViewPort.m[1][1] = -(WindowsAPI::winH / 2.0f);
	matViewPort.m[3][0] = WindowsAPI::winW / 2.0f;
	matViewPort.m[3][1] = WindowsAPI::winH / 2.0f;

	Matrix4 matVBV = Object3d::camera->GetViewProjection() * matViewPort;

	for (size_t i = 0; i < shotPosMax; i++) {
		shotPos[i] = ConvertScreenToWorld({ shotPos[i].x,shotPos[i].y }, 240.0f, matVBV);
		shotPos[i].z = posZ;
	}
	

	//現在座標を移動用座標にセット
	movePosBefore = position;
	lastPosActMove = position;
	movePosAfter = shotPos.front();
	eDataMove.Start(20.0f);

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
