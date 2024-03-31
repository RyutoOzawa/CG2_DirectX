//�Q�[���N���A�V�[���N���X
//�Q�[�����N���A�����Ƃ��Ɉȍ~����V�[���̏������s��

#include "GameClearScene.h"
#include"Input.h"
#include"GameSceneManager.h"
#include"Util.h"

void GameClearScene::Initialize()
{
	//�e�N�X�`��
	gameClearTexture = Texture::LoadTexture("gameClearText.png");


	//�X�v���C�g
	gameClearSprite = new Sprite();
	gameClearSprite->Initialize(gameClearTexture);
	gameClearSprite->SetAnchorPoint({ 0.5f,0.5f });
	gameClearSprite->SetPos({ WindowsAPI::winW / 2.0f,WindowsAPI::winH / 2.0f });


	//�V��
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

	//���V�[���ւ̃^�C�}�[���Z�b�g
	nextSceneTimer = nextSceneTimerMax;
}

void GameClearScene::Finalize()
{
	delete gameClearSprite;
}

void GameClearScene::Update()
{
	//�V����n���ɉ�]
	skydomeObj->rotation.y += PI / 14400.0f;
	skydomeObj->Update();

	//�X�y�[�X�L�[�Ń^�C�g����
	if (Input::GetInstance()->IsKeyTrigger(DIK_SPACE) || Input::GetInstance()->IsPadTrigger(XINPUT_GAMEPAD_START))
	{
		//�V�[���̐؂�ւ����˗�
		sceneManager->ChangeScene("TITLE");
	}

	//�����ŃV�[���؂�ւ����s��
	if (nextSceneTimer > 0) {
		nextSceneTimer--;
	}
	else {
		//�V�[���̐؂�ւ����˗�
		sceneManager->ChangeScene("TITLE");
	}
}

void GameClearScene::Draw()
{


	Object3d::BeginDraw(camera.get());

	skydomeObj->Draw();


	Sprite::BeginDraw();

	gameClearSprite->Draw();
}
