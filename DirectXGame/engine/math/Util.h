#define PI 3.1415926f
#include"Vector2.h"
#include"Vector3.h"
#include"Matrix4.h"

float Random(float num1, float num2);

int RGBColorCode(const Vector3& rgb);

Vector3 ColorCodeRGB(int colorCode);

//スクリーン座標からワールド座標に変換
Vector3 ConvertScreenToWorld(const Vector2& v,float distanceZ, const Matrix4& matViewProjectionViewPort);

float EaseOut(float t);

float Lerp(float before, float after, float t);