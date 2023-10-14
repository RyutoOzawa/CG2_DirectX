#pragma once
//�Փˏ��
//�Փ˂�������̏����i�[���邽�߂̃N���X

#include"Vector3.h"

class Object3d;
class BaseCollider;

/// <summary>
/// �Փˏ��
/// </summary>
class CollisionInfo
{
public:
	CollisionInfo(Object3d* object_, BaseCollider* collider_, const Vector3& inter_) {
		object = object_;
		collider = collider_;
		inter = inter_;
	}

	//�Փˑ���̃I�u�W�F�N�g
	Object3d* object = nullptr;
	//�Փˑ���̃R���C�_�[
	BaseCollider* collider = nullptr;
	//�Փ˓_
	Vector3 inter;
};

