#pragma once
//イベントカメラワーク用クラス
//各イベントでのカメラワークを行いやすいような機能つきカメラクラス

#include"Camera.h"

class EventCamera
{
public:

	//初期化
	void Initialize(const Vector3& eye, const Vector3& target, const Vector3& up );


	//getter
	Camera* GetCamera()const { return camera.get(); }

private:
	std::unique_ptr<Camera> camera;



};

