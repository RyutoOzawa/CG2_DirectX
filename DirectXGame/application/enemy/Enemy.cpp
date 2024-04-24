//敵クラス
//ゲームに登場する敵単体の処理を行う

#include "Enemy.h"
#include"SphereCollider.h"
#include"Util.h"
#include"CollisionAttribute.h"
#include"ImguiManager.h"
using namespace Utility;
using namespace Easing;

ParticleManager Enemy::particleManager{};
const INT32 Enemy::shotCoolTime = 120;
Model* Enemy::model = nullptr;
Model* Enemy::bulletModel = nullptr;


void Enemy::EnemyParticleUpdate()
{
	particleManager.Update();
}

void Enemy::EnemyInitialize(TextureData* texData,Model* enemyModel, Model* bulletModel_)
{
	particleManager.Initialize(texData);
	Enemy::model = enemyModel;
	Enemy::bulletModel = bulletModel_;
}

void Enemy::DrawParticle()
{
	particleManager.Draw();
}

void Enemy::Initialize(std::vector<Vector3>& points)
{
	//object3dの初期化
	Object3d::Initialize();
	SetModel(model);

	moveLine.SetPositions(points);

	

	SetCollider(new SphereCollider({ 0,0,0 }, enemyColSize));
	collider->SetAttribute(COLLISION_ATTR_INVINCIBLE);
	scale = baseScale;
	shotInterval = shotCoolTime;

}

void Enemy::Initialize(const Vector3& spawnPos, uint16_t leaveTime_)
{
	//Obj3Dの初期化
	Object3d::Initialize();
	SetModel(model);

	//留まる座標に応じて移動前の座標を設定
	spawnPosBefore = spawnPos;
	if (spawnPos.x > 0.0f) {
		spawnPosBefore.x = 300.0f;
	}
	else {
		spawnPosBefore.x = -300.0f;
	}

	if (spawnPos.y > 0.0f) {
		spawnPosBefore.y = 150.0f;
	}
	else {
		spawnPosBefore.y = -150.0f;
	}

	easeMove.Start(60.0f);

	stayPosition = spawnPos;
	leaveTime = leaveTime_;

	SetCollider(new SphereCollider({ 0,0,0 }, enemyColSize));
	collider->SetAttribute(COLLISION_ATTR_INVINCIBLE);
	scale = baseScale;
	shotInterval = shotCoolTime;
}

void Enemy::Update(const Vector3& playerWorldPos, const Matrix4& cameraMat)
{

	//曲線にカメラ行列を掛けてカメラ基準にする
	Vector3 cameraPos = { cameraMat.m[3][0],cameraMat.m[3][1] ,cameraMat.m[3][2] };

	//移動
	Move(cameraMat, cameraPos);

	//攻撃
	Attack(playerWorldPos);

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

	//スケール制御
	ScaleControll();

	//イージング中なら回転
	easeAtkRot.Update();
	if (easeAtkRot.GetTimeRate() < 1.0f) {
		rotation.z =  Lerp(0.0f, 2 * PI, Out( easeAtkRot.GetTimeRate()));
	}
	else {
		rotation.z = 0.0f;
	}

	Object3d::Update();

	//カメラのビュー行列と掛け算してzがマイナスなら殺す
	Matrix4 matEnemyView = matWorld * camera->GetView();
	if (matEnemyView.m[3][2] < 25.0f) {
		isAlive = false;
	}

}

void Enemy::Draw()
{
	for (std::unique_ptr<EnemyBullet>& bullet : bullets) {
		bullet->Draw();
	}

	if (!isAlive) {
		return;
	}

	Object3d::Draw();

}

void Enemy::Spawn()
{
	if (moveLine.GetCPosCount() > 0) {
		moveLine.Start(600, true);
	}

	isAlive = true;
}

void Enemy::OnCollision([[maybe_unused]] const CollisionInfo& info)
{


	isAlive = false;

	//パーティクル追加
	for (int i = 0; i < 20; i++) {
		Vector3 vel;
		const float baseVel = 2.0f;
		vel.x = Random(-baseVel, baseVel);
		vel.y = Random(-baseVel, baseVel);
		vel.z = Random(-baseVel, baseVel);
		Vector3 acc;
		const float baseAcc = 0.25f;
		acc.x = Random(-baseAcc, baseAcc);
		acc.y = Random(-baseAcc, baseAcc);
		acc.z = Random(-baseAcc, baseAcc);

		particleManager.Add(30, GetWorldPosition(), vel, acc, 10.0f, 0.0f);
	}

}

