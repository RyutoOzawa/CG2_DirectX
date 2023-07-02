#include "RailCamera.h"
#include"ImguiManager.h"
#include"Util.h"

RailCamera::~RailCamera()
{
	delete camera;
	delete world;

}

void RailCamera::Initialize(const Vector3& position, const Vector3& rotation)
{
	world = new Object3d();
	camera = new Camera();

	//���[���h���W�����ݒ�
	world->Initialize();
	world->position = position;
	world->rotation = rotation;
	world->Update();

	Vector3 eye = { 0,0,0 };
	Vector3 target = { 0,0,10 };
	Vector3 up = { 0,1,0 };

	camera->Initialize(eye, target, up);



}

void RailCamera::Update()
{
	//���[���J�����p�f�o�b�O�e�L�X�g
	ImGui::Begin("railCamera");


	//���[���h���W�̍X�V
	ImGui::SliderFloat("posX", &world->position.x, -100.0f, 100.0f);
	ImGui::SliderFloat("posY", &world->position.y, -100.0f, 100.0f);
	ImGui::SliderFloat("posZ", &world->position.z, -100.0f, 100.0f);

	Vector3 rotation = world->rotation;
	ImGui::SliderFloat("rotX", &rotation.x, (float)-PI, (float)PI);
	ImGui::SliderFloat("rotY", &rotation.y, (float)-PI, (float)PI);
	ImGui::SliderFloat("rotZ", &rotation.z, (float)-PI, (float)PI);
	world->rotation = rotation;

	world->Update();

	camera->eye = world->position;
	//���[���h�O���x�N�g��
	Vector3 forward(0, 0, 1);
	//���[���J�����̉�]�𔽉f
	forward = Matrix4::transform(forward, world->matWorld);
	//���_+�O���x�N�g���Œ����_
	camera->target = camera->eye + forward;
	camera->target = forward;
	//������x�N�g��
	Vector3 up(0, 1, 0);
	//�J�����̏������forward�Ɠ��l�ɕϊ�
	//camera->up = Matrix4::transform(up, world->matWorld);
	//�J�����X�V
	camera->UpdateMatrix();


	ImGui::Text("eye:%f,%f,%f", camera->eye.x, camera->eye.y, camera->eye.z);
	ImGui::Text("target:%f,%f,%f", camera->target.x, camera->target.y, camera->target.z);
	ImGui::End();

}
