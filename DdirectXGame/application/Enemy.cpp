#include "Enemy.h"

void Enemy::Initialize(std::vector<Vector3>& points)
{
	//object3d‚Ì‰Šú‰»
	Object3d::Initialize();

	moveLine.SetPositions(points);
	


}

void Enemy::Update()
{
	

	moveLine.Update();

	position = moveLine.GetPosition();

	Object3d::Update();

}

void Enemy::Draw()
{
	if (!isAlive) {
		return;
	}

	Object3d::Draw();

}

void Enemy::Spawn()
{
	moveLine.Start(600);

	isAlive = true;
}
