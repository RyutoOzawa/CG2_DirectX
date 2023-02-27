#include "GamePlayScene.h"
#include"Texture.h"
#include"DirectX.h"
#include"SpriteManager.h"
#include"GameSceneManager.h"
#include"Collision.h"

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
	ImGui::Begin("skydome");
	ImGui::SliderFloat("rotateY", &skydomeObj->rotation.y, 0.0f, 5.0f);
	ImGui::SliderFloat("posX", &sphere.pos.x, -10.0f, 10.0f);
	ImGui::SliderFloat("posY", &sphere.pos.y, -10.0f, 10.0f);
	ImGui::SliderFloat("posZ", &sphere.pos.z, -10.0f, 10.0f);
	ImGui::SliderFloat("scaleX", &skydomeObj->scale.x, 0.0f, 5.0f);
	ImGui::SliderFloat("scaleY", &skydomeObj->scale.y, 0.0f, 5.0f);
	ImGui::SliderFloat("scaleZ", &skydomeObj->scale.z, 0.0f, 5.0f);
	ImGui::End();

	//���C�̑���
	ImGui::Begin("Ray");
	ImGui::Text("cube wo scale de ookiku siteiru tame usiro ha hanntei arimasen");
	ImGui::SliderFloat("posX", &ray.start.x, -10.0f, 10.0f);
	ImGui::SliderFloat("posZ", &ray.start.z, -10.0f, 10.0f);
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

	//----------------------�Q�[�������[�v�͂����܂�---------------------//


}

void GamePlayScene::Draw()
{
	//-------�w�i�X�v���C�g�`�揈��-------//
	SpriteManager::GetInstance()->beginDraw();

	//backGroundSprite->Draw();

	//-------3D�I�u�W�F�N�g�`�揈��-------//
	Object3d::BeginDraw(camera);

	skydomeObj->Draw();
	planeObj->Draw();
	triangleObj->Draw();
	rayObj->Draw();


	//-------�O�i�X�v���C�g�`�揈��-------//
	SpriteManager::GetInstance()->beginDraw();

	//sprite->Draw();
	//sprite2->Draw();
}
