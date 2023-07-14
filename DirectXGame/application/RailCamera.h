#pragma once
#include"Camera.h"
#include"Object3d.h"
#include"SplineCurve.h"


class RailCamera
{

public://�����o�֐�

	~RailCamera();

	/// <summary>
	/// ������
	/// </summary>
	void Initialize(const Vector3& position,const Vector3& rotation);

	void Start();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	Object3d* GetObject3d()const { return world; }

	Camera* GetCamera()const { return camera; }

	//�J�����i�s�x
	float GetProgress() const { return spline.GetProgress(); }

private://�����o�ϐ�

	//���[���h�ϊ��f�[�^
	Object3d* world = nullptr;

	//�J����
	Camera* camera = nullptr;

	//�J���������p�X�v���C���Ȑ�
	SplineCurve spline;

};

