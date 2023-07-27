#include "TestScene.h"
#include"DebugLine.h"

void TestScene::Initialize()
{
	bossBodyModel = std::make_unique<Model>();
	bossBodyModel = Model::CreateModel("BossBody");

	bossBarrelModel = std::make_unique<Model>();
	bossBarrelModel = Model::CreateModel("BossBarrel");

	boss.Initialize(bossBodyModel.get(), bossBarrelModel.get());

	Vector3 eye = { 0,0,0 };
	Vector3 target = { 0,0,1 };
	Vector3 up = { 0,1,0 };

	//eye = { 0,100,240 };
	//target = { 0,0,230 };
	//up = { 0,1,0 };

	camera = new Camera();
	camera->Initialize(eye,target,up);



	Object3d::camera = camera;
}

void TestScene::Finalize()
{
}

void TestScene::Update()
{
	Vector3 pos(0, 0, 50);

	boss.Update(pos);
}

void TestScene::Draw()
{
	Object3d::BeginDraw(camera);

	boss.Draw();

	DebugLine::BeginDraw(camera);

	boss.DrawDebugLine();

	Sprite::BeginDraw();
	boss.DrawSprite();
}
