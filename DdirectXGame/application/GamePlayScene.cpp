#include "GamePlayScene.h"
#include"Texture.h"
#include"DirectX.h"
#include"SpriteManager.h"
#include"GameSceneManager.h"
#include"Collision.h"
#include"Util.h"


using namespace DirectX;

void GamePlayScene::Initialize()
{


	//--------------�Q�[�����ϐ�������--------------//

	//input�̃C���X�^���X�擾
	input = Input::GetInstance();

	//�e�N�X�`���f�[�^������

	backGroundTexture = Texture::LoadTexture(L"Resources/dummyPlayGame.png");
	marioGraph = Texture::LoadTexture(L"Resources/mario.jpg");
	reimuGraph = Texture::LoadTexture(L"Resources/reimu.png");
	backGroundSprite = std::make_unique<Sprite>();
	sprite = std::make_unique<Sprite>();
	sprite2 = std::make_unique<Sprite>();
	backGroundSprite->Initialize(backGroundTexture);
	sprite->Initialize(marioGraph);
	sprite2->Initialize(reimuGraph);

	skydome = std::make_unique<Model>();
	skydome = Model::CreateModel("skydome");

	defaultModel = std::make_unique<Model>();
	defaultModel = Model::CreateModel();
	defaultModel->textureIndex = reimuGraph;

	triangleModel = std::make_unique<Model>();
	triangleModel = Model::CreateModel("triangle_mat");

	//�J����������
	Vector3 eye(0, 20, -20);	//���_���W
	Vector3 target(0, 0, 6);	//�����_���W
	Vector3 up(0, 1, 0);		//������x�N�g��

	camera.Initialize(eye, target, up);

	skydomeObj = std::make_unique<Object3d>();
	skydomeObj->Initialize();
	skydomeObj->SetModel(skydome.get());

	planeObj = std::make_unique<Object3d>();
	planeObj->Initialize();
	planeObj->SetModel(defaultModel.get());
	planeObj->scale = { 10.0f,0.01f,10.0f };
	planeObj->position.y = -2.0f;

	triangleObj = std::make_unique<Object3d>();
	triangleObj->Initialize();
	triangleObj->SetModel(triangleModel.get());

	rayObj = std::make_unique<Object3d>();
	rayObj->Initialize();
	rayObj->SetModel(defaultModel.get());
	rayObj->scale = { 0.01f,10.0f,0.01f };

	newAudio = std::make_unique<AudioManager>();
	newAudio->SoundLoadWave("Resources/bgm_title.wav");

	//���̏����l��ݒ�
	sphere.pos = { -5,2,-0 };
	sphere.radius = 1.0f;
	//���ʂ̏����l��ݒ�
	plane.normal = { 0,1,0 };
	plane.distance = -2.0f;
	//�O�p�`�̏����l��ݒ�
	triangle.p0 = { -1.0f,0,-1.0f };
	triangle.p1 = { -1.0f,0,+1.0f };
	triangle.p2 = { +1.0f,0,-1.0f };
	triangle.normal = { 0.0f,1.0f,0.0f };
	//���C�̏����l��ݒ�
	ray.start = { 0,1,0 };
	ray.dir = { 0,-1,0 };

	//�p�[�e�B�N��������
	particle1 = std::make_unique<ParticleManager>();
	particle1->Initialize();
	particle1->SetTextureHandle(reimuGraph);

	particle2 = std::make_unique<ParticleManager>();
	particle2->Initialize();
	particle2->SetTextureHandle(marioGraph);

	particleStart1 = { -10,0,0 };
	particleStart2 = { 10,0,0 };

}

void GamePlayScene::Finalize()
{
	//�Q�[�����[�v�Ŏg�p�������������A��ՃV�X�e���̌㏈���Ɖ�����s��

	//delete sprite;
	//delete skyDome;

	//-------------�����܂łɃ��[�v���Ŏg�p�������̂̌㏈��------------//



}

