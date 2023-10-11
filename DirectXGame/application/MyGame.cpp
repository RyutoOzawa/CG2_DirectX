#include "MyGame.h"
using namespace DirectX;
#include"SceneFactory.h"

MyGame::MyGame()
{
}

void MyGame::Initialize()
{
	//基底クラスの初期化処理
	KEngineFramework::Initialize();

	//シーンファクトリーの生成とマネージャへセット
	sceneFactory = std::make_unique< SceneFactory>();
	GameSceneManager::GetInstance()->SetSceneFactory(sceneFactory.get());
	//シーンマネージャに最初のシーンをセット
	GameSceneManager::GetInstance()->ChangeScene("TITLE",false);
}

void MyGame::Finalize()
{
	delete postEffect;


	// 基底クラスの終了処理
	KEngineFramework::Finalize();
}

void MyGame::Update()
{
	//imgui開始処理
	imguiManager->Begin();

	//基底クラスの更新処理
	KEngineFramework::Update();

	//imgui終了
	imguiManager->End();

	

}

void MyGame::Draw()
{

	//レンダーテクスチャへの描画
	postEffect->BeginDrawScene(directX->GetCommandList());
	sceneManager->Draw();
	postEffect->EndDrawScene(directX->GetCommandList());


	directX->BeginDraw();


	//ポストエフェクトの描画
	postEffect->Draw(directX->GetCommandList());

	//シーン描画処理
//	sceneManager->Draw();
	
#ifdef _DEBUG
	imguiManager->Draw();
#endif

	directX->EndDraw();
}
