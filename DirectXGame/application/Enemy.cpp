#include "Enemy.h"
#include"SphereCollider.h"
#include"Util.h"
#include"CollisionAttribute.h"

ParticleManager Enemy::particleManager{};
const INT32 Enemy::shotCoolTime = 120;
Model* Enemy::bulletModel = nullptr;


void Enemy::EnemyParticleUpdate()
{
	particleManager.Update();
}

void Enemy::EnemyInitialize(uint32_t texIndex, Model* bulletModel)
{
	particleManager.Initialize(texIndex);
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

	moveLine.SetPositions(points);

	SetCollider(new SphereCollider({ 2,2,2 }, 3.0f));
	collider->SetAttribute(COLLISION_ATTR_ENEMYS);

}

void Enemy::Update(const Vector3& playerWorldPos)
{


	moveLine.Update();

	position = moveLine.GetPosition();

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

		particleManager.Add(30, GetWorldPosition(), vel, acc, 1.0f, 0.0f);
	}

}

void Enemy::Attack(const Vector3& playerWorldPos)
{
	//インターバルが0なら発射
	if (shotInterval == 0) {
		//インターバルリセット
		shotInterval = shotCoolTime;
		//敵から自機のベクトル取得
		Vector3 vecEtoP = playerWorldPos - GetWorldPosition();
		vecEtoP.normalize();
		//弾の速度
		const float bulletSpdBase = 2.0f;
		vecEtoP *= bulletSpdBase;

		//弾の生成と初期化
		std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
		newBullet->Initialize(bulletModel, GetWorldPosition(), vecEtoP);

		//登録
		bullets.push_back(std::move(newBullet));
	}
	else {
		shotInterval--;
	}
}
