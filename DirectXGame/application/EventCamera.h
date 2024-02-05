#pragma once
//イベントカメラワーク用クラス
//各イベントでのカメラワークを行いやすいような機能つきカメラクラス

#include"Camera.h"
#include"EasingData.h"
#include"BezierCurve.h"

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
	void MoveEye(const Vector3& eye, uint16_t time, Easing::InterType eyeInter = Easing::InterType::Lerp,
		bool followTarget = true,Easing::InterType targetInter = Easing::InterType::Lerp);

	/// <summary>
	/// ベジエ曲線によるカメラ移動
	/// </summary>
	/// <param name="curve">ベジエ曲線</param>
	/// <param name="time">移動時間</param>
	/// <param name="eyeInter">視点の補間種類</param>
	/// <param name="followTarget">注視点が追従するか？</param>
	/// <param name="targetInter">注視点の補間の種類</param>
	/*void MoveEye(const BezierCurve& curve, uint16_t time, Easing::InterType eyeInter = Easing::InterType::Lerp,
		bool followTarget = true, Easing::InterType targetInter = Easing::InterType::Lerp);*/
	/// <summary>
	/// カメラ注視点移動
	/// </summary>
	/// <param name="target">移動後の注視点</param>
	/// <param name="time">移動時間</param>
	/// <param name="targetInter">注視点の補間種類</param>
	/// <param name="followEye">視点が追従するかどうか</param>
	/// <param name="eyeInter">視点の</param>
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

