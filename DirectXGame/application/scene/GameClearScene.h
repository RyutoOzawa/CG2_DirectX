#pragma once
//�Q�[���N���A�V�[���N���X
//�Q�[�����N���A�����Ƃ��Ɉȍ~����V�[���̏������s��

#include"GameBaseScene.h"
#include"Sprite.h"
#include"Model.h"
#include"Object3d.h"

class GameClearScene : public GameBaseScene
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

	TextureData* gameClearTexture = 0;
	Sprite* gameClearSprite = nullptr;

	std::unique_ptr<Camera> camera = nullptr;

	std::unique_ptr<Model> skydome = nullptr;
	std::unique_ptr<Object3d> skydomeObj = nullptr;

	const uint16_t nextSceneTimerMax = 120;
	uint16_t nextSceneTimer = 0;
};

