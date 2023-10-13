//�G�̒e�N���X
//�G�����˂���e�̏������s��

#include "EnemyBullet.h"
#include"SphereCollider.h"
#include"CollisionAttribute.h"
#include"Util.h"

void EnemyBullet::Initialize(Model* model_, const Vector3& position_, const Vector3& velocity_)
{
	assert(model_);

	//obj�̏�����
	Object3d::Initialize();
	//���f���Z�b�g
	SetModel(model_);

	//�����̍��W���Z�b�g
	position = position_;
	velocity = velocity_;

	SetCollider(new SphereCollider({ 0,0,0 }, 1.0f));
	collider->SetAttribute(COLLISION_ATTR_ENEMYS);
}

void EnemyBullet::Update()
{
	//���Ԍo�߂Ŏ��S
	if (--deathTimer <= 0) {
		isAlive = false;
	}

	//�ړ�
	position += velocity;

	//Z����]
//	rotation.z += (float)PI / 180.0f * 36;

	//object3d�̍X�V
	Object3d::Update();
}
