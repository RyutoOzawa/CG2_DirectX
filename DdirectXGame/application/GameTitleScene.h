#pragma once
#include"GameBaseScene.h"

#include"ImguiManager.h"
#include"Input.h"
#include"Sprite.h"

class GameTitleScene :public GameBaseScene
{
public:
	//������
	void Initialize() override;

	//�I��
	void Finalize()override;

	//�X�V
	void Update()override;

	//�`��
	void Draw()override;


public://�����o�ϐ�

	ImguiManager* imguiManager = nullptr;	//imgui�p
	Input* input = nullptr;

	//�Q�[�����Ŏg�p����ϐ��܂Ƃ�

	uint32_t titleTexture = 0;	//�w�i�摜(���݂̓_�~�[
	Sprite* titleSprite = nullptr;

};

