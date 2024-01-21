#include "EventCamera.h"
#include"Util.h"
#include"ImguiManager.h"
using namespace Utility;
using namespace Easing;

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

	ImGui::Text("pos before %f,%f,%f", eyeBefore.x, eyeBefore.y, eyeBefore.z);
	ImGui::Text("pos after %f,%f,%f", eyeAfter.x, eyeAfter.y, eyeAfter.z);
	ImGui::Text("camera eyeeeeeee %f,%f,%f", camera->eye.x, camera->eye.y, camera->eye.z);

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

	if (eyeRate < 1.0f && eyeRate > 0.0f) {
		Vector3 eye = Vector3::Lerp(eyeBefore, eyeAfter, eyeRate);
		camera->eye = eye;
	}
	else if (eyeRate >= 1.0f) {
		camera->eye = eyeAfter;
		eyeBefore = camera->eye;
	}

	if (targetRate < 1.0f && targetRate > 0.0f) {
		Vector3 target = Vector3::Lerp(targetBefore, targetAfter, targetRate);
		camera->target = target;
	}

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
		targetBefore = camera->target;
		targetAfter = camera->target;
		return;
	}

	eMoveTarget.Start((float)time);
	//視点座標の差で注視点をどれだけ動かすか計算
	Vector3  targetLen = eyeAfter - eyeBefore;
	targetBefore = camera->target;
	targetAfter = targetBefore + targetLen;
	targetInterType = targetInter;

}

void EventCamera::MoveTarget(const Vector3& target, uint16_t time, InterType targetInter, bool followEye, InterType eyeInter)
{
	//補間開始
	eMoveTarget.Start((float)time);
	//座標セット
	targetAfter = target;
	targetBefore = camera->target;
	targetInterType = targetInter;

	//視点を動かさないなら処理終わり
	if (!followEye) {
		eyeBefore = camera->eye;
		eyeAfter = camera->eye;
		return;
	}

	eMoveEye.Start((float)time);
	//視点座標の差で注視点をどれだけ動かすか計算
	Vector3 eyeLen = targetAfter - targetBefore;
	eyeBefore = camera->eye;
	eyeAfter = eyeBefore + eyeLen;
	eyeInterType = eyeInter;

}
