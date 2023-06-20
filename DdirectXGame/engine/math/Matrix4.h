#pragma once

#include"Vector3.h"
class Matrix4
{
public:

	float m[4][4];


	//�P�ʍs������߂�
	Matrix4 identity();

	//�g��k���s��̐ݒ�
	Matrix4 scale(const Vector3& s);

	//��]�s��̐ݒ�
	Matrix4 rotateX(float angle);
	Matrix4 rotateY(float angle);
	Matrix4 rotateZ(float angle);

	//���s�ړ��s��쐬
	Matrix4 translate(const Vector3& t);

	//���W�ϊ��i�x�N�g���ƍs��̊|���Z�j
	Vector3 transform(const Vector3& v, const Matrix4& m);

	//�t�s��
	Matrix4 Inverse();

	Matrix4 CreateViewMat(const Vector3& eye, const Vector3& target, const Vector3& up );

	Matrix4 CreatePerspectiveProjection(float fovY,float aspectRatio,float nearZ,float farZ);

	Matrix4 CreateParallelProjection(float windowW,float windowH);

	//���Z�q�I�[�o�[���[�h
	Matrix4& operator*=(const Matrix4& mat);

};

//2�����Z�q�I�[�o�[���[�h
const Matrix4 operator*(const Matrix4& m1, const Matrix4& m2);
const Vector3 operator*(const Vector3& v, const Matrix4& m);