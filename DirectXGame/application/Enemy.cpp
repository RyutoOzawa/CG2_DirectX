//�G�N���X
//�Q�[���ɓo�ꂷ��G�P�̂̏������s��

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
	//object3d�̏�����
	Object3d::Initialize();
	SetModel(model);

	moveLine.SetPositions(points);

	SetCollider(new SphereCollider({ 0,0,0 }, 9.0f));
	collider->SetAttribute(COLLISION_ATTR_ENEMYS);
	scale = baseScale;
	shotInterval = shotCoolTime;

}

void Enemy::Update(const Vector3& playerWorldPos, const Matrix4& cameraMat)
{
	moveLine.Update();

	//�Ȑ��ɃJ�����s����|���ăJ������ɂ���
	Vector3 cameraPos = { cameraMat.m[3][0],cameraMat.m[3][1] ,cameraMat.m[3][2] };

	position = Matrix4::transform( moveLine.GetPosition(),cameraMat) + cameraPos;

	Attack(playerWorldPos);

	//�e�̍X�V
	//����ł�e������
	bullets.remove_if([](std::unique_ptr<EnemyBullet>& bullet) {
		if (!bullet->IsAlive()) {
			return true;
		}
		return false;
		});


	for (std::unique_ptr<EnemyBullet>& bullet : bullets) {
		bullet->Update();
	}

	//�X�P�[������
	ScaleControll();

	//�C�[�W���O���Ȃ��]
	easeAtkRot.Update();
	if (easeAtkRot.GetTimeRate() < 1.0f) {
		rotation.z =  Lerp(0.0f, 2 * PI, Out( easeAtkRot.GetTimeRate()));
	}
	else {
		rotation.z = 0.0f;
	}

	Object3d::Update();

	//�J�����̃r���[�s��Ɗ|���Z����z���}�C�i�X�Ȃ�E��
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

void Enemy::OnCollision([[maybe_unused]] const CollisionInfo& info)
{


	isAlive = false;

	//�p�[�e�B�N���ǉ�
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
	//�C���^�[�o����0�Ȃ甭��
	if (shotInterval == 0) {
		//�C���^�[�o�����Z�b�g
		shotInterval = shotCoolTime;
		//�G���玩�@�̃x�N�g���擾
		Vector3 vecEtoP = playerWorldPos - GetLocalPosition();
		vecEtoP.normalize();
		//�e�̑��x
		const float bulletSpdBase = 2.0f;
		vecEtoP *= bulletSpdBase;

		//�e�̐����Ə�����
		std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
		newBullet->Initialize(bulletModel, GetWorldPosition(), vecEtoP);

		//�o�^
		bullets.push_back(std::move(newBullet));

		//�X�P�[����3�{��
		scale *= 2.5f;

		//��]
		easeAtkRot.Start(30.0f);

	}
	else {
		shotInterval--;
	}
}

void Enemy::Death()
{
	isAlive = false;
}

void Enemy::ScaleControll()
{
	//�傫����1�𒴂��Ă����班����������
	//x,y,z�S�ăX�P�[�������l�̂��̂��Ɖ��肵�ăT�C�Y�̒������s��
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
