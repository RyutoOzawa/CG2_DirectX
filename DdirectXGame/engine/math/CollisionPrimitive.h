#pragma once
#include"Vector3.h"

/// <summary>
/// �����蔻��v���~�e�B�u
/// </summary>

/// <summary>
/// ���̃f�[�^�^
/// </summary>
struct Sphere {
	//���W
	Vector3 pos = {0,0,0};
	//���a
	float radius = 1.0f;
};

/// <summary>
/// ���ʂ̃f�[�^�^
/// </summary>
struct Plane {
	//�@���x�N�g��
	Vector3 normal = { 0,1,0 };
	//���_(0,0)����̋���
	float distance = 0.0f;
};



