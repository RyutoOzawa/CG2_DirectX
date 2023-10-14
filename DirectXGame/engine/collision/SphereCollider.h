#pragma once
//���̃R���C�_�[�N���X
//���̂̓����蔻������N���X

#include"BaseCollider.h"
#include"CollisionPrimitive.h"

#include"Vector3.h"

class SphereCollider : public BaseCollider, public Sphere
{
public:
	SphereCollider(Vector3 offset_ = {0,0,0},float radius_ = 0):
	offset(offset_),radius(radius_) {
		//���`����Z�b�g
		shapeType = COLLISIONSHAPE_SPHERE;
	}

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	inline void SetRadius(float radius_) { radius = radius_; }

private:
	//�I�u�W�F�N�g���S����̃I�t�Z�b�g
	Vector3 offset;
	//���a
	float radius;


};

