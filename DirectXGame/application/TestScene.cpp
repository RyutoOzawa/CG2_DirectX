#include "TestScene.h"
#include"DebugLine.h"

void TestScene::Initialize()
{
	bossBodyModel = std::make_unique<Model>();
	bossBodyModel = Model::CreateModel("BossBody");

	bossBarrelModel = std::make_unique<Model>();
	bossBarrelModel = Model::CreateModel("BossBarrel");

	boss.Initialize(bossBodyModel.get(), bossBarrelModel.get());

	camera = new Camera();
	camera->Initialize({0,0,0},{0,0,1},{0,1,0});

}

void TestScene::Finalize()
{
}

void TestScene::Update()
{
	Vector3 pos(0, 0, 0);

	boss.Update(pos);
}

void TestScene::Draw()
{
	Object3d::BeginDraw(camera);

	boss.Draw();

	DebugLine::BeginDraw(camera);

	boss.DrawDebugLine();
}
