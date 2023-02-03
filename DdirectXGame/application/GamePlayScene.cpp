#include "GamePlayScene.h"
#include"Texture.h"
#include"DirectX.h"
#include"SpriteManager.h"
#include"GameSceneManager.h"

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

	//�J����������
	Vector3 eye(0, 0, -20);	//���_���W
	Vector3 target(0, 0, 6);	//�����_���W
	Vector3 up(0, 1, 0);		//������x�N�g��

	camera.Initialize(eye, target, up);

	skydomeObj = std::make_unique<Object3d>();
	skydomeObj->Initialize();
	skydomeObj->SetModel(skydome.get());

	newAudio = std::make_unique<AudioManager>();
	newAudio->SoundLoadWave("Resources/bgm_title.wav");

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

	//AD�œV���̉�]
	ImGui::Begin("debug");

	ImGui::SliderFloat("Skydome:rotateY", &skydomeObj->rotation.y, 0.0f, 5.0f);
	ImGui::SliderFloat("Skydome:posX", &skydomeObj->position.x, 0.0f, 5.0f);
	ImGui::SliderFloat("Skydome:posY", &skydomeObj->position.y, 0.0f, 5.0f);
	ImGui::SliderFloat("Skydome:scaleX", &skydomeObj->scale.x, 0.0f, 5.0f);
	ImGui::SliderFloat("Skydome:scaleY", &skydomeObj->scale.y, 0.0f, 5.0f);
	ImGui::SliderFloat("Skydome:scaleZ", &skydomeObj->scale.z, 0.0f, 5.0f);

	if (ImGui::Button("music")) {
		newAudio->SoundPlayWave();
	}

	ImGui::End();

	skydomeObj->Update();

	//----------------------�Q�[�������[�v�͂����܂�---------------------//


}

void GamePlayScene::Draw()
{
	//-------�w�i�X�v���C�g�`�揈��-------//
	SpriteManager::GetInstance()->beginDraw();

	backGroundSprite->Draw();

	//-------3D�I�u�W�F�N�g�`�揈��-------//
	Object3d::BeginDraw(camera);

	skydomeObj->Draw();

	//-------�O�i�X�v���C�g�`�揈��-------//
	SpriteManager::GetInstance()->beginDraw();

	sprite->Draw();
	sprite2->Draw();
}
