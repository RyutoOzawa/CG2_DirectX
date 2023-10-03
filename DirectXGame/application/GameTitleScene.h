#pragma once
#include"GameBaseScene.h"

#include"ImguiManager.h"
#include"Input.h"
#include"Sprite.h"
#include<memory>
#include"Model.h"
#include"Object3d.h"
#include"Camera.h"

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
	std::unique_ptr<Camera> camera = nullptr;

	TextureData* titleTexture = nullptr;	//�w�i�摜(���݂̓_�~�[
	Sprite* titleSprite = nullptr;

	TextureData* texTextTitleLogo = nullptr;
	std::unique_ptr<Sprite> spTextTitleLogo = nullptr;

	TextureData* texTextPressA = nullptr;
	std::unique_ptr<Sprite> spTextPressA = nullptr;

	std::unique_ptr<Model> skydome = nullptr;
	std::unique_ptr<Object3d> skydomeObj = nullptr;


	float textColorRad = 0.0f;
};

