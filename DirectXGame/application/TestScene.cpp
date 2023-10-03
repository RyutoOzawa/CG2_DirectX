#include "TestScene.h"
#include"DebugLine.h"
#include"Input.h"

void TestScene::Initialize()
{
	bossBodyModel = std::make_unique<Model>();
	bossBodyModel = Model::CreateModel("BossBody");

	bossBarrelModel = std::make_unique<Model>();
	bossBarrelModel = Model::CreateModel("BossBarrel");



	Vector3 eye = { 0,0,0 };
	Vector3 target = { 0,0,1 };
	Vector3 up = { 0,1,0 };

	//eye = { 0,100,240 };
	//target = { 0,0,230 };
	//up = { 0,1,0 };

	camera = new Camera();
	camera->Initialize(eye,target,up);

	objRailDummy.Initialize();
	objRailDummy.matWorld.identity();

	boss.Initialize(bossBodyModel.get(), bossBarrelModel.get(),&objRailDummy);

	Object3d::camera = camera;
}

void TestScene::Finalize()
{
}

void TestScene::Update()
{
	Vector3 pos(0, 0,50);

	//1キーでボススポーン
	if (Input::GetInstance()->IsKeyTrigger(DIK_0)) {
		boss.Spawn(objRailDummy.matWorld);
	}

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
