#pragma once
#include"GameBaseScene.h"

#include"ImguiManager.h"
#include"Input.h"
#include"Sprite.h"
#include"Camera.h"
#include"Object3d.h"
#include"AudioManager.h"

#include<memory>

class GamePlayScene :public GameBaseScene
{
public:
	//初期化
	void Initialize()override;

	//終了
	void Finalize()override;

	//更新
	void Update()override;

	//描画
	void Draw()override;


public://メンバ変数

	Input* input = nullptr;

	uint32_t backGroundTexture = 0;
	uint32_t marioGraph = 0;
	uint32_t reimuGraph = 0;

	std::unique_ptr<Sprite> backGroundSprite = nullptr;
	std::unique_ptr<Sprite> sprite = nullptr;
	std::unique_ptr<Sprite> sprite2 = nullptr;
	std::unique_ptr<Model> skydome = nullptr;
	std::unique_ptr<Object3d> skydomeObj = nullptr;

	std::unique_ptr<AudioManager> newAudio = nullptr;

	Camera camera;

};

