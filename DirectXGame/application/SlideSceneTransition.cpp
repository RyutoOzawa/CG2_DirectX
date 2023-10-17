//�V�[���J��(�X���C�h)
//�X�v���C�g���X���C�h�C���A�A�E�g���Ă���J��

#include "SlideSceneTransition.h"
#include"Util.h"

void SlideSceneTransition::Initialize()
{
	//�ǂݍ��݂Ə�����
	slideSprite = std::make_unique<Sprite>();
	titleLogoSprite = std::make_unique<Sprite>();
	slideTex = Texture::LoadTexture("syberBackGround.png");
	titleLogoTex = Texture::LoadTexture("titleLogo.png");

	//�X���C�h�̉摜
	slideSprite->Initialize(slideTex);
	//�T�C�Y���E�B���h�E�T�C�Y�ɕύX
	Vector2 window = { (float)WindowsAPI::winW, (float)WindowsAPI::winH };
	slideSprite->SetSize(window);
	//�������W�ݒ�
	slideSprite->SetPos(slidePosOpen);

	//�^�C�g�����S�ݒ�
	titleLogoSprite->Initialize(titleLogoTex);
	titleLogoSprite->SetAnchorPoint({ 0.5f,0.5f });
	titleLogoSprite->SetPos(logoPosTop);

	//�J�ڊJ�n
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

	Vector2 slidePos{ 0,0 };
	switch (transitionPhase)
	{
	case TransitionPhase::Close:
		//�C�[�W���O�X�V
		easeSpritePos.Update();
		//���W�ړ�
		slidePos.x = Lerp(slidePosOpen.x, slidePosClose.x, EaseOut(easeSpritePos.GetTimeRate()));
		//spritePos.x = Lerp(spritePosOpen.x, spritePosClose.x, easeSpirtePos.GetTimeRate());

		//�C�[�W���O�I�������̪��ޕς���
		if (easeSpritePos.GetTimeRate() >= 1.0f) {
			slidePos.x = slidePosClose.x;
			transitionPhase = TransitionPhase::Change;
			//���S�𗎂Ƃ��C�[�W���O�J�n
			easeLogoPos.Start(60.0f);
			isLogoFall = true;
			//��ʃV�F�C�N�J�n
			easeCameraShake.Start(30.0f);
		}

		break;

	case TransitionPhase::Change:

		//�X���C�h�̈ʒu�͌Œ�
		slidePos = slidePosClose;


		easeLogoPos.Update();
		//�C�[�W���O�I�����A���S�~����true�Ȃ�false�ɂ��čăC�[�W���O
		if (easeLogoPos.GetTimeRate() >= 1.0f) {
			if (isLogoFall) {
				if (easeCooltime > 0) {
					easeCooltime--;
				}
				else {
					easeCooltime = easeCooltimeMax;

					isLogoFall = false;
					easeLogoPos.Start(30.0f);
				}
			}
			else {
				//false�Ȃ�V�[���J����
				Open();
			}
		}
		else {

			//���S�~���t���O��lerp�̕ϐ��Ƒ��x�ς���
			Vector2 before, after;
			before = logoPosBottom;
			after = logoPosTop;
			float t = Out(easeLogoPos.GetTimeRate());
			if (isLogoFall) {
				before = logoPosTop;
				after = logoPosBottom;
				t = OutBounce(easeLogoPos.GetTimeRate());
			}

			//���S�ړ�
			Vector2 logoPos;
			logoPos.x = Lerp(before.x, after.x, t);
			logoPos.y = Lerp(before.y, after.y, t);

			//�X�v���C�g�ɃZ�b�g
			titleLogoSprite->SetPos(logoPos);
		}

		//�J�����V�F�C�N
		easeCameraShake.Update();
		//�U�����𒲐�
		absShake.x = Lerp(absShakeMax.x,0.0f , Out(easeCameraShake.GetTimeRate()));
		absShake.y = Lerp( absShakeMax.y,0.0f, Out(easeCameraShake.GetTimeRate()));
		//�J�����̐U�ꕝ��ݒ�
		cameraOffset.x = Random(-absShake.x, absShake.x);
		cameraOffset.y = Random(-absShake.y, absShake.y);

		break;
	case TransitionPhase::Open:

		//�C�[�W���O�X�V
		easeSpritePos.Update();
		//���W�ړ�
		slidePos.x = Lerp(slidePosClose.x, slidePosOpen.x, In(easeSpritePos.GetTimeRate()));
		//spritePos.x = Lerp(spritePosOpen.x, spritePosClose.x, easeSpirtePos.GetTimeRate());

		//�C�[�W���O�I�������̪��ޕς���
		if (easeSpritePos.GetTimeRate() >= 1.0f) {
			slidePos.x = slidePosOpen.x;
			isTransition = false;
		}


		break;

	default:
		break;
	}

	slideSprite->SetPos(slidePos);

}

void SlideSceneTransition::Draw()
{
	Sprite::BeginDraw(cameraOffset);

	slideSprite->Draw();
	titleLogoSprite->Draw();
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
