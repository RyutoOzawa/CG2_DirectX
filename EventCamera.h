#pragma once
//�C�x���g�J�������[�N�p�N���X
//�e�C�x���g�ł̃J�������[�N���s���₷���悤�ȋ@�\���J�����N���X

#include"Camera.h"

class EventCamera
{
public:

	//������
	void Initialize(const Vector3& eye, const Vector3& target, const Vector3& up );


	//getter
	Camera* GetCamera()const { return camera.get(); }

private:
	std::unique_ptr<Camera> camera;



};

