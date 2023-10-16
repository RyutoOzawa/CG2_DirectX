#pragma once
//�V�[���J��(�X���C�h)
//�X�v���C�g���X���C�h�C���A�A�E�g���Ă���J��

#include"BaseSceneTransition.h"
#include"Sprite.h"
#include"EasingData.h"

class SlideSceneTransition :public BaseSceneTransition
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

	//�V�[������
	void Close()override;

	//�V�[���J����
	void Open()override;

private:

	std::unique_ptr<Sprite> slideSprite = nullptr;
	TextureData* slideTex = nullptr;
	Vector2 spritePosOpen = { -WindowsAPI::winW,0.0f };
	Vector2 spritePosClose = { 0.0f,0.0f };

	EasingData easeSpritePos;

};

