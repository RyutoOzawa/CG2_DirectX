#include "GamePlayScene.h"
#include"Texture.h"
#include"DirectX.h"
#include"GameSceneManager.h"
#include"Collision.h"
#include"FbxLoader.h"
#include"FbxObject3d.h"
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
	particleGraph = Texture::LoadTexture(L"Resources/particle.png");
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
	//triangleModel->textureIndex = particleGraph;

	//�J����������
	Vector3 eye(0, 20, -20);	//���_���W
	Vector3 target(0, 0, 6);	//�����_���W
	Vector3 up(0, 1, 0);		//������x�N�g��

	sprite2->SetPos({ 240, 240 });

	camera = new Camera();
	camera->Initialize(eye, target, up);

	skydomeObj = std::make_unique<Object3d>();
	skydomeObj->Initialize();
	skydomeObj->SetModel(skydome.get());
	//skydomeObj->scale = { 100,100,100 };

	planeObj = std::make_unique<Object3d>();
	planeObj->Initialize();
	planeObj->SetModel(defaultModel.get());
	planeObj->scale = { 10.0f,0.01f,10.0f };
	planeObj->position.y = -2.0f;

	triangleObj = std::make_unique<Object3d>();
	triangleObj->Initialize();
	triangleObj->SetModel(triangleModel.get());

	particleMan = std::make_unique<ParticleManager>();
	particleMan->Initialize(particleGraph);
	
	for (int i = 0; i < 100; i++) {
		//X,Y,Z���ׂ�[-5.0f,+5.0f]�Ń����_���ɕ��z
		const float randPos = 5.0f;
		Vector3 pos{};
		pos.x = Random(-randPos, randPos);
		pos.y = Random(-randPos, randPos);
		pos.z = Random(-randPos, randPos);
		//x,y,z���ׂā}0.05�ŕ��z
		const float randVel = 0.05f;
		Vector3 vel{};
		vel.x = Random(-randVel, randVel);
		vel.y = Random(-randVel, randVel);
		vel.z = Random(-randVel, randVel);
		//�d�͂Ɍ����Ă�Y�̂�-0.001f~0�Ń����_���ɕ��z
		Vector3 acc{};
		const float randAcc = 0.001f;
		acc.y = Random(-randAcc, 0.0f);

		//�ǉ�
		particleMan->Add(60, pos, vel, acc,1.0f,0.0f);
	}

	rayObj = std::make_unique<Object3d>();
	rayObj->Initialize();
	rayObj->SetModel(skydome.get());
	//	rayObj->scale = { 0.01f,10.0f,0.01f };

	newAudio = std::make_unique<AudioManager>();
	newAudio->SoundLoadWave("Resources/bgm_title.wav");

	//���̏����l��ݒ�
	sphere.pos = { -1,1,-0 };
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


	//���f�������w�肵�ăt�@�C���ǂݍ���
	//model1 = std::make_unique<FbxModel>();
	model1 = FbxLoader::GetInstance()->LoadModelFromFile("boneTest");

	object1 = std::make_unique<FbxObject3d>();
	object1->Initialize();
	object1->SetModel(model1.get());

	//�f�o�C�X�Z�b�g
	FbxObject3d::SetCamera(camera);

	camera->target = { 0,0,0 };
	//	camera->eye = { 0,0,-20 };
	camera->eye = { 0,0,-50 };

	levelData = JsonLoader::LoadJsonFile("levelDataTest");

	static int a = 0;
	a++;

	JsonLoader::CreateObjectFromLevelData(levelData, stageObjects,stageModels);

}

void GamePlayScene::Finalize()
{
	//�Q�[�����[�v�Ŏg�p�������������A��ՃV�X�e���̌㏈���Ɖ�����s��

	//delete sprite;
	//delete skyDome;

	delete camera;

	//-------------�����܂łɃ��[�v���Ŏg�p�������̂̌㏈��------------//



}

