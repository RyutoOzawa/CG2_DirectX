//�Q�[���N���A�V�[���N���X
//�Q�[�����N���A�����Ƃ��Ɉȍ~����V�[���̏������s��

#include "GameClearScene.h"
#include"Input.h"
#include"GameSceneManager.h"

void GameClearScene::Initialize()
{
	//�e�N�X�`��
	gameClearTexture = Texture::LoadTexture("gameclear.png");

	//�X�v���C�g
	gameClearSprite = new Sprite();
	gameClearSprite->Initialize(gameClearTexture);
}

void GameClearScene::Finalize()
{
	delete gameClearSprite;
}

void GameClearScene::Update()
{
	//�X�y�[�X�L�[�Ń^�C�g����
	if (Input::GetInstance()->IsKeyTrigger(DIK_SPACE) || Input::GetInstance()->IsPadTrigger(XINPUT_GAMEPAD_START))
	{
		//�V�[���̐؂�ւ����˗�
		sceneManager->ChangeScene("TITLE");
	}
}

void GameClearScene::Draw()
{
	Sprite::BeginDraw();

	gameClearSprite->Draw();

}
