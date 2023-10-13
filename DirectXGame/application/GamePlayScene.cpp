//ゲームシーンクラス
//ゲーム本編の処理を行う

#include "GamePlayScene.h"
#include"Texture.h"
#include"DirectX.h"
#include"GameSceneManager.h"
#include"Collision.h"
#include"FbxLoader.h"
#include"FbxObject3d.h"
#include"Util.h"
#include"SphereCollider.h"
#include"DebugLine.h"

using namespace DirectX;

void GamePlayScene::Initialize()
{


	//--------------ゲーム内変数初期化--------------//

	//inputのインスタンス取得
	input = Input::GetInstance();

	//衝突マネージャにインスタンス取得
	collisionManager = CollisionManager::GetInstance();


	//テクスチャデータ初期化

	backGroundTexture = Texture::LoadTexture("dummyPlayGame.png");
	marioGraph = Texture::LoadTexture("mario.jpg");
	reimuGraph = Texture::LoadTexture("reimu.png");
	particleGraph = Texture::LoadTexture("particle.png");
	reticleGraph = Texture::LoadTexture("reticle.png");
	whiteGraph = Texture::LoadTexture("white1x1.png");
	backGroundSprite = std::make_unique<Sprite>();
	backGroundSprite->Initialize(backGroundTexture);

	//画像重複テスト
	testTex1 = Texture::LoadTexture("mario.jpg");
	testTex2 = Texture::LoadTexture("mario.jpg");
	testTex3 = Texture::LoadTexture("mario.jpg");

	static int a = 0;
	a++;

	skydome = std::make_unique<Model>();
	skydome = Model::CreateModel("skydome");

	defaultModel = std::make_unique<Model>();
	defaultModel = Model::CreateModel();
	defaultModel->SetTexture(reimuGraph);

	playerModel = std::make_unique<Model>();
	playerModel = Model::CreateModel("Player");

	playerBulletModel = std::make_unique<Model>();
	playerBulletModel = Model::CreateModel("PlayerBullet");

	enemyModel = std::make_unique<Model>();
	enemyModel = Model::CreateModel("ZakoEnemy");

	Enemy::EnemyInitialize(particleGraph, enemyModel.get(), playerBulletModel.get());

	//カメラ初期化
	Vector3 eye(0, 20, -20);	//視点座標
	Vector3 target(0, 0, 6);	//注視点座標
	Vector3 up(0, 1, 0);		//上方向ベクトル

	currentCamera = new Camera();
	currentCamera->Initialize(eye, target, up);

	skydomeObj = std::make_unique<Object3d>();
	skydomeObj->Initialize();
	skydomeObj->SetModel(skydome.get());
	skydomeObj->scale = { 1000,1000,1000 };


	player = std::make_unique<Player>();
	player->Initialize(playerModel.get(), reticleGraph, whiteGraph);
	player->SetBulletModel(playerBulletModel.get());

	//当たり判定テスト用オブジェクト
	colTestObj = std::make_unique<Object3d>();
	colTestObj->Initialize();
	colTestObj->SetModel(defaultModel.get());
	//colTestObj->SetCollider(new SphereCollider({ 0,0,0 }, 1.0f));
	colTestObj->Update();


	newAudio = std::make_unique<AudioManager>();
	newAudio->SoundLoadWave("Resources/bgm_title.wav");

	//モデル名を指定してファイル読み込み
	//model1 = std::make_unique<FbxModel>();
	model1 = FbxLoader::GetInstance()->LoadModelFromFile("boneTest");

	object1 = std::make_unique<FbxObject3d>();
	object1->Initialize();
	object1->SetModel(model1.get());

	//デバイスセット
	FbxObject3d::SetCamera(currentCamera);


	currentCamera->target = { 0,0,0 };
	//	camera->eye = { 0,0,-20 };
	currentCamera->eye = { 0,0,-50 };


	railCamera = new RailCamera();
	railCamera->Initialize({ 0,0,0 }, { 0,0,0 });

	//obj3dクラスにカメラをセット
	Object3d::SetCamera(railCamera->GetCamera());

	//ボスの初期化
	bossBodyModel = std::make_unique<Model>();
	bossBodyModel = Model::CreateModel("BossBody");

	bossBarrelModel = std::make_unique<Model>();
	bossBarrelModel = Model::CreateModel("BossBarrel");

	boss = std::make_unique<BossEnemy>();
	boss->Initialize(bossBodyModel.get(), bossBarrelModel.get(), railCamera->GetObject3d());

}

void GamePlayScene::Finalize()
{
	//ゲームループで使用した物を解放後、基盤システムの後処理と解放を行う

	//delete sprite;
	//delete skyDome;
	delete currentCamera;

	//delete railCamera;

	//-------------ここまでにループ内で使用したものの後処理------------//



}

