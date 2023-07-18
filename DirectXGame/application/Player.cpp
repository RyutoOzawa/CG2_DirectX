#include "Player.h"
#include"Input.h"
#include"SphereCollider.h"
#include"Util.h"
#include"CollisionAttribute.h"
#include"Enemy.h"
#include"Collision.h"
#include"ImguiManager.h"

void Player::Initialize(Model* model, uint32_t reticleTexture, uint32_t healthTexture)
{
	Object3d::Initialize();
	SetModel(model);

	position = { 0,0,distanceCamera };

	//コライダーの追加
	float radius = 1.0f;
	//半径分だけ浮いた座標を球の中心にする
	SetCollider(new SphereCollider(Vector3(0, 0, 0),radius));
	collider->SetAttribute(COLLISION_ATTR_ALLIES);

	//命中パーティクル
	hitParticle.Initialize(0);

	//レティクルオブジェクト
	reticleObj.Initialize();
	//モデルはオブジェクトと同じもの
	reticleObj.SetModel(Object3d::model);

	//レティクルスプライト
	reticleSprite.Initialize(reticleTexture);
	reticleSprite.SetAnchorPoint({ 0.5f,0.5f });
	reticleSprite.SetColor({ 1,1,1,0.5f });

	//HPの初期化
	health = healthMax;
	healthWidthMax = 720;
	healthWidth = healthWidth;

	//HPスプライト
	healthSprite.Initialize(healthTexture);
	healthSprite.SetPos({ 32,32 });
	healthSprite.SetSize({ 720,32 });
	Vector3 healthColor = ColorCodeRGB(0x72b876);
	healthSprite.SetColor({ healthColor.x,healthColor.y,healthColor.z,1.0f });
	damageInterval = 0;

}

void Player::Update(std::list<std::unique_ptr<Enemy>>* enemys)
{
	ImGui::Begin("Player");


	//死んでる弾を消す
	bullets.remove_if([](std::unique_ptr<PlayerBullet>& bullet) {
		if (!bullet->IsAlive()) {
			return true;
		}
		return false;
		});

	

	//移動
	Move();

	//攻撃
	Attack();



	//弾の更新
	for (std::unique_ptr<PlayerBullet>& bullet : bullets) {
		bullet->Update();
	}

	//命中時パーティクル更新
	hitParticle.Update();

	//レティクルのオブジェクトデータ更新
	ReticleUpdate(enemys);

	//HPバーの更新
	HealthUpdate();

	ImGui::End();

}

void Player::Draw()
{
	//自分の描画

	//ダメージのクールタイムによって点滅
	if (damageInterval % 4 < 1) {
		Object3d::Draw();
	}

	//弾の描画
	for (std::unique_ptr<PlayerBullet>& bullet : bullets) {
		bullet->Draw();
	}

	reticleObj.Draw();
}

void Player::DrawParticle()
{
	hitParticle.Draw();
	
}

void Player::DrawUI()
{
	reticleSprite.Draw();
	healthSprite.Draw();
}

void Player::OnCollision(const CollisionInfo& info)
{
	//ダメージのクールタイムが終わってないなら何もしない
	if (damageInterval > 0) {
		return;
	}


	static int a = 0;
	a++;

	//パーティクルの速度
	Vector3 vel = { 0,0,0 };
	Vector3 acc = { Random(-10.0f,10.0f),Random(-10.0f,10.0f) ,Random(-10.0f,10.0f) };

	
	//パーティクル追加
	hitParticle.Add(15, info.inter, vel, acc, 3.0f, 0.0f);

	//ダメージを受ける
	Damage();

}


