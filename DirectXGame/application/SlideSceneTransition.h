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

	std::unique_ptr<Sprite> slideSprite = nullptr;
	TextureData* slideTex = nullptr;
	Vector2 spritePosOpen = { -WindowsAPI::winW,0.0f };
	Vector2 spritePosClose = { 0.0f,0.0f };

	EasingData easeSpritePos;

};

