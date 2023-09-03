#pragma once
#include"Object3d.h"
#include"SplineCurve.h"
#include"ParticleManager.h"
#include"EnemyBullet.h"

/// <summary>
/// 敵の発生情報
/// </summary>
struct EnemyData {
	float spawnTime = 0.0f;

};

class Enemy : public Object3d
{
public:

	static void EnemyParticleUpdate();

	static void EnemyInitialize(TextureData* texData, Model* enemyModel,Model* bulletModel);

	static void DrawParticle();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="points">移動用スプライン曲線の座標配列</param>
	void Initialize(std::vector<Vector3>& points);

	void Update(const Vector3& playerWorldPos,const Matrix4& cameraMat);

	
	void Draw();

	//スポーン
	void Spawn();

	void OnCollision(const CollisionInfo& info) override;

	bool IsAlive()const { return isAlive; }

	void Attack(const Vector3& playerWorldPos);

private:

	static ParticleManager particleManager;

	//動線
	SplineCurve moveLine;

	bool isAlive = false;

	//弾関係
	INT32 shotInterval = 0;
	static const INT32 shotCoolTime;
	std::list<std::unique_ptr<EnemyBullet>> bullets;
	static Model* model;
	static Model* bulletModel;

	//大きさの制御
	void ScaleControll();

	//スピン
	void Spin();

	Vector3 baseScale = { 3,3,3 };
};

