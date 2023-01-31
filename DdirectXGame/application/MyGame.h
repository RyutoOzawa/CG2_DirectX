#pragma once
#include"KEngineFramework.h"
#include"GamePlayScene.h"
#include"GameTitleScene.h"


class MyGame : public KEngineFramework
{
public://メンバ関数

	MyGame();

	//初期化
	void Initialize() override;

	//終了
	void Finalize()override;

	//毎フレーム更新
	void Update()override;

	//描画
	void Draw()override;

public://メンバ関数

};

