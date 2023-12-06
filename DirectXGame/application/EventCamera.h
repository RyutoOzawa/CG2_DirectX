#pragma once
//�C�x���g�J�������[�N�p�N���X
//�e�C�x���g�ł̃J�������[�N���s���₷���悤�ȋ@�\���J�����N���X

#include"Camera.h"
#include"EasingData.h"

class EventCamera
{
public:

	//������
	void Initialize(const Vector3& eye, const Vector3& target, const Vector3& up);

	//�X�V
	void Update();

	/// <summary>
	/// �J�����ړ�
	/// </summary>
	/// <param name="eye">�ړ���̎��_</param>
	/// <param name="time">�ړ�����</param>
	/// <param name="eyeInter">���_�̕�Ԏ��</param>
	/// <param name="followTarget">�����_���Ǐ]���邩�ǂ���</param>
	/// <param name="targetInter">�����_�̕�Ԃ̎��</param>
	void MoveEye(const Vector3& eye, uint16_t time,InterType eyeInter = InterType::Lerp, bool followTarget = true,InterType targetInter = InterType::Lerp);

	//getter
	Camera* GetCamera()const { return camera.get(); }

	//setter
	void SetEye(const Vector3& eye_) { camera->eye = eye_; }
	void SetTarget(const Vector3& target_) { camera->target = target_; }

private:
	std::unique_ptr<Camera> camera;

	EasingData eMoveEye;
	Vector3 eyeBefore{ 0,0,0 };
	Vector3 eyeAfter{ 0,0,0 };
	InterType eyeInterType = InterType::Lerp;

	EasingData eMoveTarget;
	Vector3 targetBefore{ 0,0,0 };
	Vector3 targetAfter{ 0,0,0 };
	InterType targetInterType = InterType::Lerp;


};

