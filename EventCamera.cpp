#include "EventCamera.h"

void EventCamera::Initialize(const Vector3& eye, const Vector3& target, const Vector3& up)
{
	

	//カメラの初期化
	camera = std::make_unique<Camera>();
	camera->Initialize(eye, target, up);

}
