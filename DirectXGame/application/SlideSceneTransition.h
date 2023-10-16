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

	//�X���C�h�̃X�v���C�g�n
	std::unique_ptr<Sprite> slideSprite = nullptr;
	TextureData* slideTex = nullptr;
	Vector2 slidePosOpen = { -WindowsAPI::winW,0.0f };
	Vector2 slidePosClose = { 0.0f,0.0f };

	EasingData easeSpritePos;

	//�^�C�g�����S�֌W
	std::unique_ptr<Sprite> titleLogoSprite = nullptr;
	TextureData* titleLogoTex = nullptr;
	Vector2 logoPosTop = { (float)WindowsAPI::winW / 2.0f,-160.0f };
	Vector2 logoPosBottom = { logoPosTop.x,WindowsAPI::winH / 2.0f };
	EasingData easeLogoPos;
	bool isLogoFall = false;//���S���~�����邩
	const uint16_t easeCooltimeMax = 60;
	uint16_t easeCooltime = easeCooltimeMax;

};

