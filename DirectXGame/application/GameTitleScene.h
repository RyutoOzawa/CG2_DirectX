#pragma once
#include"GameBaseScene.h"

#include"ImguiManager.h"
#include"Input.h"
#include"Sprite.h"

class GameTitleScene :public GameBaseScene
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


public://メンバ変数

	ImguiManager* imguiManager = nullptr;	//imgui用
	Input* input = nullptr;

	//ゲーム内で使用する変数まとめ

	uint32_t titleTexture = 0;	//背景画像(現在はダミー
	Sprite* titleSprite = nullptr;

};

