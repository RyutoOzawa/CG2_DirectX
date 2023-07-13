#include "GameTitleScene.h"
#include"Texture.h"
#include"DirectX.h"
using namespace DirectX;
#include"GameSceneManager.h"

void GameTitleScene::Initialize()
{


	//--------------�Q�[�����ϐ�������--------------//

	//input�̃C���X�^���X�擾
	input = Input::GetInstance();

	//�e�N�X�`���f�[�^������
	titleTexture = Texture::LoadTexture("dummyTitle.png");

	titleSprite = new Sprite();
	titleSprite->Initialize(titleTexture);

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
	if (input->IsKeyTrigger(DIK_SPACE) || input->IsPadTrigger(XINPUT_GAMEPAD_START))
	{
		//�V�[���̐؂�ւ����˗�
		sceneManager->ChangeScene("GAMEPLAY");
	}

	//----------------------�Q�[�������[�v�͂����܂�---------------------//


}

void GameTitleScene::Draw()
{
	//-------�X�v���C�g�`�揈��-------//
	Sprite::BeginDraw();

	titleSprite->Draw();

}
