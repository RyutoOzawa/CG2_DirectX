//�^�C�g���V�[���N���X
//�Q�[�����N�������Ƃ��̍ŏ��̉�ʂ̏������s��

#include "GameTitleScene.h"
#include"Texture.h"
#include"DirectX.h"
using namespace DirectX;
#include"GameSceneManager.h"
#include"WindowsAPI.h"
#include"Util.h"
#include"ImguiManager.h"
using namespace Utility;
using namespace Easing;

void GameTitleScene::Initialize()
{


	//--------------�Q�[�����ϐ�������--------------//

	//input�̃C���X�^���X�擾
	input = Input::GetInstance();

	//�e�N�X�`���f�[�^������
	titleTexture = Texture::LoadTexture("dummyTitle.png");
	//titleTexture = Texture::LoadTexture("mari.dds");
	texTextTitleLogo = Texture::LoadTexture("titleLogo.png");
	//texTextTitleLogo = Texture::LoadTexture("mari.dds");
	texTextPressA = Texture::LoadTexture("textPressA.png");

	titleSprite = new Sprite();
	titleSprite->Initialize(titleTexture);

	spTextTitleLogo = std::make_unique<Sprite>();
	spTextTitleLogo->Initialize(texTextTitleLogo);
	spTextTitleLogo->SetAnchorPoint({ 0.5f,0.5f });
	spTextTitleLogo->SetPos({ WindowsAPI::winW / 2.0f,WindowsAPI::winH / 2.0f - WindowsAPI::winH / 4.0f });
	//�^�C�g���̃T�C�Y��n�{�ɂ���
	Vector2 size = spTextTitleLogo->GetSize();
	size *= 1.5f;
	spTextTitleLogo->SetSize(size);



	spTextPressA = std::make_unique<Sprite>();
	spTextPressA->Initialize(texTextPressA);
	spTextPressA->SetAnchorPoint({ 0.5f,0.5f });
	spTextPressA->SetPos({ WindowsAPI::winW / 2.0f,WindowsAPI::winH / 2.0f + WindowsAPI::winH / 4.0f });

	skydome = std::make_unique<Model>();
	skydome = Model::CreateModel("skydome");

	skydomeObj = std::make_unique<Object3d>();
	skydomeObj->Initialize();
	skydomeObj->SetModel(skydome.get());
	skydomeObj->scale = { 1000,1000,1000 };

	//�G
	enemyModel = std::make_unique<Model>();
	enemyModel = Model::CreateModel("ZakoEnemy");

	float enemyPosZ = 100.0f;
	for (uint8_t i = 0; i < enemyMax; i++) {
		enemysObj[i] = std::make_unique<Object3d>();
		enemysObj[i]->Initialize();
		enemysObj[i]->SetModel(enemyModel.get());
		enemysObj[i]->position.z = enemyPosZ;
		float posX = 200.0f;
		if (i == 0) {
			enemysObj[i]->position.x = posX;
		}
		else {
			enemysObj[i]->position.x = enemysObj[i - 1]->position.x + 15.0f;
		}
	}

	//�J����������
	camera = std::make_unique<Camera>();
	Vector3 eye, target, up;
	eye = { 0,0,0 };
	target = { 0,0,1 };
	up = { 0,1,0 };

	camera->Initialize(eye, target, up);

}

void GameTitleScene::Finalize()
{
	//�Q�[�����[�v�Ŏg�p�������������A��ՃV�X�e���̌㏈���Ɖ�����s��

	//delete sprite;
	//delete skyDome;

	delete titleSprite;
	//-------------�����܂łɃ��[�v���Ŏg�p�������̂̌㏈��------------//



}

void GameTitleScene::Update()
{


	//----------------------�Q�[�������[�v�͂�������---------------------//

	//�X�y�[�X�L�[�Ń��C���Q�[����
	if (input->IsKeyTrigger(DIK_SPACE) || input->IsPadTrigger(XINPUT_GAMEPAD_A))
	{
		//�V�[���̐؂�ւ����˗�
		sceneManager->ChangeScene("GAMEPLAY",true,"SLIDE");
	}

	if (input->IsKeyTrigger(DIK_F1))
	{
		//�V�[���̐؂�ւ����˗�
		sceneManager->ChangeScene("TESTSCENE",false);
	}

	//�e�L�X�g�̐F�𖾂邭������Â������肷��
	float colorRadSpd = 2.5f;
	textColorRad+= colorRadSpd;
	if (textColorRad > 360.0f) {
		textColorRad -= 360.0f;
	}
	float alpha = (sinf(textColorRad * PI / 180.0f) + 1.0f) / 2.0f;
	Vector4 texColor = spTextPressA->GetColor();
	texColor.w = alpha;
	spTextPressA->SetColor(texColor);

	//�V������
	
	skydomeObj->rotation.y += PI / 14400.0f;
	skydomeObj->Update();

	//�G�̈ړ�����
	ImGui::SliderFloat("enemy[0].x", &enemysObj[0]->position.x, -100.0f, 100.0f);

	for (uint8_t i = 0; i < enemyMax; i++) {
		enemysObj[i]->rotation.y = PI / 2.0f;
		float enemyRad = textColorRad - 18.0f * i;
		float enemyPosY = ((sinf(enemyRad* PI / 180.0f) + 1.0f) / 2.0f) * 60.0f - 30.0f;
		enemysObj[i]->position.y = enemyPosY;
		enemysObj[i]->position.x -= 0.75f;
		if (enemysObj[i]->position.x < -100.0f) {
			enemysObj[i]->position.x += 400.0f;
		}

		enemysObj[i]->Update();
		ImGui::Text("posY %f", enemyPosY);

	}

	//�^�C�g�����S�̉�]
	if (logoRotTimer > 0) {
		logoRotTimer--;
	}
	else if(logoRotTimer == 0){
		logoRotTimer = logoRotTimeMax;
		//5�b�Ɉ��m���Ń��S��]
		if ((uint16_t)Random(0.0f, 10.0f) % 10 <= 2) {
			easeLogoRot.Start(60.0f);
		}
	}

	ImGui::Text("ease %f", easeLogoRot.GetTimeRate());

	easeLogoRot.Update();
	float rot = Lerp(0.0f, PI * 6.0f, Out(easeLogoRot.GetTimeRate()));
	spTextTitleLogo->SetRotation(rot);

	//----------------------�Q�[�������[�v�͂����܂�---------------------//


}

void GameTitleScene::Draw()
{
	//-------3D�I�u�W�F�N�g�`��------//
	Object3d::BeginDraw(camera.get());


	skydomeObj->Draw();

	for (std::unique_ptr<Object3d>& enemy : enemysObj) {
		enemy->Draw();
	}

	//-------�X�v���C�g�`�揈��-------//
	Sprite::BeginDraw();

	//titleSprite->Draw();

	spTextTitleLogo->Draw();

	spTextPressA->Draw();


}
