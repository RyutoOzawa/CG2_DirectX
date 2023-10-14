//ベジエ曲線クラス
//n次ベジエ補間を行う
//制御点をいくらでも設定できるため、n次に対応可能

#include "BezierCurve.h"
#include"DebugLine.h"

void BezierCurve::Start(float allTime_, bool isLoop_)
{
	allTime = allTime_;
	eData.Start(allTime);
	currentPos = controllPoints.front();
	isLoop = isLoop_;
}

void BezierCurve::Update()
{
	eData.Update();

	if (eData.GetTimeRate() < 1.0f) {
		//ループするなら配列の末尾に先頭を差し込んで関数回す
		std::vector<Vector3> points = controllPoints;
		if (isLoop) {
			points.push_back(points.front());
		}

		currentPos = BezierPoints(points,eData.GetTimeRate());
	}
	else {
		//ループされているならもう一度開始
		if (isLoop) {
			Start(allTime, true);
		}
		else {
			currentPos = controllPoints.back();
		}
	}

}

void BezierCurve::DrawCurve(const Vector4& color, const Vector3& offset)
{
	std::vector<Vector3> points = controllPoints;
	if (isLoop) {
		points.push_back(points.front());
	}

	//制御点1つにつき描画用点は96個
	size_t pointCount = points.size() * 96;
	//描画用座標配列
	std::vector<Vector3> drawPoints;
	for (size_t i = 0; i < pointCount; i++) {
		//1.0をpointCountで分割して点を割り出していく
		float t = 1.0f / pointCount * i;
		Vector3 p = BezierPoints(points, t);
		//オフセット加算
		p += offset;
		drawPoints.push_back(p);

	}

	DebugLine::Draw(drawPoints, color);

}

Vector3 BezierCurve::BezierPoints(std::vector<Vector3> points,float t)
{

	//要素が2つならその2点の補間でとる
	size_t arraySize = points.size();
	if (arraySize == 2) {
		return Vector3::Lerp(points.front(), points.back(), t);
	}
	else if (arraySize == 1) {
		return points.front();
	}
	else if (arraySize == 0) {
		return Vector3(0,0,0);
	}
	else {

		//引数の配列から新しく配列を作る
		std::vector<Vector3> newPoints;
		for (size_t i = 0; i < points.size() - 1; i++) {
			Vector3 point;
			point = Vector3::Lerp(points[i], points[i + 1], t);
			newPoints.push_back(point);
		}

		//要素数が2個になるまで再帰呼び出し
		return BezierPoints(newPoints, t);

	}
}