void Enemy::Attack(const Vector3& playerWorldPos)
{
	//インターバルが0なら発射
	if (shotInterval == 0) {
		//インターバルリセット
		shotInterval = shotCoolTime;
		//敵から自機のベクトル取得
		Vector3 vecEtoP = playerWorldPos - GetLocalPosition();
		vecEtoP.normalize();
		//弾の速度
		const float bulletSpdBase = 2.0f;
		vecEtoP *= bulletSpdBase;

		//弾の生成と初期化
		std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
		newBullet->Initialize(bulletModel, GetWorldPosition(), vecEtoP);

		//登録
		bullets.push_back(std::move(newBullet));

		//でかくする
		float atkScaleSize = 2.5f;

		scale *= atkScaleSize;

		//回転
		easeAtkRot.Start(30.0f);

	}
	else {
		shotInterval--;
		if (shotInterval < 110) {
			collider->SetAttribute(COLLISION_ATTR_ENEMYS);
		}
	}
}

void Enemy::Death()
{
	isAlive = false;
}

void Enemy::Move(const Matrix4& camMat, const Vector3& camPos)
{
	//移動用曲線が設定されていれば曲線の更新
	if (moveLine.GetCPosCount() > 0) {
		moveLine.Update();

		position = Matrix4::transform(moveLine.GetPosition(), camMat) + camPos;
	}
	else{//設定されていなければ設定された退避時間を減らす
		easeMove.Update();

		Vector3 currentPos = Vector3::Lerp(spawnPosBefore, stayPosition, easeMove.GetTimeRate());

		//currentPos = stayPosition;

		ImGui::Text("current %f,%f,%f", currentPos.x, currentPos.y, currentPos.z);

		position = Matrix4::transform(currentPos,camMat) + camPos;

		if (leaveTime > 0 && easeMove.GetTimeRate() >= 1.0f) {
			leaveTime--;
		}
		else if (leaveTime == 0) {	//退避時間が0になったら退避行動に移行
			Leave();
		}
	}

}

void Enemy::Leave()
{
	//退避用ベクトルの算出
	//ビューポート行列
	Matrix4 viewPort;
	viewPort.identity();
	viewPort.m[0][0] = WindowsAPI::winW / 2.0f;
	viewPort.m[1][1] = -(WindowsAPI::winH / 2.0f);
	viewPort.m[3][0] = WindowsAPI::winW / 2.0f;
	viewPort.m[3][1] = WindowsAPI::winH / 2.0f;

	//カメラ行列と合わせる
	Matrix4 matViewProViewPort = Object3d::camera->GetViewProjection() * viewPort;
	//スクリーン座標変換
	Vector3 posScreen = Matrix4::transformDivW(GetWorldPosition(), matViewProViewPort);
	//画面上のどこにいるかで退避ベクトルを計算
	if (posScreen.x >= WindowsAPI::winW / 2.0f) {
		leaveSpd.x = 1.0f;
	}
	else {
		leaveSpd.x = -1.0f;
	}

	if (posScreen.y > WindowsAPI::winH / 2.0f) {
		leaveSpd.y = -1.0f;
	}
	else {
		leaveSpd.y = 1.0f;
	}
	//Z方向には移動しない
	leaveSpd.z = 0.0f;

	//正規化して基本速度に直す
	leaveSpd.normalize();
	leaveSpd *= leaveSpdBase;

	//座標に加算
	stayPosition += leaveSpd;

}

void Enemy::ScaleControll()
{
	//大きさが1を超えていたら少しずつ小さく
	//x,y,z全てスケールが同値のものだと仮定してサイズの調整を行う
	if (scale.x > baseScale.x) {
		scale.x -= 0.5f;
		scale.y -= 0.5f;
		scale.z -= 0.5f;
	}
	else if (scale.x < baseScale.x) {
		scale = baseScale;
	}

}

void Enemy::Spin()
{
}
