#include "Enemy.h"
#include"SphereCollider.h"
#include"Util.h"
#include"CollisionAttribute.h"

ParticleManager Enemy::particleManager{};

void Enemy::EnemyParticleUpdate()
{
	particleManager.Update();
}

void Enemy::EnemyInitialize(uint32_t texIndex)
{
	particleManager.Initialize(texIndex);
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

	SetCollider(new SphereCollider({2,2,2},1.0f));
	collider->SetAttribute(COLLISION_ATTR_ENEMYS);

}

void Enemy::Update()
{
	

	moveLine.Update();

	position = moveLine.GetPosition();

	Object3d::Update();

}

void Enemy::Draw()
{
	if (!isAlive) {
		return;
	}

	Object3d::Draw();

}

void Enemy::Spawn()
{
	moveLine.Start(600,true);

	isAlive = true;
}

void Enemy::OnCollision(const CollisionInfo& info)
{
	isAlive = false;

	//パーティクル追加
	for (int i = 0; i < 10; i++) {
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
