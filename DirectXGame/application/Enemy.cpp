#include "Enemy.h"
#include"SphereCollider.h"
#include"Util.h"
#include"CollisionAttribute.h"

ParticleManager Enemy::particleManager{};
const INT32 Enemy::shotCoolTime = 120;
Model* Enemy::model = nullptr;
Model* Enemy::bulletModel = nullptr;


void Enemy::EnemyParticleUpdate()
{
	particleManager.Update();
}

void Enemy::EnemyInitialize(TextureData* texData,Model* enemyModel, Model* bulletModel)
{
	particleManager.Initialize(texData);
	Enemy::model = enemyModel;
	Enemy::bulletModel = bulletModel;
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

	SetCollider(new SphereCollider({ 0,0,0 }, 9.0f));
	collider->SetAttribute(COLLISION_ATTR_ENEMYS);

	scale = { 3,3,3 };

}

void Enemy::Update(const Vector3& playerWorldPos, const Matrix4& cameraMat)
{


	moveLine.Update();

	//曲線にカメラ行列を掛けてカメラ基準にする
	Vector3 cameraPos = { cameraMat.m[3][0],cameraMat.m[3][1] ,cameraMat.m[3][2] };

	position = Matrix4::transform( moveLine.GetPosition(),cameraMat) + cameraPos;

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
	moveLine.Start(600, true);

	isAlive = true;
}

void Enemy::OnCollision(const CollisionInfo& info)
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

		//スケールを3倍に
		scale = { 3,3,3 };

	}
	else {
		shotInterval--;
	}
}

void Enemy::ScaleControll()
{
	//大きさが1を超えていたら少しずつ小さく
	//x,y,z全てスケールが同値のものだと仮定してサイズの調整を行う
	if (scale.x > 1.0f) {
		scale.x -= 0.5f;
		scale.y -= 0.5f;
		scale.z -= 0.5f;
	}
	else if (scale.x < 1.0f) {
		scale.x = 1.0f;
		scale.y = 1.0f;
		scale.z = 1.0f;
	}

}

void Enemy::Spin()
{
}
