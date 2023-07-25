#pragma once
#include"EasingData.h"
#include"Vector3.h"
#include"Vector4.h"
#include<vector>

class BezierCurve
{
private:
	//制御点配列
	std::vector<Vector3> controllPoints;
	//現在座標
	Vector3 currentPos{};
	//補完用データ
	EasingData eData;
	//ループするか
	bool isLoop = false;
	//進行度
	bool isActive = false;
	float allTime = 0.0f;

public:
	//制御点のセット
	void SetPositions(std::vector<Vector3> points) { this->controllPoints = points; }

	/// <summary>
	/// 補間の開始
	/// </summary>
	/// <param name="allTime">補間の時間</param>
	/// <param name="isloop">ループするか</param>
	void Start(float allTime, bool isloop);

	//更新
	void Update();

	Vector3 GetPosition()const { return currentPos; }


	/// <summary>
	/// 曲線描画
	/// </summary>
	/// <param name="color">色</param>
	/// <param name="offset">オフセット(デフォルトは{0,0,0})</param>
	void DrawCurve(const Vector4& color, const Vector3& offset = { 0,0,0 });

	//制御点を減らしていく再帰関数
	Vector3 BezierPoints(std::vector<Vector3> posints,float t);
};