void Player::Move()
{
	//入力
	INT32 inputHorizontal = 0;
	INT32 inputVertical = 0;

	if (Input::GetInstance()->IsKeyPress(DIK_A) || Input::GetInstance()->IsDownLStickLeft()) {
		inputHorizontal = -1;
	}
	else if (Input::GetInstance()->IsKeyPress(DIK_D) || Input::GetInstance()->IsDownLStickRight()) {
		inputHorizontal = 1;
	}

	if (Input::GetInstance()->IsKeyPress(DIK_W) || Input::GetInstance()->IsDownLStickUp()) {
		inputVertical = 1;
	}
	else if (Input::GetInstance()->IsKeyPress(DIK_S) || Input::GetInstance()->IsDownLStickDown()) {
		inputVertical = -1;
	}

	//座標に加算する速度
	Vector3 spd{};
	float baseSpd = 0.5f;


	spd.x += baseSpd * inputHorizontal;
	spd.y += baseSpd * inputVertical;
	



	//現在座標を取得
	pos = position;
	//加算して代入
	pos += spd;
	position = pos;

	const float bodyTurnBase = (float)PI / 180.0f;

	//旋回の挙動
	rotation.z -= bodyTurnBase * inputHorizontal;
	rotation.x -= bodyTurnBase * inputVertical;

	if (inputHorizontal == 0) {
		if (rotation.z > 0) {
			rotation.z -= bodyTurnBase * 3.0f;
		}
		else if (rotation.z < 0) {
			rotation.z += bodyTurnBase * 3.0f;
		}
		else {
			rotation.z = 0.0f;
		}
	}

	if (inputVertical == 0) {
		if (rotation.x > 0) {
			rotation.x -= bodyTurnBase * 3.0f;
		}
		else if (rotation.x < 0) {
			rotation.x += bodyTurnBase * 3.0f;
		}
		else {
			rotation.x = 0.0f;
		}
	}

	//obj3dの更新
	Object3d::Update();

}

void Player::Attack()
{
	//スペースキーで弾発射
	if (shotInterval == 0) {

		if (Input::GetInstance()->IsKeyPress(DIK_SPACE) || Input::GetInstance()->IsPadPress(XINPUT_GAMEPAD_RIGHT_SHOULDER)) {
			
			shotInterval = shotCooltime;

			//弾の速度
			const float bulletSpdBase = 8.0f;
			Vector3 velocity(0, 0, bulletSpdBase);

			velocity = reticleObj.GetWorldPosition() - Object3d::GetWorldPosition();
			velocity.normalize();
			velocity *= bulletSpdBase;

			//弾の生成と初期化
			std::unique_ptr< PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
			newBullet->Initialize(bulletModel, GetWorldPosition(), velocity);

			//弾の登録
			bullets.push_back(std::move(newBullet));
		}
	}
	else {
		shotInterval--;
	}


}

