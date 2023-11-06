//ユーティリティクラス
//色々便利な関数達をまとめたクラス

#define PI 3.1415926f
#include"Vector2.h"
#include"Vector3.h"
#include"Matrix4.h"

/// <summary>
/// ランダム
/// </summary>
/// <param name="num1">値1</param>
/// <param name="num2">値2</param>
/// <returns>1と2の間からランダムな値</returns>
float Random(float num1, float num2);

//カラーコードを(R,G,B)に変換
int RGBColorCode(const Vector3& rgb);

//(R,G,B)をカラーコードに変換
Vector3 ColorCodeRGB(int colorCode);

//スクリーン座標からワールド座標に変換
Vector3 ConvertScreenToWorld(const Vector2& v,float distanceZ, const Matrix4& matViewProjectionViewPort);

//イージングアウト
float EaseOut(float t);

//線形補間
float Lerp(float before, float after, float t);

//クランプ
float Clump(float num, float min, float max);