#pragma once
//ゲームシーンクラス
//ゲーム本編の処理を行う

#include"GameBaseScene.h"
#include"ImguiManager.h"
#include"Input.h"
#include"Sprite.h"
#include"Camera.h"
#include"Object3d.h"
#include"AudioManager.h"
#include<memory>
#include"CollisionPrimitive.h"
#include"FbxModel.h"
#include"FbxObject3d.h"
#include"ParticleManager.h"
#include"Player.h"
#include"RailCamera.h"
#include"Enemy.h"
#include"CollisionManager.h"
#include"BossEnemy.h"

enum class GamePhase {
	Event_GameStart,//ゲーム開始イベント(自機の出撃)
	Game_Main,//メインのゲーム(雑魚敵戦)
	Event_BossSpawn,//ボスのスポーンイベント
	Game_Boss,//ボス戦
	Event_GameClear,//ゲームクリアイベント

};

class GamePlayScene :public GameBaseScene
{
public:
	//初期化
	void Initialize()override;

	//終了
	void Finalize()override;

	//更新
	void Update()override;

	//描画
	void Draw()override;

	void EnemySpawn();


private://メンバ変数

	Input* input = nullptr;
	CollisionManager* collisionManager = nullptr;

	TextureData* backGroundTexture = nullptr;
	TextureData* marioGraph = nullptr;
	TextureData* reimuGraph = nullptr;
	TextureData* particleGraph = nullptr;
	TextureData* whiteGraph = nullptr;
	TextureData* reticleGraph = nullptr;
	TextureData* testTex1 = nullptr;
	TextureData* testTex2 = nullptr;
	TextureData* testTex3 = nullptr;


	std::unique_ptr<Sprite> backGroundSprite = nullptr;
	std::unique_ptr<Model> skydome = nullptr;
	std::unique_ptr<Model> defaultModel = nullptr;
	std::unique_ptr<Model> playerModel = nullptr;
	std::unique_ptr<Model> playerBulletModel = nullptr;
	std::unique_ptr<Model> enemyModel = nullptr;
	std::unique_ptr<Object3d> skydomeObj = nullptr;
	std::unique_ptr<FbxModel> model1 = nullptr;
	std::unique_ptr<FbxObject3d> object1 = nullptr;
	std::unique_ptr<Player> player = nullptr;
	std::list< std::unique_ptr<Enemy>> enemys;
	std::unique_ptr<Object3d> colTestObj = nullptr;

	std::unique_ptr<AudioManager> newAudio = nullptr;

	//ボス　
	std::unique_ptr<BossEnemy> boss = nullptr;

	std::unique_ptr<Model> bossBodyModel = nullptr;
	std::unique_ptr<Model> bossBarrelModel = nullptr;


	Camera* currentCamera = nullptr;
	RailCamera* railCamera = nullptr;

	//ゲームフェーズ
	GamePhase gamePhase = GamePhase::Event_GameStart;

private:

	//各フェーズの更新
	//全フェーズで絶対に更新する処理
	void UpdateAllPhase();	
	//ゲームフェーズで更新する処理
	void UpdateGamePhase();	
	//ゲーム開始
	void UpdateGameStart();
	//メインゲーム
	void UpdateMain();		
	//ボス発生
	void UpdateBossSpawn();	
	//ボス戦
	void UpdateBoss();		
	//ゲームクリア
	void UpdateClear();		

};

