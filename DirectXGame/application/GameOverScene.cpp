//�Q�[���I�[�o�[�V�[���N���X
//�Q�[���I�[�o�[�ɂȂ����ۂɈȍ~�����ʂ̏������s��

#include "GameOverScene.h"
#include"Input.h"
#include"GameSceneManager.h"

void GameOverScene::Initialize()
{
	//�e�N�X�`���f�[�^
	gameoverTexture = Texture::LoadTexture("gameover.png");

	//�X�v���C�g
	gameoverSprite = new Sprite();
	gameoverSprite->Initialize(gameoverTexture);
}

void GameOverScene::Finalize()
{
	delete gameoverSprite;
}

void GameOverScene::Update()
{
	

	//�X�y�[�X�L�[�Ń��C���Q�[����
	if (Input::GetInstance()->IsKeyTrigger(DIK_SPACE) || Input::GetInstance()->IsPadTrigger(XINPUT_GAMEPAD_START))
	{
		//�V�[���̐؂�ւ����˗�
		sceneManager->ChangeScene("TITLE");
	}
}

void GameOverScene::Draw()
{
	Sprite::BeginDraw();

	gameoverSprite->Draw();

}
