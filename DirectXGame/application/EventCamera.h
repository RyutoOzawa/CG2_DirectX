#pragma once
//イベントカメラワーク用クラス
//各イベントでのカメラワークを行いやすいような機能つきカメラクラス

#include"Camera.h"
#include"EasingData.h"

class EventCamera
{
public:

	//初期化
	void Initialize(const Vector3& eye, const Vector3& target, const Vector3& up);

	//更新
	void Update();

	/// <summary>
	/// カメラ移動
	/// </summary>
	/// <param name="eye">移動後の視点</param>
	/// <param name="time">移動時間</param>
	/// <param name="eyeInter">視点の補間種類</param>
	/// <param name="followTarget">注視点が追従するかどうか</param>
	/// <param name="targetInter">注視点の補間の種類</param>
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

