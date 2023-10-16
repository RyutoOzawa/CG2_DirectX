//シーン遷移(スライド)
//スプライトがスライドイン、アウトしてくる遷移

#include "SlideSceneTransition.h"
#include"Util.h"

void SlideSceneTransition::Initialize()
{
	//読み込みと初期化
	slideSprite = std::make_unique<Sprite>();
	slideTex = Texture::LoadTexture("syberBackGround.png");

	slideSprite->Initialize(slideTex);
	//サイズをウィンドウサイズに変更
	Vector2 window = { (float)WindowsAPI::winW, (float)WindowsAPI::winH };
	slideSprite->SetSize(window);
	//初期座標設定
	slideSprite->SetPos(spritePosOpen);

	isTransition = true;
}

void SlideSceneTransition::Finalize()
{
}

void SlideSceneTransition::Update()
{
	//シーン遷移中なら
	if (!isTransition) {
		return;
	}

	Vector2 spritePos{ 0,0 };
	switch (transitionPhase)
	{
	case TransitionPhase::Close:
		//イージング更新
		easeSpritePos.Update();
		//座標移動
		spritePos.x = Lerp(spritePosOpen.x, spritePosClose.x, easeSpritePos.GetTimeRate());
		//spritePos.x = Lerp(spritePosOpen.x, spritePosClose.x, easeSpirtePos.GetTimeRate());
		
		//イージング終わったらﾌｪｰｽﾞ変える
		if (easeSpritePos.GetTimeRate() >= 1.0f) {
			spritePos.x = spritePosClose.x;
			transitionPhase = TransitionPhase::Change;
		}

		break;

	case TransitionPhase::Change:

		//スライドの位置は固定
		spritePos = spritePosClose;

		//シーン状態をopenに
		Open();

		break;
	case TransitionPhase::Open:

		//イージング更新
		easeSpritePos.Update();
		//座標移動
		spritePos.x = Lerp(spritePosClose.x, spritePosOpen.x, easeSpritePos.GetTimeRate());
		//spritePos.x = Lerp(spritePosOpen.x, spritePosClose.x, easeSpirtePos.GetTimeRate());

		//イージング終わったらﾌｪｰｽﾞ変える
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
	//フェーズ変更
	transitionPhase = TransitionPhase::Close;
	easeSpritePos.Start(60.0f);
}

void SlideSceneTransition::Open()
{
	//ﾌｪｰｽﾞ変更
	transitionPhase = TransitionPhase::Open;
	//イージング開始
	easeSpritePos.Start(60.0f);

}