void GamePlayScene::Update()
{


	//----------------------ゲーム内ループはここから---------------------//


	

#pragma region デバッグ用コード

	if (ImGui::Button("Enemy spwan")) {
		EnemySpawn();
	}

	if (Input::GetInstance()->IsKeyTrigger(DIK_0)) {
		EnemySpawn();
	}

	//スタートボタンでレールカメラ開始
	if (Input::GetInstance()->IsPadTrigger(XINPUT_GAMEPAD_START) || Input::GetInstance()->IsKeyTrigger(DIK_F2)) {
		railCamera->Start();
	}

	if (ImGui::Button("game start") ) {
		railCamera->Start();
	}

#pragma endregion デバッグ用コード




	////レールカメラの更新
	//railCamera->Update();

	////レールカメラを親にする
	//player->parent = railCamera->GetObject3d();
	//player->Update(&enemys);

	////自機の死亡が確認されたらシーン移動
	//if (!player->IsAlive()) {
	//	//シーンの切り替えを依頼
	//	sceneManager->ChangeScene("GAMEOVER");
	//}

	////敵配列の更新
	//for (std::unique_ptr<Enemy>& enemy : enemys) {
	//	enemy->Update(player->GetWorldPosition(), railCamera->GetObject3d()->matWorld);
	//}

	////死んでる敵を消す
	//enemys.remove_if([](std::unique_ptr<Enemy>& enemy) {
	//	if (!enemy->IsAlive()) {
	//		return true;
	//	}
	//	return false;
	//	});




	////レールカメラが5%進むごとに敵を一体スポーン
	//float cameraProgressPercent = railCamera->GetProgress() * 100.0f;
	//if (fmodf(cameraProgressPercent, 10.0f) == 0.0f && cameraProgressPercent != 0) {
	//	EnemySpawn();
	//}

	//Enemy::EnemyParticleUpdate();

	ImGui::SliderFloat("cameraX", &currentCamera->eye.x, -100.0f, 100.0f);
	ImGui::SliderFloat("cameraY", &currentCamera->eye.y, -100.0f, 100.0f);
	ImGui::SliderFloat("cameraZ", &currentCamera->eye.z, -100.0f, 100.0f);

	//camera->target = camera->eye;
	//camera->target.z += 1.0f;


	//天球の操作
	ImGui::Begin("skydome");
	ImGui::SliderFloat("rotateY", &skydomeObj->rotation.y, 0.0f, 5.0f);
	ImGui::SliderFloat("scaleX", &skydomeObj->scale.x, 0.0f, 5.0f);
	ImGui::SliderFloat("scaleY", &skydomeObj->scale.y, 0.0f, 5.0f);
	ImGui::SliderFloat("scaleZ", &skydomeObj->scale.z, 0.0f, 5.0f);
	ImGui::End();

	//天球をちょっと回転させる
	skydomeObj->rotation.y += PI / 14400.0f;
	skydomeObj->Update();

	//アニメーション開始ボタン
	if (ImGui::Button("animation start")) {
		object1->PlayAnitimation();
	}

	if (input->IsKeyTrigger(DIK_A)) {
		object1->PlayAnitimation();
	}

	ImGui::Text("current:%d", object1->BaGetCurrentTime());

	//object1->SetRot({ 0,3.14f / 2,0 });
	object1->Update();

	//全フェーズ共通の更新
	UpdateAllPhase();

	//ﾌｪｰｽﾞごとの更新
	switch (gamePhase)
	{
	case GamePhase::Event_GameStart:
		UpdateGameStart();
		break;
	case GamePhase::Game_Main:
		UpdateGamePhase();
		UpdateMain();
		break;
	case GamePhase::Event_BossSpawn:
		UpdateBossSpawn();
		break;
	case GamePhase::Game_Boss:
		UpdateGamePhase();
		UpdateBoss();
		break;
	case GamePhase::Event_GameClear:
		UpdateClear();
		break;
	default:
		break;
	}

	//現在のカメラ情報をレールカメラのものに
currentCamera = railCamera->GetCamera();
currentCamera->UpdateMatrix();

	//当たり判定チェック
	collisionManager->CheckAllCollisions();


	//スペースキーでメインゲームへ
	if (input->IsKeyTrigger(DIK_F1))
	{
		//シーンの切り替えを依頼
		sceneManager->ChangeScene("TITLE");
	}



	//----------------------ゲーム内ループはここまで---------------------//


}

void GamePlayScene::Draw()
{
	//-------背景スプライト描画処理-------//
	Sprite::BeginDraw();

	//backGroundSprite->Draw();

	//-------3Dオブジェクト描画処理-------//
	Object3d::BeginDraw(currentCamera);


	//天球
	skydomeObj->Draw();

	//プレイヤー
	player->Draw();

	colTestObj->Draw();

	//敵
	for (std::unique_ptr<Enemy>& enemy : enemys) {

		enemy->Draw();



	}

	//ボス
	boss->Draw();

	//FBX
	//object1->Draw();

	//パーティクル描画
	ParticleManager::BeginDraw(currentCamera);

	player->DrawParticle();

	Enemy::DrawParticle();

	//------デバッグ線描画処理------//
	DebugLine::BeginDraw(currentCamera);

	DebugLine::Draw({ { -10,0,100 }, { 20,30,100 } }, { 1,0,0,1 });

	railCamera->DrawMoveLine({ 1,0,0,1 });


	//-------前景スプライト描画処理-------//
	Sprite::BeginDraw();

	player->DrawUI();

	boss->DrawSprite();

}

