#include"Util.h"
#include<random>

float Random(float num1, float num2) {

	float min, max;

	min = num2;
	max = num1;

	if (num1 < num2) {
		min = num1;
		max = num2;
	}



	//乱数シード生成器
	std::random_device seedGem;
	//メルセンヌ・ツイスターの乱数エンジン
	std::mt19937_64 engine(seedGem());
	//乱数範囲の指定
	std::uniform_real_distribution<float> dist(min, max);

	return dist(engine);
}

int RGBColorCode(const Vector3& rgb)
{
	int code = 0x000000;

	int r = (int)rgb.x;
	int g = (int)rgb.y;
	int b = (int)rgb.z;
	r = r << 16;
	g = g << 8;

	code = r + g + b;


	return code;

	

}

Vector3 ColorCodeRGB(int colorCode)
{
	Vector3 rgb = { (float)(colorCode >> 16),(float)(colorCode >> 8),(float)(colorCode & 0xff) };

	return rgb / 256.0f;
}

Vector3 ConvertScreenToWorld(const Vector2& v, float distanceZ, const Matrix4& matViewProjectionViewPort)
{
	Vector3 posNear = { v.x,v.y,0 };
	Vector3 posFar = { v.x,v.y,1 };
	Matrix4 matVBVInverse = matViewProjectionViewPort;
	matVBVInverse.Inverse();

	posNear = Matrix4::transformDivW(posNear, matVBVInverse);
	posFar = Matrix4::transformDivW(posFar, matVBVInverse);
	
	Vector3 direction = posFar - posNear;
	direction.normalize();

	return posNear + direction * distanceZ;
}

float EaseOut(float t)
{
	return 1.0f - powf(1-t,5);
}
