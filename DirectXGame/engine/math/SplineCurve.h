#pragma once
#include"EasingData.h"
#include"Vector3.h"
#include<vector>

class SplineCurve
{
private:
	//制御点配列
	std::vector<Vector3> controllPoints;
	//制御点の間で動作出来るための始点と終点のコピー座標
	Vector3 dummyPoints[2]{};
	//現在座標
	Vector3 currentPos{};
	//現在どの制御点から補間を行っているか
	int32_t startIndex = 0;
	//補完用データ
	EasingData eData;
	//一区間当たりの補間時間
	float allTimeOnce = 0.0f;
	//始点と終点をつないでループするかどうか
	bool isLoop = false;
	//全体の時間
	float allTime = 0.0f;
	//現在の進行度
	float progress = 0.0f;
	bool isActive = false;
	int count = 0;


public:
	//制御点のセット
	void SetPositions(std::vector<Vector3> points) { this->controllPoints = points; }

	/// <summary>
	/// 補間の開始
	/// </summary>
	/// <param name="allTime">補間にかかる時間</param>
	void Start(float allTime,bool isLoop = false);

	//更新
	void Update();

	Vector3 GetPosition()const { return currentPos; }

};

