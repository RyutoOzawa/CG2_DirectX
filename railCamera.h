#pragma once
#include"WorldTransform.h"
#include"ViewProjection.h"

#include "Input.h"
/// <summary>
/// ���L����
/// </summary>
class RailCamera {

public:
	/// <summary>
	/// ������
	/// </summary>
	void Initialize(Vector3 translation, Vector3 rot,const WorldTransform* worldTransform);
	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	//�V�F�C�N�֐�
	Vector3 Shake();

	void PlayerOnCollision();

	void Rset();

	ViewProjection& GetViewProjection() { return viewProjection_; };

	WorldTransform* GetWorldTransform() { return &worldTransform_; };
private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//�r���[�v���W�F�N�V����
	ViewProjection viewProjection_;

	Input* input_ = nullptr;

	float PI = 3.1415927f;

	const int maxShakeCount=5;
	int shakeCount= maxShakeCount;

	bool shakeFrg = false;
};