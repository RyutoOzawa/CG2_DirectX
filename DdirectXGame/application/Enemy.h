#pragma once
#include"Object3d.h"
#include"SplineCurve.h"

class Enemy : public Object3d
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="points">移動用スプライン曲線の座標配列</param>
	void Initialize(std::vector<Vector3>& points);

	void Update();

	
	void Draw();

	//スポーン
	void Spawn();

private:

	//動線
	SplineCurve moveLine;

	bool isAlive = false;

};

