//�t�F�[�h�C���A�E�g
//�V�[���J��(�t�F�[�h�C���A�A�E�g)���s��

#include "FadeSceneTransition.h"
#include"Util.h"
#include"ImguiManager.h"
using namespace Utility;


void FadeSceneTransition::Initialize()
{
	blackSprite = std::make_unique<Sprite>();
	whiteTex = Texture::LoadTexture("black1x1.png");

	blackSprite->Initialize(whiteTex);
	//�X�v���C�g��rgb��0�ɂ��č��摜�ɂ���
	Vector4 black = { 0,0,0,1 };
	//blackSprite->SetColor(black);
	Vector2 size = { WindowsAPI::winW,WindowsAPI::winH };
	blackSprite->SetSize(size);
	blackSprite->SetAnchorPoint({ 0.5f,0.5f });
	Vector2 pos = size;
	pos /= 2.0f;
	blackSprite->SetPos(pos);

	isTransition = true;
}

void FadeSceneTransition::Finalize()
{
}

void FadeSceneTransition::Update()
{
	//�V�[���J�ڒ��Ȃ�
	if (!isTransition) {
		return;
	}
	float alpha = 0.0f;
	Vector4 c;

	switch (transitionPhase)
	{
	case TransitionPhase::Close:
		//�C�[�W���O�f�[�^�X�V
		easeSpriteAlpha.Update();
		//�X�v���C�g�̃A���t�@�l��0��1��
		alpha = Lerp(0.0f, 1.0f, easeSpriteAlpha.GetTimeRate());



		//�C�[�W���O�I�������t�F�[�Y��change��
		if (easeSpriteAlpha.GetTimeRate() >= 1.0f) {
			alpha = 1.0f;
			transitionPhase = TransitionPhase::Change;
		}

		//�ݒ肳�ꂽ�F���Z�b�g
		c = blackSprite->GetColor();
		c.w = alpha;
		blackSprite->SetColor(c);

		break;
	case TransitionPhase::Change:

		c = blackSprite->GetColor();
		c.w = 1.0f;
		blackSprite->SetColor(c);

		//�V�[����Ԃ�open��
		transitionPhase = TransitionPhase::Open;

		Open();
		break;
	case TransitionPhase::Open:

		//�C�[�W���O�f�[�^�X�V
		easeSpriteAlpha.Update();
		//�A���t�@�l��1��0
		alpha = Lerp(1.0f, 0.0f, easeSpriteAlpha.GetTimeRate());



		//�C�[�W���O�I�������Ǘ��t���O���낷
		if (easeSpriteAlpha.GetTimeRate() >= 1.0f) {
			alpha = 0.0f;
			isTransition = false;
		}


		c = blackSprite->GetColor();
		c.w = alpha;
		blackSprite->SetColor(c);

		break;
	default:
		break;
	}

	ImGui::Text("alpha %f", alpha);

}

void FadeSceneTransition::Draw()
{
	Sprite::BeginDraw();

	blackSprite->Draw();
}

void FadeSceneTransition::Close()
{
	//�t�F�[�Y�ύX
	transitionPhase = TransitionPhase::Close;
	//�C�[�W���O�J�n
	easeSpriteAlpha.Start(60.0f);

}

void FadeSceneTransition::Open()
{	//�t�F�[�Y�ύX
	transitionPhase = TransitionPhase::Open;
	//�C�[�W���O�J�n
	easeSpriteAlpha.Start(60.0f);
}
