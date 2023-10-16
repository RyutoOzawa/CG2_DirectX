//�V�[���J��(�X���C�h)
//�X�v���C�g���X���C�h�C���A�A�E�g���Ă���J��

#include "SlideSceneTransition.h"
#include"Util.h"

void SlideSceneTransition::Initialize()
{
	//�ǂݍ��݂Ə�����
	slideSprite = std::make_unique<Sprite>();
	slideTex = Texture::LoadTexture("syberBackGround.png");

	slideSprite->Initialize(slideTex);
	//�T�C�Y���E�B���h�E�T�C�Y�ɕύX
	Vector2 window = { (float)WindowsAPI::winW, (float)WindowsAPI::winH };
	slideSprite->SetSize(window);
	//�������W�ݒ�
	slideSprite->SetPos(spritePosOpen);

	isTransition = true;
}

void SlideSceneTransition::Finalize()
{
}

void SlideSceneTransition::Update()
{
	//�V�[���J�ڒ��Ȃ�
	if (!isTransition) {
		return;
	}

	Vector2 spritePos{ 0,0 };
	switch (transitionPhase)
	{
	case TransitionPhase::Close:
		//�C�[�W���O�X�V
		easeSpritePos.Update();
		//���W�ړ�
		spritePos.x = Lerp(spritePosOpen.x, spritePosClose.x, easeSpritePos.GetTimeRate());
		//spritePos.x = Lerp(spritePosOpen.x, spritePosClose.x, easeSpirtePos.GetTimeRate());
		
		//�C�[�W���O�I�������̪��ޕς���
		if (easeSpritePos.GetTimeRate() >= 1.0f) {
			spritePos.x = spritePosClose.x;
			transitionPhase = TransitionPhase::Change;
		}

		break;

	case TransitionPhase::Change:

		//�X���C�h�̈ʒu�͌Œ�
		spritePos = spritePosClose;

		//�V�[����Ԃ�open��
		Open();

		break;
	case TransitionPhase::Open:

		//�C�[�W���O�X�V
		easeSpritePos.Update();
		//���W�ړ�
		spritePos.x = Lerp(spritePosClose.x, spritePosOpen.x, easeSpritePos.GetTimeRate());
		//spritePos.x = Lerp(spritePosOpen.x, spritePosClose.x, easeSpirtePos.GetTimeRate());

		//�C�[�W���O�I�������̪��ޕς���
		if (easeSpritePos.GetTimeRate() >= 1.0f) {
			spritePos.x = spritePosOpen.x;
			isTransition = false;
		}


		break;

	default:
		break;
	}

	slideSprite->SetPos(spritePos);

}

void SlideSceneTransition::Draw()
{
	Sprite::BeginDraw();

	slideSprite->Draw();
}

void SlideSceneTransition::Close()
{
	//�t�F�[�Y�ύX
	transitionPhase = TransitionPhase::Close;
	easeSpritePos.Start(60.0f);
}

void SlideSceneTransition::Open()
{
	//̪��ޕύX
	transitionPhase = TransitionPhase::Open;
	//�C�[�W���O�J�n
	easeSpritePos.Start(60.0f);

}
