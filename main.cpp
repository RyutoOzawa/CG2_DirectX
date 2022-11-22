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

	//ランダムな数値を取得
	float randValue = Random(-100, 100);

	const size_t kObjCount = 50;
	Object3d obj[kObjCount];

	Object3d object;

	XMMATRIX matProjection;
	XMMATRIX matView;
	XMFLOAT3 eye(0, 0, 10);	//視点座標
	XMFLOAT3 target(0, 0, 0);	//注視点座標
	XMFLOAT3 up(0, 1, 0);		//上方向ベクトル
	XMFLOAT3 scale = { 1.0f,1.0f,1.0f };
	XMFLOAT3 rotation = { 0.0f,0.0f,0.0f };
	XMFLOAT3 position = { 0.0f,0.0f,0.0f };
	XMFLOAT4 color_ = { 1.0f,1.0f,1.0f,1.0f };

	XMFLOAT3 pos{};


	//透視東映返還行列の計算
	//専用の行列を宣言
	matProjection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(45.0f),					//上下画角45度
		(float)WindowsAPI::winW / WindowsAPI::winH,	//アスペクト比（画面横幅/画面縦幅）
		0.1f, 1000.0f								//前橋、奥橋
	);

		//ビュー変換行列の計算
	matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));

	//w.TransferMatrix();
	//view.UpdateMatrix();

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

		sprite->SetPos({ 100, 100 });
		sprite2->SetPos({ WindowsAPI::winW/2,WindowsAPI::winH/2 });
		sprite->SetSize({ 64,64 });

		if (input->IsPress(DIK_A)) {
			objPos.rotation_.y += 0.1f;
		}
		else if (input->IsPress(DIK_D)) {
			objPos.rotation_.y -= 0.1f;
		}
		if (input->IsTrigger(DIK_W)) {
			objPos.translation_.x += 0.1f;
		}
		else if (input->IsTrigger(DIK_S)) {
			objPos.translation_.x -= 0.1f;
		}

		//view.eye = Vector3(0,0,-100)
		objPos.TransferMatrix();

#pragma endregion シーン更新処理

		//描画前処理
		directX->BeginDraw();
#pragma region シーン描画処理

		//3Dオブジェクト描画処理
		Object3d::BeginDraw();
		//object1.Draw(w,view);
		obj2.Draw(objPos, view);

		//スプライト描画処理
		spriteManager->beginDraw();
		sprite->Draw();
		//sprite2->Draw();

#pragma endregion シーン描画処理
		// ４．描画コマンドここまで
		directX->EndDraw();
		// DirectX毎フレーム処理 ここまで
	}
#pragma region シーン終了処理
	//WindowsAPI終了処理
	windowsAPI->Finalize();

	delete sprite;


	delete windowsAPI;
	delete input;
	delete directX;
	delete spriteManager;

#pragma endregion シーン終了処理

	return 0;
}


