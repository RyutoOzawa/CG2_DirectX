#pragma once
#include"GameBaseScene.h"

#include"ImguiManager.h"
#include"Input.h"
#include"Sprite.h"
#include<memory>
#include"Model.h"
#include"Object3d.h"
#include"Camera.h"
#include"EasingData.h"

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


private://メンバ変数

	ImguiManager* imguiManager = nullptr;	//imgui用
	Input* input = nullptr;



	//ゲーム内で使用する変数まとめ
	std::unique_ptr<Camera> camera = nullptr;

	TextureData* titleTexture = nullptr;	//背景画像(現在はダミー
	Sprite* titleSprite = nullptr;

	TextureData* texTextTitleLogo = nullptr;
	std::unique_ptr<Sprite> spTextTitleLogo = nullptr;

	TextureData* texTextPressA = nullptr;
	std::unique_ptr<Sprite> spTextPressA = nullptr;

	std::unique_ptr<Model> skydome = nullptr;
	std::unique_ptr<Object3d> skydomeObj = nullptr;

	std::unique_ptr<Model> enemyModel = nullptr;
	static const uint8_t enemyMax = 10;
	std::unique_ptr<Object3d> enemysObj[enemyMax];

	float textColorRad = 0.0f;

	const uint16_t logoRotTimeMax = 300;
	uint16_t logoRotTimer = logoRotTimeMax;
	EasingData easeLogoRot;
};

