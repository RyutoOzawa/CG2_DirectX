#pragma once
#include"Object3d.h"
#include"SplineCurve.h"
#include"ParticleManager.h"

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

	static void EnemyInitialize(uint32_t texIndex);

	static void DrawParticle();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="points">移動用スプライン曲線の座標配列</param>
	void Initialize(std::vector<Vector3>& points);

	void Update();

	
	void Draw();

	//スポーン
	void Spawn();

	void OnCollision(const CollisionInfo& info) override;

	bool IsAlive()const { return isAlive; }

private:

	static ParticleManager particleManager;

	//動線
	SplineCurve moveLine;

	bool isAlive = false;



};

