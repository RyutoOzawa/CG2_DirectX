#pragma once
//�Q�[���I�[�o�[�V�[���N���X
//�Q�[���I�[�o�[�ɂȂ����ۂɈȍ~�����ʂ̏������s��

#include"GameBaseScene.h"
#include"Sprite.h"

class GameOverScene : public GameBaseScene
{
public:
	//������
	void Initialize() override;

	//�I��
	void Finalize() override;

	//�X�V
	void Update() override;

	//�`��
	void Draw() override;

private:

	TextureData* gameoverTexture = 0;
	Sprite* gameoverSprite = nullptr;

const uint16_t nextSceneTimerMax = 120;
	uint16_t nextSceneTimer = 0;
};

