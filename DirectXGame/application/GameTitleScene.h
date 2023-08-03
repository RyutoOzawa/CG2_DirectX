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


private://�����o�ϐ�

	ImguiManager* imguiManager = nullptr;	//imgui�p
	Input* input = nullptr;

	//�Q�[�����Ŏg�p����ϐ��܂Ƃ�

	TextureData* titleTexture = nullptr;	//�w�i�摜(���݂̓_�~�[
	Sprite* titleSprite = nullptr;

};

