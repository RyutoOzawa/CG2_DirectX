#pragma once
//フェードインアウト
//シーン遷移(フェードイン、アウト)を行う

#include"BaseSceneTransition.h"
#include"Sprite.h"
#include"EasingData.h"

class FadeSceneTransition:public BaseSceneTransition
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

	std::unique_ptr<Sprite> blackSprite = nullptr;
	TextureData* whiteTex = nullptr;

	EasingData  easeSpriteAlpha;

};

