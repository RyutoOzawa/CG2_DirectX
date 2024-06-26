#pragma once
//動作テスト用シーン
//オブジェクト単体の動きの確認や、ボス等のモーションの作成等を一度こちらでチェックし、本編のゲームシーンに実装する


#include"GameBaseScene.h"
#include"BossEnemy.h"
#include"Camera.h"

class TestScene : public GameBaseScene
{
public:
	//初期化
	void Initialize() override;

	//終了
	void Finalize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

private:

	BossEnemy boss;

	std::unique_ptr<Model> bossBodyModel = nullptr;
	std::unique_ptr<Model> bossBarrelModel = nullptr;

	Object3d objRailDummy;

	Camera* camera = nullptr;

};

