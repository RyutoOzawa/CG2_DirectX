#include "EventCamera.h"

void EventCamera::Initialize(const Vector3& eye, const Vector3& target, const Vector3& up)
{
	

	//�J�����̏�����
	camera = std::make_unique<Camera>();
	camera->Initialize(eye, target, up);

}
