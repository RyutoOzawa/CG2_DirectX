#include "SplineCurve.h"

void SplineCurve::Start(float allTime)
{
	//補間を行う回数
	int32_t lerpCount;

	//補間の回数は配列の総数-2
	lerpCount = (int32_t)controllPoints.size()-2;

	//補間の全体時間÷補間の回数で一区間当たりの時間を計算
	allTimeOnce = allTime / (float)lerpCount;

	//ダミー座標のセット
	dummyPoints[0] = controllPoints.front();
	dummyPoints[1] = controllPoints.back();

	currentPos = controllPoints[0];

	//開始時の要素番号を初期化してイージング開始
	startIndex = 0;
	eData.Start(allTimeOnce);


}

void SplineCurve::Update()
{
	//イージングデータの更新
	eData.Update();

	float t = eData.GetTimeRate();
	//tが1以上になったら一区間の補間終了
	if (t >= 1.0f) {
		//再度イージング開始してstartindexを1進める
		eData.Start(allTimeOnce);
		startIndex++;
		t = 0.0f;
	}


	//補完用座標
	Vector3 p0, p1, p2, p3;
	//始点と終点には追加でダミーを用意しているので補完する点の数は(制御点-2)
	size_t lerpMax = controllPoints.size() - 2;

	//最初の補間、最後の補間ではそれぞれp0,p3にダミーを使う
	if (startIndex == 0) {
		//n = 0はp0(ダミー) p0 p1 p2
		p0 = dummyPoints[0];
	}
	else if (startIndex  < 0) {
		currentPos = dummyPoints[0];
		return;
	}
	else {
		p0 = controllPoints[startIndex - 1];
	}

	if (startIndex == lerpMax) {
		p3 = dummyPoints[1];
	}
	else if (startIndex > lerpMax) {
		currentPos = dummyPoints[1];
		return;
	}
	else {

		p3 = controllPoints[startIndex + 2];
	}

	p1 = controllPoints[startIndex];
	p2 = controllPoints[startIndex+1];

	Vector3 position = 0.5 * (((2 * p1) + (-p0 + p2) * t) +
		(((2 * p0) - (5 * p1) + (4 * p2) - p3) * (t * t)) +
		((-p0 + (3 * p1) - (3 * p2) + p3) * (t * t * t)));

	currentPos = position;
}
