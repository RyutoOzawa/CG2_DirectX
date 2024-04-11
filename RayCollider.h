#pragma once
//���C(������)�R���C�_�[�N���X
//���C�̓����蔻��p�N���X

#include"BaseCollider.h"
#include"CollisionPrimitive.h"

#include"Vector3.h"

class RayCollider : public BaseCollider,public Ray
{
public:
	RayCollider(Vector3 offset_ = { 0,0,0 }) : offset(offset_) {
		//�`����Z�b�g
		shapeType = COLLISIONSHAPE_RAY;
	}


	//�X�V
	void Update();

	

private:

	//���S����̃I�t�Z�b�g
	Vector3 offset;


};

