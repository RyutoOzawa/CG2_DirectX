#pragma once
#include"Object3d.h"
#include"Model.h"
#include"Vector3.h"

//3Dオブジェクトなのでobj3dクラスを継承
class Player :public Object3d
{
public:	//メンバ関数

	//初期化
	void Initialize();

	//更新
	void Update();

//	void Draw();

private://メンバ変数

	Vector3 pos;

	

private://内部処理用メンバ関数

	void Move();

};

