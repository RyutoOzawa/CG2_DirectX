#include "PlayerBullet.h"
#include<cassert>

void PlayerBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity)
{
	assert(model);

	//object3d�̏�����
	Object3d::Initialize();
	//���f���̃Z�b�g
	SetModel(model);

	//�����̍��W���Z�b�g
	this->position = position;
	this->velocity = velocity;
}

void PlayerBullet::Update()
{
	//�ړ�
	position += velocity;


	//object3d�̍X�V
	Object3d::Update();
}
