//フェードインアウト
//シーン遷移(フェードイン、アウト)を行う

#include "FadeSceneTransition.h"
#include"Util.h"
#include"ImguiManager.h"
using namespace Utility;


void FadeSceneTransition::Initialize()
{
	blackSprite = std::make_unique<Sprite>();
	whiteTex = Texture::LoadTexture("black1x1.png");

	blackSprite->Initialize(whiteTex);
	//スプライトのrgbを0にして黒画像にする
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
	//シーン遷移中なら
	if (!isTransition) {
		return;
	}
	float alpha = 0.0f;
	Vector4 c;

	switch (transitionPhase)
	{
	case TransitionPhase::Close:
		//イージングデータ更新
		easeSpriteAlpha.Update();
		//スプライトのアルファ値を0→1に
		alpha = Lerp(0.0f, 1.0f, easeSpriteAlpha.GetTimeRate());



		//イージング終わったらフェーズをchangeに
		if (easeSpriteAlpha.GetTimeRate() >= 1.0f) {
			alpha = 1.0f;
			transitionPhase = TransitionPhase::Change;
		}

		//設定された色をセット
		c = blackSprite->GetColor();
		c.w = alpha;
		blackSprite->SetColor(c);

		break;
	case TransitionPhase::Change:

		c = blackSprite->GetColor();
		c.w = 1.0f;
		blackSprite->SetColor(c);

		//シーン状態をopenに
		transitionPhase = TransitionPhase::Open;

		Open();
		break;
	case TransitionPhase::Open:

		//イージングデータ更新
		easeSpriteAlpha.Update();
		//アルファ値は1→0
		alpha = Lerp(1.0f, 0.0f, easeSpriteAlpha.GetTimeRate());



		//イージング終わったら管理フラグおろす
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
	//フェーズ変更
	transitionPhase = TransitionPhase::Close;
	//イージング開始
	easeSpriteAlpha.Start(60.0f);

}

void FadeSceneTransition::Open()
{	//フェーズ変更
	transitionPhase = TransitionPhase::Open;
	//イージング開始
	easeSpriteAlpha.Start(60.0f);
}
