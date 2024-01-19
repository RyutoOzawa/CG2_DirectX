#include "EventCamera.h"
#include"Util.h"
using namespace Utility;
using namespace Easing;

void EventCamera::Initialize(const Vector3& eye, const Vector3& target, const Vector3& up)
{
	

	//�J�����̏�����
	camera = std::make_unique<Camera>();
	camera->Initialize(eye, target, up);

}

void EventCamera::Update()
{
	//��ԗp�f�[�^�̍X�V
	eMoveEye.Update();
	eMoveTarget.Update();

	//���W�̕��
	float eyeRate = eMoveEye.GetTimeRate();
	float targetRate = eMoveTarget.GetTimeRate();

	//��Ԃ̌`�ɂ����timerate��ό`
	if (eyeInterType == InterType::EaseIn) {
		eyeRate = In(eyeRate);
	}
	else if (eyeInterType == InterType::EaseOut) {
		eyeRate = Out(eyeRate);
	}

	if (targetInterType == InterType::EaseIn) {
		targetRate = In(targetRate);
	}
	else if (targetInterType == InterType::EaseOut) {
		targetRate = Out(targetRate);
	}

	Vector3 eye = Vector3::Lerp(eyeBefore, eyeAfter, eyeRate);
	Vector3 target = Vector3::Lerp(targetBefore, targetAfter, targetRate);

	camera->eye = eye;
	camera->target = target;
	camera->UpdateMatrix();

}

void EventCamera::MoveEye(const Vector3& eye, uint16_t time,InterType eyeInter, bool followTarget,InterType targetInter)
{
	//������������ŕ�ԊJ�n
	eMoveEye.Start((float)time);
	//���W�̃Z�b�g
	eyeAfter = eye;
	eyeBefore = camera->eye;
	eyeInterType = eyeInter;



	//�����_�������Ȃ��Ȃ珈���I���
	if (!followTarget) {
		targetBefore = camera->target;
		targetAfter = camera->target;
		return;
	}

	eMoveTarget.Start((float)time);
	//���_���W�̍��Œ����_���ǂꂾ�����������v�Z
	Vector3  targetLen = eyeAfter - eyeBefore;
	targetBefore = camera->target;
	targetAfter = targetBefore + targetLen;
	targetInterType = targetInter;

}

void EventCamera::MoveTarget(const Vector3& target, uint16_t time, InterType targetInter, bool followEye, InterType eyeInter)
{
	//��ԊJ�n
	eMoveTarget.Start((float)time);
	//���W�Z�b�g
	targetAfter = target;
	targetBefore = camera->target;
	targetInterType = targetInter;

	//���_�𓮂����Ȃ��Ȃ珈���I���
	if (!followEye) {
		eyeBefore = camera->eye;
		eyeAfter = camera->eye;
		return;
	}

	eMoveEye.Start((float)time);
	//���_���W�̍��Œ����_���ǂꂾ�����������v�Z
	Vector3 eyeLen = targetAfter - targetBefore;
	eyeBefore = camera->eye;
	eyeAfter = eyeBefore + eyeLen;
	eyeInterType = eyeInter;

}