#include "KEngineFramework.h"
#include"FbxLoader.h"
#include"FbxObject3d.h"
#include"ParticleManager.h"

//KEngineFramework::KEngineFramework()
//{
//}

void KEngineFramework::Initialize()
{
	//windowsAPI初期化処理
	windowsAPI = new WindowsAPI();
	windowsAPI->Initialize();

	// DirectX初期化処理
	directX = new ReDirectX();
	directX->Initialize(windowsAPI);

	//キーボード初期化処理
	input =  Input::GetInstance();
	input->Initialize(windowsAPI);

	//テクスチャマネージャーの初期化
	Texture::Initialize(directX->GetDevice());

	//スプライトの初期化
	Sprite::StaticInitialize(directX->GetDevice(), directX->GetCommandList(), WindowsAPI::winW, WindowsAPI::winH);

	//3Dオブジェクトの初期化
	Object3d::StaticInitialize(directX);

	//カメラクラス初期化
	Camera::StaticInitialize(directX->GetDevice());

	//オーディオ初期化
	AudioManager::StaticInitialize();

	//デバッグテキスト(imgui初期化)
	imguiManager = ImguiManager::GetInstance();
	imguiManager->Initialize(windowsAPI,directX);

	//シーンマネージャの生成
	sceneManager = GameSceneManager::GetInstance();

	//FBX初期化
	FbxLoader::GetInstance()->Initialize(directX->GetDevice());

	//FbxObjectのデバイスとパイプライン生成
	FbxObject3d::SetDevice(directX->GetDevice());
	FbxObject3d::SetCmdList(directX->GetCommandList());
	FbxObject3d::CreateGraphicsPipeline();


	//ポストエフェクトの初期化
	postEffect = new PostEffect();
	postEffect->Initialize(directX->GetDevice());

	//パーティクルマネージャ初期化
	ParticleManager::StaticInitialize(directX);

}

void KEngineFramework::Finalize()
{


	imguiManager->Finalize();

	sceneManager->Finalize();

	FbxLoader::GetInstance()->Finalize();

	windowsAPI->Finalize();

	delete directX;
	delete windowsAPI;
}

void KEngineFramework::Update()
{
	//windowsのメッセージ処理
	if (windowsAPI->ProcessMessage()) {
		//ループを抜ける
		endRequest = true;
	}

	//入力系更新
	input->Update();

	//シーンマネージャの更新処理
	sceneManager->Update();
}

void KEngineFramework::Run()
{
	//初期化
	Initialize();

	//ゲームループ
	while (true)
	{
		//毎フレーム更新
		Update();
		if (IsEndRequest()) {
			break;
		}
		//描画

		Draw();
	}

	//終了処理
	Finalize();
}
