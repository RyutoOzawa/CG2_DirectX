//���̃R���C�_�[�N���X
//���̂̓����蔻������N���X
#include "SphereCollider.h"

void SphereCollider::Update()
{
	//���[���h�s�񂩂���W�𒊏o
	const Matrix4& matWorld = object3d->GetMatWorld();

	//���̃����o�ϐ����X�V
	Sphere::pos.x = matWorld.m[3][0];
	Sphere::pos.y = matWorld.m[3][1];
	Sphere::pos.z = matWorld.m[3][2];
	Sphere::pos + offset;
	Sphere::radius = radius;
}
