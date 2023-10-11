#pragma once
#include"GameBaseScene.h"
#include"AbstractSceneFactory.h"
#include"BaseSceneTransition.h"

class GameSceneManager
{
private:
	GameSceneManager();
	~GameSceneManager();

public:

	//コピーコンストラクタ無効
	GameSceneManager(const GameSceneManager& obj) = delete;
	//代入演算子を無効
	GameSceneManager& operator=(const GameSceneManager& obj) = delete;

	//インスタンスアクセス専用関数
	static GameSceneManager* GetInstance();

	//更新
	void Update();

	//描画
	void Draw();

	//終了処理
	void Finalize();

	/// <summary>
	/// シーン変更依頼
	/// </summary>
	/// <param name="sceneName">次シーン</param>
	/// <param name="isTransition">遷移を行うか</param>
	/// <param name="transitionName">遷移のパターン</param>
	void ChangeScene(const std::string& sceneName,bool isTransition = true,const std::string& transitionName = "FADE");

	//シーンファクトリーのセッター
	void SetSceneFactory(AbstractSceneFactory* sceneFactory_) { sceneFactory = sceneFactory_; }

private:
	//現在のシーン
	GameBaseScene* activeScene = nullptr;
	GameBaseScene* nextScene = nullptr;

	//借りてくるシーンファクトリー
	AbstractSceneFactory* sceneFactory = nullptr;

	//シーン遷移クラス
	BaseSceneTransition* sceneTransition = nullptr;

	//次シーンの予約の検知
	void CheckNextScene();

};

