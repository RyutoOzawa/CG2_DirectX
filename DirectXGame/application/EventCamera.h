#pragma once
//�C�x���g�J�������[�N�p�N���X
//�e�C�x���g�ł̃J�������[�N���s���₷���悤�ȋ@�\���J�����N���X

#include"Camera.h"
#include"EasingData.h"
#include"BezierCurve.h"

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
	void MoveEye(const Vector3& eye, uint16_t time, Easing::InterType eyeInter = Easing::InterType::Lerp,
		bool followTarget = true,Easing::InterType targetInter = Easing::InterType::Lerp);

	/// <summary>
	/// �x�W�G�Ȑ��ɂ��J�����ړ�
	/// </summary>
	/// <param name="curve">�x�W�G�Ȑ�</param>
	/// <param name="time">�ړ�����</param>
	/// <param name="eyeInter">���_�̕�Ԏ��</param>
	/// <param name="followTarget">�����_���Ǐ]���邩�H</param>
	/// <param name="targetInter">�����_�̕�Ԃ̎��</param>
	/*void MoveEye(const BezierCurve& curve, uint16_t time, Easing::InterType eyeInter = Easing::InterType::Lerp,
		bool followTarget = true, Easing::InterType targetInter = Easing::InterType::Lerp);*/
	/// <summary>
	/// �J���������_�ړ�
	/// </summary>
	/// <param name="target">�ړ���̒����_</param>
	/// <param name="time">�ړ�����</param>
	/// <param name="targetInter">�����_�̕�Ԏ��</param>
	/// <param name="followEye">���_���Ǐ]���邩�ǂ���</param>
	/// <param name="eyeInter">���_��</param>
	void MoveTarget(const Vector3& target, uint16_t time, Easing::InterType targetInter = Easing::InterType::Lerp, 
		bool followEye = false, Easing::InterType eyeInter = Easing::InterType::Lerp);

	//getter
	Camera* GetCamera()const { return camera.get(); }

	//setter
	void SetEye(const Vector3& eye_) { camera->eye = eye_; }
	void SetTarget(const Vector3& target_) { camera->target = target_; }

private:
	std::unique_ptr<Camera> camera;

	Easing::EasingData eMoveEye;
	Vector3 eyeBefore{ 0,0,0 };
	Vector3 eyeAfter{ 0,0,0 };
	Easing::InterType eyeInterType = Easing::InterType::Lerp;

	Easing::EasingData eMoveTarget;
	Vector3 targetBefore{ 0,0,0 };
	Vector3 targetAfter{ 0,0,0 };
	Easing::InterType targetInterType = Easing::InterType::Lerp;

	BezierCurve eyeBezire;
	bool useBezire = false;

};

