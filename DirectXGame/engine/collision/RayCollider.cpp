#include "RayCollider.h"

void RayCollider::Update()
{

	//���[���h�s�񂩂���W�𒊏o
	const Matrix4& matWorld = object3d->GetMatWorld();

	//���C�̃����o�X�V
	Ray::start.x = matWorld.m[3][0];
	Ray::start.y = matWorld.m[3][1];
	Ray::start.z = matWorld.m[3][2];
	Ray::dir = this->dir;


}
