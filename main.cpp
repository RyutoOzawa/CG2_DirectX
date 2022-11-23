#include <DirectXMath.h>
using namespace DirectX;
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")
#include"Input.h"
#include<DirectXTex.h>
//#include"WindowsAPI.h"
//#include"DirectX.h"
#include"Object3d.h"
#include"Util.h"
#include"Texture.h"
#include"GpPipeline.h"
#include"Gridline.h"
#include<string>
#include"SpriteManager.h"
#include"Sprite.h"
#include"Material.h"
#include "GameScene.h"

using namespace Microsoft::WRL;


//Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	HRESULT result{};
#pragma region 基盤システム初期化
	//windowsAPI初期化処理
	WindowsAPI* windowsAPI = new WindowsAPI();
	windowsAPI->Initialize();

	// DirectX初期化処理
	ReDirectX* directX = new ReDirectX();
	directX->Initialize(windowsAPI);

	//キーボード初期化処理
	Input* input = new Input();
	input->Initialize(windowsAPI);

	//テクスチャマネージャーの初期化
	Texture::Initialize(directX->GetDevice());

	//スプライト共通部の初期化
	SpriteManager* spriteManager = nullptr;
	spriteManager = new SpriteManager;
	spriteManager->Initialize(directX,WindowsAPI::winW,WindowsAPI::winH);

	//3Dオブジェクトの初期化
	Object3d::StaticInitialize(directX);


#pragma endregion 基盤システム初期化

#pragma region 描画初期化処理
	WorldTransform w;
	w.StaticInitialize(directX);
	w.Initialize();

	WorldTransform objPos;
	objPos.Initialize();
	objPos.translation_ = { 0, 0, 0 };
	objPos.scale_ = { 1,1,1 };
	objPos.TransferMatrix();

	ViewProjection view;
	view.StaticInitialize(directX);
	view.Initialize();

	//画像読み込み
	uint32_t marioGraph = Texture::LoadTexture(L"Resources/mario.jpg");
	uint32_t reimuGraph = Texture::LoadTexture(L"Resources/reimu.png");

	//スプライト一枚の初期化
	Sprite* sprite = new Sprite();
	sprite->Initialize(spriteManager,marioGraph);

	Sprite* sprite2 = new Sprite();
	sprite2->Initialize(spriteManager,reimuGraph);
	//sprite2->SetTextureNum(1);

	Object3d object1;
	object1.Initialize("skydome");

	Object3d obj2;
	obj2.Initialize("Medama");

	GameScene* gameScene = nullptr;
	gameScene = new GameScene;
	gameScene->Initialize(spriteManager,windowsAPI);


#pragma endregion 描画初期化処理
	// ゲームループ
	while (true) {

#pragma region 基盤システム初期化
		//windowsのメッセージ処理
		if (windowsAPI->ProcessMessage()) {
			//ループを抜ける
			break;
		}
		input->Update();
#pragma endregion 基盤システム初期化
#pragma region シーン更新処理
		gameScene->Update();
	

#pragma endregion シーン更新処理

		//描画前処理
		directX->BeginDraw();
#pragma region シーン描画処理

		//3Dオブジェクト描画処理
		Object3d::BeginDraw();
		gameScene->ModelDraw();

		//スプライト描画処理
		spriteManager->beginDraw();
		gameScene->FrontSpriteDraw();

#pragma endregion シーン描画処理
		// ４．描画コマンドここまで
		directX->EndDraw();
		// DirectX毎フレーム処理 ここまで
	}
#pragma region シーン終了処理
	//WindowsAPI終了処理
	windowsAPI->Finalize();

	delete sprite;

	delete gameScene;
	delete windowsAPI;
	delete input;
	delete directX;
	delete spriteManager;

#pragma endregion シーン終了処理

	return 0;
}