void Player::ReticleUpdate(std::list<std::unique_ptr<Enemy>>* enemys)
{
	reticleObj.SetModel(bulletModel);

	//ビューポート行列
	Matrix4 matViewPort;
	matViewPort.identity();
	matViewPort.m[0][0] = WindowsAPI::winW / 2.0f;
	matViewPort.m[1][1] = -(WindowsAPI::winH / 2.0f);
	matViewPort.m[3][0] = WindowsAPI::winW / 2.0f;
	matViewPort.m[3][1] = WindowsAPI::winH / 2.0f;

	//カメラ行列との合成
	Matrix4 matViewProViewPort = Object3d::camera->GetViewProjection() * matViewPort;


	//画面上のレティクル座標を動かす
	Vector2 reticleSpd = { 0,0 };
	float reticleSpdBase = 8.0f;

	//入力
	INT32 inputHorizontal = 0;
	INT32 inputVertical = 0;

	if (Input::GetInstance()->IsKeyPress(DIK_LEFT) || Input::GetInstance()->IsDownRStickLeft()) {
		inputHorizontal = -1;
	}
	else if (Input::GetInstance()->IsKeyPress(DIK_RIGHT) || Input::GetInstance()->IsDownRStickRight()) {
		inputHorizontal = 1;
	}

	if (Input::GetInstance()->IsKeyPress(DIK_UP) || Input::GetInstance()->IsDownRStickUp()) {
		inputVertical = -1;
	}
	else if (Input::GetInstance()->IsKeyPress(DIK_DOWN) || Input::GetInstance()->IsDownRStickDown()) {
		inputVertical = 1;
	}


	
	//入力値×レティクルスピードで移動
	reticleSpd.x = inputHorizontal * reticleSpdBase;
	reticleSpd.y = inputVertical * reticleSpdBase;
	reticlePosScreen += reticleSpd;

	//レティクル座標の移動制限
	Vector2 reticlePosMin = { reticleRadius,reticleRadius };
	Vector2 reticlePosMax = { WindowsAPI::winW - reticleRadius,WindowsAPI::winH - reticleRadius };
	if (reticlePosScreen.x > reticlePosMax.x) {
		reticlePosScreen.x = reticlePosMax.x;
	}
	else if (reticlePosScreen.x < reticlePosMin.x) {
		reticlePosScreen.x = reticlePosMin.x;
	}

	if (reticlePosScreen.y > reticlePosMax.y) {
		reticlePosScreen.y = reticlePosMax.y;
	}
	else if (reticlePosScreen.y < reticlePosMin.y) {
		reticlePosScreen.y = reticlePosMin.y;
	}

	//座標をスプライトにセット
	reticleSprite.SetPos(reticlePosScreen);

	

	

	//カメラからレティクル(3D)への距離
	float distanceReticle3D = distanceCamera + 100.0f;

	//スクリーン座標変換した自機の座標
	Vector3 posPlayerScreen = Matrix4::transformDivW(GetWorldPosition(), matViewProViewPort);

	//ImGui::Text("screen Z player %f", posPlayerScreen.z);

	//敵の座標をスクリーン座標に変換
	std::list<std::unique_ptr<Enemy>>::iterator itE;
	itE = enemys->begin();
	for (; itE != enemys->end(); itE++) {
		//敵のワールド座標を取得
		Vector3 posEnemyWorld = itE->get()->GetWorldPosition();

		//ビュー、プロジェクション、ビューポート行列を掛けてW除算
		posEnemyWorld = Matrix4::transformDivW(posEnemyWorld, matViewProViewPort);
		Vector2 posEnemyScreen = { posEnemyWorld.x,posEnemyWorld.y };
		
		//ImGui::Text("screen Z eyemy %f", posEnemyWorld.z);

		//自機より後ろにいる奴は対象外;
		if (posEnemyWorld.z < posPlayerScreen.z) {
			continue;
		}

		Circle reticleC{ reticlePosScreen,reticleRadius };
		Circle enemyC{ posEnemyScreen,1.0f };

		//カメラからレティクルの距離を(敵のワールド×ビュー)のzにする
		Vector3 posEnemyView = itE->get()->GetLocalPosition() * camera->GetView();
		//ImGui::Text("enemyView.z %f", posEnemyView.z);

		//レティクルが敵に当たっているなら
		if (Collision::ColCircleToCircle(reticleC, enemyC)) {
			


			//カメラから敵の距離が自機との距離より小さいなら狙わない
			if (posEnemyView.z < distanceCamera) {
				continue;
			}


			distanceReticle3D = posEnemyView.z;


			//レティクルが動いているならロックオン
			if (inputHorizontal != 0 || inputVertical != 0) {
			//	reticlePosScreen = posEnemyScreen;
			}
			
		}

	}

//	ImGui::Text("distance retilce3d %f", distanceReticle3D);

	//ビュー、射影、ビューポートの行列を合成
	Matrix4 matVBVInverse = matViewProViewPort;
	matVBVInverse.Inverse();

	//スクリーン座標
	Vector3 posNear = { reticlePosScreen.x, reticlePosScreen.y, 0 };
	Vector3 posFar = { reticlePosScreen.x, reticlePosScreen.y, 1 };

	//ワールド座標系に変換
	posNear = Matrix4::transformDivW(posNear, matVBVInverse);
	posFar = Matrix4::transformDivW(posFar, matVBVInverse);

	//レイの方向
	Vector3 direction = posFar - posNear;
	direction.normalize();

	reticleObj.position = posNear + direction * distanceReticle3D;
	reticleObj.Update();

}

void Player::HealthUpdate()
{
	//ダメージのクールタイムを減らす
	if (damageInterval > 0) {
		damageInterval--;
	}


	//ボタンで体力減らしたり増やしたり
	if (ImGui::Button("damage!!")) {
		health--;
	}

	if (ImGui::Button("healing")) {
		health++;
	}

	ImGui::Text("interval %d", damageInterval);

	//横幅最大値をHP最大値で分割して1HP当たりの横幅を計算
	float widthOnce = (float)healthWidthMax / (float)healthMax;
	//現在のHPに掛け算して横幅を割り出す
	float nowWidth = widthOnce * health;
	//HPバーサイズ取得
	Vector2 hp = healthSprite.GetSize();
	hp.x = nowWidth;
	healthSprite.SetSize(hp);


}

void Player::Damage()
{
	//hpを減らす
	health--;
	//次食らうクールタイムを設定
	damageInterval = damageCooltime;
}