void GamePlayScene::Update()
{


	//----------------------�Q�[�������[�v�͂�������---------------------//





	camera.UpdateMatrix();

	//�V���̑���
	//ImGui::Begin("skydome");
	//ImGui::SliderFloat("rotateY", &skydomeObj->rotation.y, 0.0f, 5.0f);
	//ImGui::SliderFloat("posX", &sphere.pos.x, -10.0f, 10.0f);
	//ImGui::SliderFloat("posY", &sphere.pos.y, -10.0f, 10.0f);
	//ImGui::SliderFloat("posZ", &sphere.pos.z, -10.0f, 10.0f);
	//ImGui::SliderFloat("scaleX", &skydomeObj->scale.x, 0.0f, 5.0f);
	//ImGui::SliderFloat("scaleY", &skydomeObj->scale.y, 0.0f, 5.0f);
	//ImGui::SliderFloat("scaleZ", &skydomeObj->scale.z, 0.0f, 5.0f);
	//ImGui::End();

	//�J��������f�o�b�O�e�L�X�g
	ImGui::Begin("camera");
	ImGui::SliderFloat("eyeX", &camera.eye.x, -10.0f, 10.0f);
	ImGui::SliderFloat("eyeY", &camera.eye.y, -10.0f, 10.0f);
	ImGui::SliderFloat("eyeZ", &camera.eye.z, -10.0f, 10.0f);
	ImGui::End();

	//�����蔻��m�F
	ImGui::Begin("collision");

	if (Collision::ColRayToSphere(ray,sphere,nullptr,&colHitPos)) {
		ImGui::Text("hit Ray to Sphere!");
		ImGui::Text("hitPos:(%2.2f,%2.2f,%2.2f)", colHitPos.x, colHitPos.y, colHitPos.z);
	}

	if (Collision::ColRayToTriangle(ray,triangle, nullptr, &colHitPos)) {
		ImGui::Text("hit Ray to triangle!");
		ImGui::Text("hitPos:(%2.2f,%2.2f,%2.2f)", colHitPos.x, colHitPos.y, colHitPos.z);
	}

	if (Collision::ColRayToPlane(ray, plane, nullptr, &colHitPos)) {
		ImGui::Text("hit Ray to Plane!");
		ImGui::Text("hitPos:(%2.2f,%2.2f,%2.2f)", colHitPos.x, colHitPos.y, colHitPos.z);
	}

	if (Collision::ColSphereToPlane(sphere,plane, &colHitPos)) {
		ImGui::Text("hit Plane to Sphere!");
		ImGui::Text("hitPos:(%2.2f,%2.2f,%2.2f)", colHitPos.x, colHitPos.y, colHitPos.z);
	}

	if (Collision::ColSphereToTriangle(sphere, triangle, &colHitPos)) {
		ImGui::Text("hit triangle to Sphere!");
		ImGui::Text("hitPos:(%2.2f,%2.2f,%2.2f)", colHitPos.x, colHitPos.y, colHitPos.z);
	}

	ImGui::End();

	skydomeObj->position = sphere.pos;
	skydomeObj->Update();
	planeObj->Update();

	rayObj->position = ray.start;
	rayObj->Update();

	//�p�[�e�B�N���𐶐���������
	Vector3 particleEndPos1;
	Vector3 particleEndPos2;

	//�p�[�e�B�N���I�_�͎n�_���烉���_���ɑ���
	particleEndPos1 = particleStart1 + Vector3(Random(-10,10), Random(-10, 10), Random(-10, 10));
	particleEndPos2 = particleStart2 + Vector3(Random(-10,10), Random(-10, 10), Random(-10, 10));

	particle1->Add(ParticleManager::Type::Normal, 30, false, particleStart1, Vector3{ 0,0,0 }, particleEndPos1, 0.1f, 2,
		Vector4{ 1,1,1,1 }, Vector4{ 0,0,0,1 });

	particle2->Add(ParticleManager::Type::Normal, 30, false, particleStart2, Vector3{ 0,0,0 }, particleEndPos2,0.1f, 2,
		Vector4{ 0,0,0,1 }, Vector4{ 1,1,1,1 });

	particle1->Update();
	particle2->Update();

	sprite->SetSize(XMFLOAT2{ 64, 64 });
	sprite->SetPos(XMFLOAT2{ 700,0 });
	sprite->Update();

	sprite2->SetSize(XMFLOAT2{ 64, 64 });
	sprite2->Update();

	//----------------------�Q�[�������[�v�͂����܂�---------------------//


}

void GamePlayScene::Draw()
{



	//-------�w�i�X�v���C�g�`�揈��-------//
	SpriteManager::GetInstance()->beginDraw();

	//backGroundSprite->Draw();
	// 

		//�p�[�e�B�N���`�揈��
	ParticleManager::BeginDraw();

	particle1->Draw(camera);
	particle2->Draw(camera);

	ParticleManager::EndDraw();


	//-------3D�I�u�W�F�N�g�`�揈��-------//
	Object3d::BeginDraw(camera);

	skydomeObj->Draw();
	//planeObj->Draw();
	triangleObj->Draw();
	rayObj->Draw();


	//-------�O�i�X�v���C�g�`�揈��-------//
	SpriteManager::GetInstance()->beginDraw();

	sprite->Draw();
	sprite2->Draw();
}
