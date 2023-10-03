#include "GameTitleScene.h"
#include"Texture.h"
#include"DirectX.h"
using namespace DirectX;
#include"GameSceneManager.h"
#include"WindowsAPI.h"
#include"Util.h"

void GameTitleScene::Initialize()
{


	//--------------�Q�[�����ϐ�������--------------//

	//input�̃C���X�^���X�擾
	input = Input::GetInstance();

	//�e�N�X�`���f�[�^������
	titleTexture = Texture::LoadTexture("dummyTitle.png");
	texTextTitleLogo = Texture::LoadTexture("titleLogo.png");
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
		sceneManager->ChangeScene("GAMEPLAY");
	}

	if (input->IsKeyTrigger(DIK_F1))
	{
		//�V�[���̐؂�ւ����˗�
		sceneManager->ChangeScene("TESTSCENE");
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

	//----------------------�Q�[�������[�v�͂����܂�---------------------//


}

void GameTitleScene::Draw()
{
	//-------3D�I�u�W�F�N�g�`��------//
	Object3d::BeginDraw(camera.get());


	skydomeObj->Draw();


	//-------�X�v���C�g�`�揈��-------//
	Sprite::BeginDraw();

	//titleSprite->Draw();

	spTextTitleLogo->Draw();

	spTextPressA->Draw();


}