void GamePlayScene::EnemySpawn()
{
	float posZ = Random(150.0f, 200.0f);

	//Z座標はカメラ基準に
	//posZ += railCamera->GetObject3d()->GetWorldPosition().z;

	Vector3 start{ -60,0,posZ };
	Vector3 p1 = { 0,30,posZ };
	Vector3 p2 = { -30,0,posZ };
	Vector3 p3 = { 0,-30,posZ };
	Vector3 p4 = { 30,0,posZ };
	Vector3 end = { 60,0,posZ };

	Vector3 lTop, lBtm, rTop, rBtm;
	lTop = { -60,30,posZ };
	lBtm = { -60,-30,posZ };
	rTop = { 60,30,posZ };
	rBtm = { 60,-30,posZ };



	std::vector<Vector3> enemyMovePoints = { start,p1,p2,p3,p4,end };
	std::vector<Vector3> rightForLeftUpper;//上側右から左
	std::vector<Vector3> rightForLeftLower;//下側右から左
	std::vector<Vector3> leftForRightUpper;//上側左から右
	std::vector<Vector3> leftForRightLower;//下側左から右

	rightForLeftUpper = { rTop, rTop,lTop,lTop };
	rightForLeftLower = { rBtm, rBtm,lBtm,lBtm };
	leftForRightUpper = { lTop, lTop,rTop,rTop };
	leftForRightLower = { lBtm, lBtm,rBtm,rBtm };


	//曲線をカメラ基準に
	//for (auto& p : enemyMovePoints) {
	//	//p += railCamera->GetObject3d()->GetWorldPosition();
	//}

	int pasent = (INT32)Random(0, 100) % 4;

	if (pasent == 0) {
		enemyMovePoints = rightForLeftUpper;
	}
	else if (pasent == 1) {
		enemyMovePoints = rightForLeftLower;
	}
	else if (pasent == 2) {
		enemyMovePoints = leftForRightUpper;
	}
	else {
		enemyMovePoints = leftForRightLower;
	}


	//敵の生成と初期化
	std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();
	newEnemy->Initialize(enemyMovePoints);
	newEnemy->Spawn();

	//リストに登録
	enemys.push_back(std::move(newEnemy));
	

}

void GamePlayScene::UpdateAllPhase()
{
	//レールカメラの更新
	railCamera->Update();

	//レールカメラを親にする
	player->parent = railCamera->GetObject3d();
	player->Update(&enemys);



}

void GamePlayScene::UpdateGamePhase()
{


	//自機の死亡が確認されたらシーン移動
	if (!player->IsAlive()) {
		//シーンの切り替えを依頼
		sceneManager->ChangeScene("GAMEOVER");
	}

}

void GamePlayScene::UpdateGameStart()
{
	//自機のスポーン終了でレールカメラを開始
	if (player->GetSpawnTimer() == 0) {
		railCamera->Start();

		//ﾌｪｰｽﾞをメインゲームに移行
		gamePhase = GamePhase::Game_Main;
	}

}

void GamePlayScene::UpdateMain()
{
	//敵配列の更新
	for (std::unique_ptr<Enemy>& enemy : enemys) {
		enemy->Update(player->GetWorldPosition(), railCamera->GetObject3d()->matWorld);
		//レールカメラが進行しきったら敵を全消滅
		if (railCamera->GetProgress() >= 1.0f) {
			//enemy->Death();
		}
	}

	ImGui::Text("enemy count %d", enemys.size());

	//ゲームフェーズをボス戦(ボス戦イベント)にしてボスをスポーン
	if (railCamera->GetProgress() >= 1.0f) {

		//敵が全員倒されたらゲーム進行
		if (enemys.empty()) {

			boss->Spawn(railCamera->GetObject3d()->matWorld);
			gamePhase = GamePhase::Game_Boss;
		}
	}
	else {
		//レールカメラが5%進むごとに敵を一体スポーン
		float cameraProgressPercent = railCamera->GetProgress() * 100.0f;
		if (fmodf(cameraProgressPercent, 10.0f) == 0.0f && cameraProgressPercent != 0) {
			EnemySpawn();
		}
	}

	//死んでる敵を消す
	enemys.remove_if([](std::unique_ptr<Enemy>& enemy) {
		if (!enemy->IsAlive()) {
			return true;
		}
		return false;
		});



	//敵のパーティクル更新
	Enemy::EnemyParticleUpdate();

}

void GamePlayScene::UpdateBossSpawn()
{
	


	//ボスの更新
	boss->Update(player->GetWorldPosition());
}

void GamePlayScene::UpdateBoss()
{
	//TODO:この辺の処理はボス登場のカメラワークが完成したらボス登場更新に持ってく
	//最後の敵が死んでもパーティクルが更新されるように
	Enemy::EnemyParticleUpdate();


	//
	boss->Update(player->GetWorldPosition());
}

void GamePlayScene::UpdateClear()
{
}
