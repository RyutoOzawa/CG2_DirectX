
#include"MyGame.h"
#include"KEngineFramework.h"



//Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	//フレームワークを基底クラスとしたゲームクラスを生成
	std::unique_ptr<KEngineFramework> game = std::make_unique< MyGame>();

	//ゲームクラスの処理実行
	game->Run();

	return 0;
}

