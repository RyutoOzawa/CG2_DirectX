//シーン遷移(スライド)
//スプライトがスライドイン、アウトしてくる遷移

#include "SlideSceneTransition.h"
#include"Util.h"

void SlideSceneTransition::Initialize()
{
	//読み込みと初期化
	slideSprite = std::make_unique<Sprite>();
	titleLogoSprite = std::make_unique<Sprite>();
	slideTex = Texture::LoadTexture("syberBackGround.png");
	titleLogoTex = Texture::LoadTexture("titleLogo.png");

	//スライドの画像
	slideSprite->Initialize(slideTex);
	//サイズをウィンドウサイズに変更
	Vector2 window = { (float)WindowsAPI::winW, (float)WindowsAPI::winH };
	slideSprite->SetSize(window);
	//初期座標設定
	slideSprite->SetPos(slidePosOpen);

	//タイトルロゴ設定
	titleLogoSprite->Initialize(titleLogoTex);
	titleLogoSprite->SetAnchorPoint({ 0.5f,0.5f });
	titleLogoSprite->SetPos(logoPosTop);

	//遷移開始
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

	Vector2 slidePos{ 0,0 };
	switch (transitionPhase)
	{
	case TransitionPhase::Close:
		//イージング更新
		easeSpritePos.Update();
		//座標移動
		slidePos.x = Lerp(slidePosOpen.x, slidePosClose.x, EaseOut(easeSpritePos.GetTimeRate()));
		//spritePos.x = Lerp(spritePosOpen.x, spritePosClose.x, easeSpirtePos.GetTimeRate());

		//イージング終わったらﾌｪｰｽﾞ変える
		if (easeSpritePos.GetTimeRate() >= 1.0f) {
			slidePos.x = slidePosClose.x;
			transitionPhase = TransitionPhase::Change;
			//ロゴを落とすイージング開始
			easeLogoPos.Start(60.0f);
			isLogoFall = true;
			//画面シェイク開始
			easeCameraShake.Start(30.0f);
		}

		break;

	case TransitionPhase::Change:

		//スライドの位置は固定
		slidePos = slidePosClose;


		easeLogoPos.Update();
		//イージング終了時、ロゴ降下がtrueならfalseにして再イージング
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
				//falseならシーン開ける
				Open();
			}
		}
		else {

			//ロゴ降下フラグでlerpの変数と速度変える
			Vector2 before, after;
			before = logoPosBottom;
			after = logoPosTop;
			float t = Out(easeLogoPos.GetTimeRate());
			if (isLogoFall) {
				before = logoPosTop;
				after = logoPosBottom;
				t = OutBounce(easeLogoPos.GetTimeRate());
			}

			//ロゴ移動
			Vector2 logoPos;
			logoPos.x = Lerp(before.x, after.x, t);
			logoPos.y = Lerp(before.y, after.y, t);

			//スプライトにセット
			titleLogoSprite->SetPos(logoPos);
		}

		//カメラシェイク
		easeCameraShake.Update();
		//振動幅を調節
		absShake.x = Lerp(absShakeMax.x,0.0f , Out(easeCameraShake.GetTimeRate()));
		absShake.y = Lerp( absShakeMax.y,0.0f, Out(easeCameraShake.GetTimeRate()));
		//カメラの振れ幅を設定
		cameraOffset.x = Random(-absShake.x, absShake.x);
		cameraOffset.y = Random(-absShake.y, absShake.y);

		break;
	case TransitionPhase::Open:

		//イージング更新
		easeSpritePos.Update();
		//座標移動
		slidePos.x = Lerp(slidePosClose.x, slidePosOpen.x, In(easeSpritePos.GetTimeRate()));
		//spritePos.x = Lerp(spritePosOpen.x, spritePosClose.x, easeSpirtePos.GetTimeRate());

		//イージング終わったらﾌｪｰｽﾞ変える
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
