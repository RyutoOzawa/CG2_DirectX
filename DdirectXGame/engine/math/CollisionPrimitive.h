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

/// <summary>
/// �@���t���O�p�`�f�[�^(���v��肪�\��)
/// </summary>
class Triangle {
public:
	Vector3 p0;
	Vector3 p1;
	Vector3 p2;
	//�@��
	Vector3 normal;
};

/// <summary>
/// ���C(������)�f�[�^
/// </summary>
struct Ray {
	//�n�_���W
	Vector3 start{ 0,0,0 };
	//����
	Vector3 dir{ 1,0,0 };
};