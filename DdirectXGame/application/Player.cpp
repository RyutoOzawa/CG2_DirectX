#include "Player.h"
#include"Input.h"
#include"SphereCollider.h"
#include"Util.h"

void Player::Initialize(Model* model, uint32_t reticleTexture)
{
	Object3d::Initialize();
	SetModel(model);

	position = { 0,0,50 };

	//コライダーの追加
	float radius = 1.0f;
	//半径分だけ浮いた座標を球の中心にする
	SetCollider(new SphereCollider(Vector3(1, 1, 1),radius));

	hitParticle.Initialize(0);

	reticleObj.Initialize();
	//モデルはオブジェクトと同じもの
	reticleObj.SetModel(Object3d::model);

	reticleSprite.Initialize(reticleTexture);

//	reticleSprite.SetPos({ 500.0f,500.0f });

	reticleSprite.SetAnchorPoint({ 0.5f,0.5f });

}

void Player::Update()
{
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
	ReticleUpdate();

}

void Player::Draw()
{
	//自分の描画
	Object3d::Draw();

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
}

void Player::OnCollision(const CollisionInfo& info)
{

	static int a = 0;
	a++;

	//パーティクルの速度
	Vector3 vel = { 0,0,0 };
	Vector3 acc = { Random(-10.0f,10.0f),Random(-10.0f,10.0f) ,Random(-10.0f,10.0f) };

	
	//パーティクル追加
	hitParticle.Add(15, info.inter, vel, acc, 3.0f, 0.0f);

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

void Player::ReticleUpdate()
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

	

	reticleSpd.x = inputHorizontal * reticleSpdBase;
	reticleSpd.y = inputVertical * reticleSpdBase;

	reticlePosScreen += reticleSpd;

	//座標をスプライトにセット
	reticleSprite.SetPos(reticlePosScreen);

	//ビュー、射影、ビューポートの行列を合成
	Matrix4 matVBVInverse = matViewProViewPort;
	matVBVInverse.Inverse();

	//スクリーン座標
	Vector3 posNear = {reticlePosScreen.x, reticlePosScreen.y, 0};
	Vector3 posFar = {reticlePosScreen.x, reticlePosScreen.y, 1};

	//ワールド座標系に変換
	posNear = Matrix4::transformDivW(posNear, matVBVInverse);
	posFar = Matrix4::transformDivW(posFar, matVBVInverse);

	//レイの方向
	Vector3 direction = posFar - posNear;
	direction.normalize();

	//カメラからレティクル(3D)への距離
	const float distanceReticle3D = 100.0f;
	reticleObj.position = posNear + direction * distanceReticle3D;


////自機からレティクルへの距離(スカラー)
//	const float distance = 50.0f;
//	//自機からレティクルへのオフセット
//	Vector3 offset = { 0,0,1.0f };
//	//自機の回転を反映
//	offset = Matrix4::transform(offset, matWorld);
//	//ベクトルの長さを整える
//	offset.normalize();
//	offset *= distance;
//	//座標設定
//	reticleObj.position = GetWorldPosition() + offset;
	reticleObj.Update();
//
//	////3dのレティクル座標から2Dのレティクル座標を計算
//	Vector3 reticlePos = reticleObj.GetWorldPosition();
//
//	//
//
//	////スクリーン座標変換
//	reticlePos = Matrix4::transformDivW(reticlePos, matViewProViewPort);
//
//	//座標設定
//	reticleSprite.SetPos( { reticlePos.x, reticlePos.y });

}

