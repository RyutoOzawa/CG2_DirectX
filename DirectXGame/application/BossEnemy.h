#pragma once
#include"Object3d.h"
#include"Model.h"
#include<array>

//ボスの行動列挙クラス
enum class BossAct {
	Spawn,			//スポーン
	Move,			//行動
	AttackShot,		//攻撃1
	AttackLaser,	//攻撃2
	Death,			//死亡
};

class BossEnemy : public Object3d
{
public: 

	//初期化
	void Initialize(Model* bodyModel,Model* barrelModel);

	//更新
	void Update(const Vector3& playerPos);

	//描画
	void Draw();

	//終了
	void Finalize();

private:
	Model* bodyModel;
	Model* BarrelModel;

	static const INT32 barrelCount = 4;
	const float baseBarrelDistance = 15.0f;

	std::array<Object3d,barrelCount> barrelObject;
	std::array<Vector3,barrelCount> barrelDistance;
	std::array<float,barrelCount> barrelRadian;

	BossAct bossAct = BossAct::Move;

	//各行動の更新処理
	void UpdateSpawn();
	void UpdateMove();
	void UpdateAtkShot();
	void UpdateAtkLaser();
	void UpdateDeath();
};

