#pragma once
//シーン遷移(スライド)
//スプライトがスライドイン、アウトしてくる遷移

#include"BaseSceneTransition.h"
#include"Sprite.h"
#include"EasingData.h"

class SlideSceneTransition :public BaseSceneTransition
{
public:

	//初期化
	void Initialize() override;

	//終了
	void Finalize()override;

	//更新
	void Update()override;

	//描画
	void Draw()override;

	//シーン閉じる
	void Close()override;

	//シーン開ける
	void Open()override;

private:

	//スライドのスプライト系
	std::unique_ptr<Sprite> slideSprite = nullptr;
	TextureData* slideTex = nullptr;
	Vector2 slidePosOpen = { -WindowsAPI::winW,0.0f };
	Vector2 slidePosClose = { 0.0f,0.0f };

	Easing::EasingData easeSpritePos;

	//タイトルロゴ関係
	std::unique_ptr<Sprite> titleLogoSprite = nullptr;
	TextureData* titleLogoTex = nullptr;
	Vector2 logoPosTop = { (float)WindowsAPI::winW / 2.0f,-160.0f };
	Vector2 logoPosBottom = { logoPosTop.x,WindowsAPI::winH / 2.0f };
	Easing::EasingData easeLogoPos;
	bool isLogoFall = false;//ロゴが降下するか
	const uint16_t easeCooltimeMax = 60;
	uint16_t easeCooltime = easeCooltimeMax;

	//画面シェイク関係
	const Vector2 absShakeMax = { 64.0f,64.0f };
	Vector2 absShake{ 0.0f,0.0f };
	Easing::EasingData easeCameraShake;
	Vector2 cameraOffset{ 0.0f,0.0f };
};

