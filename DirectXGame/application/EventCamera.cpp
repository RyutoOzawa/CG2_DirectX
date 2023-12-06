#include "EventCamera.h"
#include"Util.h"

void EventCamera::Initialize(const Vector3& eye, const Vector3& target, const Vector3& up)
{
	

	//カメラの初期化
	camera = std::make_unique<Camera>();
	camera->Initialize(eye, target, up);

}

void EventCamera::Update()
{
	//補間用データの更新
	eMoveEye.Update();
	eMoveTarget.Update();

	//座標の補間
	float eyeRate = eMoveEye.GetTimeRate();
	float targetRate = eMoveTarget.GetTimeRate();

	//補間の形によってtimerateを変形
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
	//もらった引数で補間開始
	eMoveEye.Start((float)time);
	//座標のセット
	eyeAfter = eye;
	eyeBefore = camera->eye;
	eyeInterType = eyeInter;

	//注視点が動かないなら処理終わり
	if (!followTarget) {
		return;
	}

	eMoveTarget.Start((float)time);
	//視点座標の差で注視点をどれだけ動かすか計算
	Vector3  targetLen = eyeAfter - eyeBefore;
	targetBefore = camera->target;
	targetAfter = targetBefore + targetLen;
	targetInterType = targetInter;

}