void GamePlayScene::Update()
{


	//----------------------�Q�[�������[�v�͂�������---------------------//



	camera->target = { 0,5,0 };

	ImGui::SliderFloat("cameraX", &camera->eye.x, -100.0f, 100.0f);
	ImGui::SliderFloat("cameraY", &camera->eye.y, -100.0f, 100.0f);
	ImGui::SliderFloat("cameraZ", &camera->eye.z, -100.0f, 100.0f);

	//camera->target = camera->eye;
	//camera->target.z += 1.0f;

	camera->UpdateMatrix();

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

	////���C�̑���
	//ImGui::Begin("Ray");
	//ImGui::Text("cube wo scale de ookiku siteiru tame usiro ha hanntei arimasen");
	//ImGui::SliderFloat("posX", &ray.start.x, -10.0f, 10.0f);
	//ImGui::SliderFloat("posZ", &ray.start.z, -10.0f, 10.0f);
	//ImGui::End();

	Vector2 sprite1Pos = { sprite->GetPosition().x,sprite->GetPosition().y };
	Vector2 sprite2Pos = { sprite2->GetPosition().x,sprite2->GetPosition().y };
	float pos[2] = { sprite1Pos.x,sprite1Pos.y };
	ImGui::Begin("Sprite");

	ImGui::SliderFloat2("sprite1", pos, 0.0f, WindowsAPI::winW);
	sprite->SetPos({ pos[0],pos[1] });


	sprite->Update();
	sprite2->Update();

	ImGui::End();

	skydomeObj->position = sphere.pos;
	skydomeObj->Update();
	planeObj->Update();

	rayObj->position = { 1,1,0 };
	rayObj->Update();


	triangleObj->position = { 0,0,0 };

	triangleObj->camera = camera;

	ImGui::Checkbox("is billboard", &triangleObj->isBillboard);
	ImGui::Checkbox("is billboardY", &triangleObj->isBillboardY);


	triangleObj->Update();

	//X,Y,Z���ׂ�[-5.0f,+5.0f]�Ń����_���ɕ��z
	{
		const float randPos = 5.0f;
		Vector3 pos{};
		pos.x = Random(-randPos, randPos);
		pos.y = Random(-randPos, randPos);
		pos.z = Random(-randPos, randPos);
		//x,y,z���ׂā}0.05�ŕ��z
		const float randVel = 0.05f;
		Vector3 vel{};
		vel.x = Random(-randVel, randVel);
		vel.y = Random(-randVel, randVel);
		vel.z = Random(-randVel, randVel);
		//�d�͂Ɍ����Ă�Y�̂�-0.001f~0�Ń����_���ɕ��z
		Vector3 acc{};
		const float randAcc = 0.001f;
		acc.y = Random(-randAcc, 0.0f);

		//�ǉ�
		particleMan->Add(60, pos, vel, acc, 1.0f, 0.0f);


	}
	particleMan->Update();

	//�A�j���[�V�����J�n�{�^��
	if (ImGui::Button("animation start")) {
		object1->PlayAnitimation();
	}

	if (input->IsKeyTrigger(DIK_A)) {
		object1->PlayAnitimation();
	}

	ImGui::Text("current:%d", object1->BaGetCurrentTime());

	//object1->SetRot({ 0,3.14f / 2,0 });
	object1->Update();

	//�X�y�[�X�L�[�Ń��C���Q�[����
	if (input->IsKeyTrigger(DIK_SPACE))
	{
		//�V�[���̐؂�ւ����˗�
		sceneManager->ChangeScene("TITLE");
	}

	//----------------------�Q�[�������[�v�͂����܂�---------------------//


}

void GamePlayScene::Draw()
{
	//-------�w�i�X�v���C�g�`�揈��-------//
	Sprite::BeginDraw();

	//backGroundSprite->Draw();

	//-------3D�I�u�W�F�N�g�`�揈��-------//
	Object3d::BeginDraw(camera);

	skydomeObj->Draw();
	//rayObj->Draw();
	planeObj->Draw();
	//triangleObj->Draw();

	for (int i = 0; i < stageObjects.size(); i++) {
		stageObjects[i]->Draw();
	}

	object1->Draw();

	

	//�p�[�e�B�N���`��
	ParticleManager::BeginDraw(camera);
	particleMan->Draw();

	//-------�O�i�X�v���C�g�`�揈��-------//
	Sprite::BeginDraw();

	//sprite->Draw();
	//sprite2->Draw();
}
