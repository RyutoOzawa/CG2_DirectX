#pragma once
#include"GameBaseScene.h"
#include"BossEnemy.h"
#include"Camera.h"

class TestScene : public GameBaseScene
{
public:
	void Initialize() override;

	void Finalize() override;

	void Update() override;

	void Draw() override;

private:

	BossEnemy boss;

	std::unique_ptr<Model> bossBodyModel = nullptr;
	std::unique_ptr<Model> bossBarrelModel = nullptr;

	Object3d objRailDummy;

	Camera* camera = nullptr;

};

