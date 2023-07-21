#include "BossEnemy.h"

void BossEnemy::Initialize(Model* bodyModel, Model* barrelModel)
{
	//���f����null�`�F�b�N�ƃ����o�ւ̃Z�b�g
	assert(bodyModel);
	assert(barrelModel);
	this->bodyModel = bodyModel;
	this->BarrelModel = barrelModel;

	//�{�̃I�u�W�F�N�g(�p����)�̏�����
	Object3d::Initialize();
	SetModel(bodyModel);

	//TODO:�C��I�u�W�F�N�g�̏������A���f���Z�b�g

}

void BossEnemy::Update(const Vector3& playerPos)
{

	//�eact�ōX�V������
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

	//���f���̕`��
	Object3d::Draw();

	//TODO: �C��̕`��

}

void BossEnemy::Finalize()
{
}

void BossEnemy::UpdateSpawn()
{
}

void BossEnemy::UpdateMove()
{
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
