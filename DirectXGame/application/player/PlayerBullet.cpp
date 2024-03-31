//���@�̒e�N���X
//���@����������e

#include "PlayerBullet.h"
#include<cassert>
#include"Util.h"
#include"SphereCollider.h"
#include"CollisionAttribute.h"

void PlayerBullet::Initialize(Model* model_, const Vector3& position_, const Vector3& velocity_)
{
	assert(model_);

	//object3d�̏�����
	Object3d::Initialize();
	//���f���̃Z�b�g
	SetModel(model_);

	//�����̍��W���Z�b�g
	position = position_;
	velocity = velocity_;
	float bulletColSize = 1.0f;

	SetCollider(new SphereCollider({ 0,0,0 }, bulletColSize));
	collider->SetAttribute(COLLISION_ATTR_ALLIES);
}

void PlayerBullet::Update()
{
	//���Ԍo�߂Ŏ��S
	if (--deathTimer <= 0) {
		isAlive = false;
	}

	//�ړ�
	position += velocity;

	//Z����]
	rotation.z += (float)PI / 180.0f * 36;

	//object3d�̍X�V
	Object3d::Update();
}

void PlayerBullet::OnCollision([[maybe_unused]] const CollisionInfo& info)
{
	//�Փ˂̏d��������邽�ߓ��������疳�G�Ɂ@
	collider->SetAttribute(COLLISION_ATTR_INVINCIBLE);
}
