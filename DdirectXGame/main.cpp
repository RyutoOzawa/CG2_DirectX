
#include"MyGame.h"
#include"KEngineFramework.h"
#include"fbxsdk.h"


//Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	//フレームワークを基底クラスとしたゲームクラスを生成
	std::unique_ptr<KEngineFramework> game = std::make_unique< MyGame>();

	FbxManager* fbxManager = FbxManager::Create();

	//ゲームクラスの処理実行
	game->Run();

	return 0;
}

