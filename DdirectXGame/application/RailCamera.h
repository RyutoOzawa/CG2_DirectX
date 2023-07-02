#pragma once
#include"Camera.h"
#include"Object3d.h"

class RailCamera
{

public://�����o�֐�

	~RailCamera();

	/// <summary>
	/// ������
	/// </summary>
	void Initialize(const Vector3& position,const Vector3& rotation);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	Object3d* GetObject3d()const { return world; }

	Camera* GetCamera()const { return camera; }

private://�����o�ϐ�

	//���[���h�ϊ��f�[�^
	Object3d* world = nullptr;

	//�J����
	Camera* camera = nullptr;

};

