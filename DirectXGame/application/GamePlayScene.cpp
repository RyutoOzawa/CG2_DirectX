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
using namespace Utility;
using namespace Easing;

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

	texUIPlay = Texture::LoadTexture("UI_play.png");
	spriteUIPlay = std::make_unique<Sprite>();
	spriteUIPlay->Initialize(texUIPlay);
	spriteUIPlay->SetAnchorPoint({ 1.0f,0.0f });
	spriteUIPlay->SetPos({ WindowsAPI::winW,0.0f });

	skydome = std::make_unique<Model>();
	skydome = Model::CreateModel("skydome");

	defaultModel = std::make_unique<Model>();
	defaultModel = Model::CreateModel();
	defaultModel->SetTexture(reimuGraph);

	playerModel = std::make_unique<Model>();
	playerModel = Model::CreateModel("Player");

	bulletModel = std::make_unique<Model>();
	bulletModel = Model::CreateModel("PlayerBullet");

	enemyModel = std::make_unique<Model>();
	enemyModel = Model::CreateModel("ZakoEnemy");

	Enemy::EnemyInitialize(particleGraph, enemyModel.get(), bulletModel.get());


	skydomeObj = std::make_unique<Object3d>();
	skydomeObj->Initialize();
	skydomeObj->SetModel(skydome.get());
	Vector3 skydomeScale = { 1000.0f, 1000.0f,1000.0f };
	skydomeObj->scale = { skydomeScale };


	player = std::make_unique<Player>();
	player->Initialize(playerModel.get(), reticleGraph, whiteGraph);

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



	//レールカメラの初期化とセット
	railCamera = std::make_unique< RailCamera>();
	railCamera->Initialize({ 0,0,0 }, { 0,0,0 });

	//現在のカメラ情報をレールカメラのものに
	currentCamera = railCamera->GetCamera();
	currentCamera->UpdateMatrix();

	//イベントシーン用カメラ
	eventCamera = std::make_unique<EventCamera>();
	eventCamera->Initialize({ 0,0,0 }, { 0,0,1 }, { 0,1,0 });

	//デバイスセット
	FbxObject3d::SetCamera(currentCamera);

	//obj3dクラスにカメラをセット
	Object3d::SetCamera(railCamera->GetCamera());

	//ボスの初期化
	bossBodyModel = std::make_unique<Model>();
	bossBodyModel = Model::CreateModel("BossBody");

	bossBarrelModel = std::make_unique<Model>();
	bossBarrelModel = Model::CreateModel("BossBarrel");

	boss = std::make_unique<BossEnemy>();
	boss->Initialize(bossBodyModel.get(), bossBarrelModel.get(), railCamera->GetObject3d());

	//ステージのobjデータ読み込み
	stageData = JsonLoader::LoadJsonFile("gameStageData");

	//読み込んだデータでモデル、obj生成
	JsonLoader::CreateObjectFromLevelData(stageData, stageObjects, stageModels);

	//敵の配置データ読み込み
	enemyData = EnemyLoader::LoadEnemyData("enemyData.txt");

}

void GamePlayScene::Finalize()
{
	//ゲームループで使用した物を解放後、基盤システムの後処理と解放を行う

	//delete sprite;
	//delete skyDome;

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

	if (ImGui::Button("game start")) {
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
		//現在のカメラ情報をレールカメラのものに
		currentCamera = railCamera->GetCamera();

		break;
	case GamePhase::Event_BossSpawn:
		UpdateBossSpawn();
		currentCamera = eventCamera->GetCamera();

		break;
	case GamePhase::Game_Boss:
		UpdateGamePhase();
		UpdateBoss();
		//現在のカメラ情報をレールカメラのものに
		currentCamera = railCamera->GetCamera();
		break;
	case GamePhase::Event_GameClear:
		UpdateClear();
		currentCamera = eventCamera->GetCamera();
		break;
	default:
		break;
	}

	currentCamera->UpdateMatrix();

	ImGui::Begin("camera");
	ImGui::Text("current");
	ImGui::Text("eye %f,%f,%f", currentCamera->eye.x, currentCamera->eye.y, currentCamera->eye.z);
	ImGui::Text("target %f,%f,%f", currentCamera->target.x, currentCamera->target.y, currentCamera->target.z);
	ImGui::Text("rail");
	ImGui::Text("eye %f,%f,%f", railCamera->GetCamera()->eye.x, railCamera->GetCamera()->eye.y, railCamera->GetCamera()->eye.z);
	ImGui::Text("target %f,%f,%f", railCamera->GetCamera()->target.x, railCamera->GetCamera()->target.y, railCamera->GetCamera()->target.z);
	ImGui::Text("event");
	ImGui::Text("eye %f,%f,%f", eventCamera->GetCamera()->eye.x, eventCamera->GetCamera()->eye.y, eventCamera->GetCamera()->eye.z);
	ImGui::Text("target %f,%f,%f", eventCamera->GetCamera()->target.x, eventCamera->GetCamera()->target.y, eventCamera->GetCamera()->target.z);
	ImGui::End();


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

	//ステージのobj描画
	for (int i = 0; i < stageObjects.size(); i++) {
		stageObjects[i]->Draw();
	}

	//FBX
	//object1->Draw();

	//パーティクル描画
	ParticleManager::BeginDraw(currentCamera);

	player->DrawParticle();

	Enemy::DrawParticle();

	boss->DrawParticle();

	//------デバッグ線描画処理------//
	DebugLine::BeginDraw(currentCamera);

	//DebugLine::Draw({ { -10,0,100 }, { 20,30,100 } }, { 1,0,0,1 });
	//railCamera->DrawMoveLine({ 1,0,0,1 });

	//-------前景スプライト描画処理-------//
	Sprite::BeginDraw();

	player->DrawUI();

	spriteUIPlay->Draw();

	boss->DrawSprite();

}

