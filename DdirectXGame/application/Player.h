#pragma once
#include"Object3d.h"
#include"Model.h"
#include"Vector3.h"
#include"SplineCurve.h"

//3D�I�u�W�F�N�g�Ȃ̂�obj3d�N���X���p��
class Player :public Object3d
{
public:	//�����o�֐�

	//������
	void Initialize();

	//�X�V
	void Update();

//	void Draw();

private://�����o�ϐ�

	Vector3 pos;

	SplineCurve splineTest;

private://���������p�����o�֐�

	void Move();

};

