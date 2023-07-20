#include "EnemyBullet.h"
#include"SphereCollider.h"
#include"CollisionAttribute.h"
#include"Util.h"

void EnemyBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity)
{
	assert(model);

	//obj�̏�����
	Object3d::Initialize();
	//���f���Z�b�g
	SetModel(model);

	//�����̍��W���Z�b�g
	this->position = position;
	this->velocity = velocity;

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