void GamePlayScene::EnemySpawn()
{
	float posZ = 0.0f;
	

	//Z座標はカメラ基準に
	//posZ += railCamera->GetObject3d()->GetWorldPosition().z;

	//Vector3 start{ -60,0,posZ };
	//Vector3 p1 = { 0,30,posZ };
	//Vector3 p2 = { -30,0,posZ };
	//Vector3 p3 = { 0,-30,posZ };
	//Vector3 p4 = { 30,0,posZ };
	//Vector3 end = { 60,0,posZ };

	//Vector3 lTop, lBtm, rTop, rBtm;
	//lTop = { -60,30,posZ };
	//lBtm = { -60,-30,posZ };
	//rTop = { 60,30,posZ };
	//rBtm = { 60,-30,posZ };

	//std::vector<Vector3> enemyMovePoints = { start,p1,p2,p3,p4,end };
	//std::vector<Vector3> rightForLeftUpper;//上側右から左
	//std::vector<Vector3> rightForLeftLower;//下側右から左
	//std::vector<Vector3> leftForRightUpper;//上側左から右
	//std::vector<Vector3> leftForRightLower;//下側左から右

	//rightForLeftUpper = { rTop, rTop,lTop,lTop };
	//rightForLeftLower = { rBtm, rBtm,lBtm,lBtm };
	//leftForRightUpper = { lTop, lTop,rTop,rTop };
	//leftForRightLower = { lBtm, lBtm,rBtm,rBtm };


	////曲線をカメラ基準に
	////for (auto& p : enemyMovePoints) {
	////	//p += railCamera->GetObject3d()->GetWorldPosition();
	////}

	//int pasent = (INT32)Random(0, 100) % 4;

	//if (pasent == 0) {
	//	enemyMovePoints = rightForLeftUpper;
	//}
	//else if (pasent == 1) {
	//	enemyMovePoints = rightForLeftLower;
	//}
	//else if (pasent == 2) {
	//	enemyMovePoints = leftForRightUpper;
	//}
	//else {
	//	enemyMovePoints = leftForRightLower;
	//}


	////敵の生成と初期化
	//std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();
	//newEnemy->Initialize(enemyMovePoints);
	//newEnemy->Spawn();

	//敵のデータ配列を走査
	for (size_t i = 0; i < enemyData.size(); i++) {
		//フレームカウントでスポーンする敵がいればスポーンさせる
		if (enemyData[i].spawnTime == frameCount) {

			std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();
			Vector3 newEnemyPosition = enemyData[i].spawnPos;


			newEnemy->Initialize(enemyData[i].spawnPos, enemyData[i].leaveTime);
			newEnemy->Spawn();

			//リストに登録
			enemys.push_back(std::move(newEnemy));

			//データ配列から該当データを削除+for文を1回巻き戻す
			enemyData.erase(enemyData.begin() + i);
			i--;

		}

	
	}

	


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
		railCamera->Start(600.0f);

		//ﾌｪｰｽﾞをメインゲームに移行
		gamePhase = GamePhase::Game_Main;

		//フレームカウントをリセット
		frameCount = 0;
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

			////ボススポーンへ遷移
			gamePhase = GamePhase::Event_BossSpawn;
			////レールカメラの座標をイベントカメラにもコピー
			eventCamera->SetEye(railCamera->GetCamera()->eye);
			eventCamera->SetTarget(railCamera->GetCamera()->target);

		}
	}
	else {
		//敵出現制御用のフレームカウントを進める
		frameCount++;

		//レールカメラが5%進むごとに敵を一体スポーン
		float cameraProgressPercent = railCamera->GetProgress() * 100.0f;
		//if (fmodf(cameraProgressPercent, 10.0f) == 0.0f && cameraProgressPercent != 0) {
		EnemySpawn();
		//}
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
	boss->Update(player->GetWorldPosition(), eventCamera.get());
	//カメラ更新
	eventCamera->Update();

	//条件でボス戦へ遷移
	if (boss->GetBossAct() == BossAct::Move) {
		gamePhase = GamePhase::Game_Boss;
	}

	//最後の敵が死んでもパーティクルが更新されるように
	Enemy::EnemyParticleUpdate();

}

void GamePlayScene::UpdateBoss()
{


	boss->Update(player->GetWorldPosition(), eventCamera.get());

	//ボスのHPが0になったらゲームクリア演出開始
	if (boss->GetHealth() == 0) {

		//自機の演出開始
		//player->Leave();


		//sceneManager->ChangeScene("GAMECLEAR");

		//イベントカメラの移動とセット
		//eventCamera->SetEye(railCamera->GetCamera()->eye);
		//eventCamera->SetTarget(railCamera->GetCamera()->target);
		Vector3 cameraTargetAfter = eventCamera->GetCamera()->target;
		cameraTargetAfter += {10, 20, 30};
		//eventCamera->MoveTarget(cameraTargetAfter, 60, InterType::EaseIn);
		//ゲームフェーズの変更
		gamePhase = GamePhase::Event_GameClear;

		//レールカメラの座標をイベントカメラにもコピー
		eventCamera->SetEye(railCamera->GetCamera()->eye);
		eventCamera->SetTarget(railCamera->GetCamera()->target);

	}
}

void GamePlayScene::UpdateClear()
{

	//ボス
	boss->Update(player->GetWorldPosition(), eventCamera.get());

	//イベントカメラの更新
	eventCamera->Update();

	//ボスが死んだらクリアシーンへ
	if (!boss->IsAlive()) {
		sceneManager->ChangeScene("GAMECLEAR");
	}

}
