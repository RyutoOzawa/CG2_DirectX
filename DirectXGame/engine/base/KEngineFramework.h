#pragma once
//ゲームエンジンのフレームワーク
//ゲームに最低限必要な変数をもち、基盤システムの初期化等を行う基底クラス

#include"WindowsAPI.h"
#include"DirectX.h"
#include"Input.h"
#include"Texture.h"
#include"Sprite.h"
#include"Object3d.h"
#include"Camera.h"
#include"ImguiManager.h"
#include"AudioManager.h"
#include"GameSceneManager.h"
#include"SceneFactory.h"
#include"PostEffect.h"
#include<memory>

class KEngineFramework
{
public:
	//処理の全実行
	void Run();

	virtual ~KEngineFramework() = default;

	//初期化
	virtual void Initialize();

	//終了
	virtual void Finalize();

	//毎フレーム更新
	virtual void Update();

	//描画
	virtual void Draw() = 0;

	//終了チェック
	virtual bool IsEndRequest() { return endRequest; }

	bool endRequest = false;
	WindowsAPI* windowsAPI = nullptr;
	ReDirectX* directX = nullptr;
	Input* input = nullptr;
	ImguiManager* imguiManager = nullptr;
	PostEffect* postEffect = nullptr;


protected:
	GameSceneManager* sceneManager = nullptr;
	//シーンファクトリー
	std::unique_ptr<AbstractSceneFactory> sceneFactory = nullptr;
};

