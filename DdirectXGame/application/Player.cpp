#include "Player.h"
#include"Input.h"

void Player::Initialize()
{
	Object3d::Initialize();

	position = { 0,0,100 };

	Vector3 start{ -10,0,0 };
	Vector3 p1{ -3,5,-5 };
	Vector3 p2{ 3,-5,5 };
	Vector3 end{ 110,0,-10 };

	std::vector<Vector3> c{start,p1,p2,end};

	splineTest.SetPositions(c);



}

void Player::Update()
{

	Move();


	//obj3dの更新
	Object3d::Update();
}

void Player::Move()
{
	//座標に加算する速度
	Vector3 spd{};
	float baseSpd = 0.5f;

	splineTest.Update();

	//WASD移動
	if (Input::GetInstance()->IsKeyPress(DIK_A)) {
		spd.x -= baseSpd;
	}
	else if (Input::GetInstance()->IsKeyPress(DIK_D)) {
		spd.x += baseSpd;
	}

	if (Input::GetInstance()->IsKeyPress(DIK_S)) {
		spd.y -= baseSpd;
	}
	else if (Input::GetInstance()->IsKeyPress(DIK_W)) {
		spd.y += baseSpd;
	}

	if (Input::GetInstance()->IsKeyTrigger(DIK_SPACE)) {
		splineTest.Start(240.0f);
	}

	//現在座標を取得
	pos = position;
	//加算して代入
	pos += spd;
	position = pos;

	position = splineTest.GetPosition();
	position.z += 100.0f;
}
