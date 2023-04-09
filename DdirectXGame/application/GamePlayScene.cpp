#include "GamePlayScene.h"
#include"Texture.h"
#include"DirectX.h"
#include"SpriteManager.h"
#include"GameSceneManager.h"
#include"Collision.h"
#include"Util.h"


using namespace DirectX;

void GamePlayScene::Initialize()
{


	//--------------ゲーム内変数初期化--------------//

	//inputのインスタンス取得
	input = Input::GetInstance();

	//テクスチャデータ初期化

	backGroundTexture = Texture::LoadTexture(L"Resources/dummyPlayGame.png");
	marioGraph = Texture::LoadTexture(L"Resources/mario.jpg");
	reimuGraph = Texture::LoadTexture(L"Resources/reimu.png");
	backGroundSprite = std::make_unique<Sprite>();
	sprite = std::make_unique<Sprite>();
	sprite2 = std::make_unique<Sprite>();
	backGroundSprite->Initialize(backGroundTexture);
	sprite->Initialize(marioGraph);
	sprite2->Initialize(reimuGraph);

	skydome = std::make_unique<Model>();
	skydome = Model::CreateModel("skydome");

	defaultModel = std::make_unique<Model>();
	defaultModel = Model::CreateModel();
	defaultModel->textureIndex = reimuGraph;

	triangleModel = std::make_unique<Model>();
	triangleModel = Model::CreateModel("triangle_mat");

	//カメラ初期化
	Vector3 eye(0, 20, -20);	//視点座標
	Vector3 target(0, 0, 6);	//注視点座標
	Vector3 up(0, 1, 0);		//上方向ベクトル

	camera.Initialize(eye, target, up);

	skydomeObj = std::make_unique<Object3d>();
	skydomeObj->Initialize();
	skydomeObj->SetModel(skydome.get());

	planeObj = std::make_unique<Object3d>();
	planeObj->Initialize();
	planeObj->SetModel(defaultModel.get());
	planeObj->scale = { 10.0f,0.01f,10.0f };
	planeObj->position.y = -2.0f;

	triangleObj = std::make_unique<Object3d>();
	triangleObj->Initialize();
	triangleObj->SetModel(triangleModel.get());

	rayObj = std::make_unique<Object3d>();
	rayObj->Initialize();
	rayObj->SetModel(defaultModel.get());
	rayObj->scale = { 0.01f,10.0f,0.01f };

	newAudio = std::make_unique<AudioManager>();
	newAudio->SoundLoadWave("Resources/bgm_title.wav");

	//球の初期値を設定
	sphere.pos = { -5,2,-0 };
	sphere.radius = 1.0f;
	//平面の初期値を設定
	plane.normal = { 0,1,0 };
	plane.distance = -2.0f;
	//三角形の初期値を設定
	triangle.p0 = { -1.0f,0,-1.0f };
	triangle.p1 = { -1.0f,0,+1.0f };
	triangle.p2 = { +1.0f,0,-1.0f };
	triangle.normal = { 0.0f,1.0f,0.0f };
	//レイの初期値を設定
	ray.start = { 0,1,0 };
	ray.dir = { 0,-1,0 };

	//パーティクル初期化
	particle1 = std::make_unique<ParticleManager>();
	particle1->Initialize();
	particle1->SetTextureHandle(reimuGraph);

	particle2 = std::make_unique<ParticleManager>();
	particle2->Initialize();
	particle2->SetTextureHandle(marioGraph);

	particleStart1 = { -10,0,0 };
	particleStart2 = { 10,0,0 };

}

void GamePlayScene::Finalize()
{
	//ゲームループで使用した物を解放後、基盤システムの後処理と解放を行う

	//delete sprite;
	//delete skyDome;

	//-------------ここまでにループ内で使用したものの後処理------------//



}

void GamePlayScene::Update()
{


	//----------------------ゲーム内ループはここから---------------------//





	camera.UpdateMatrix();

	//天球の操作
	//ImGui::Begin("skydome");
	//ImGui::SliderFloat("rotateY", &skydomeObj->rotation.y, 0.0f, 5.0f);
	//ImGui::SliderFloat("posX", &sphere.pos.x, -10.0f, 10.0f);
	//ImGui::SliderFloat("posY", &sphere.pos.y, -10.0f, 10.0f);
	//ImGui::SliderFloat("posZ", &sphere.pos.z, -10.0f, 10.0f);
	//ImGui::SliderFloat("scaleX", &skydomeObj->scale.x, 0.0f, 5.0f);
	//ImGui::SliderFloat("scaleY", &skydomeObj->scale.y, 0.0f, 5.0f);
	//ImGui::SliderFloat("scaleZ", &skydomeObj->scale.z, 0.0f, 5.0f);
	//ImGui::End();

	//カメラ操作デバッグテキスト
	ImGui::Begin("camera");
	ImGui::SliderFloat("eyeX", &camera.eye.x, -10.0f, 10.0f);
	ImGui::SliderFloat("eyeY", &camera.eye.y, -10.0f, 10.0f);
	ImGui::SliderFloat("eyeZ", &camera.eye.z, -10.0f, 10.0f);
	ImGui::End();

	//当たり判定確認
	ImGui::Begin("collision");

	if (Collision::ColRayToSphere(ray,sphere,nullptr,&colHitPos)) {
		ImGui::Text("hit Ray to Sphere!");
		ImGui::Text("hitPos:(%2.2f,%2.2f,%2.2f)", colHitPos.x, colHitPos.y, colHitPos.z);
	}

	if (Collision::ColRayToTriangle(ray,triangle, nullptr, &colHitPos)) {
		ImGui::Text("hit Ray to triangle!");
		ImGui::Text("hitPos:(%2.2f,%2.2f,%2.2f)", colHitPos.x, colHitPos.y, colHitPos.z);
	}

	if (Collision::ColRayToPlane(ray, plane, nullptr, &colHitPos)) {
		ImGui::Text("hit Ray to Plane!");
		ImGui::Text("hitPos:(%2.2f,%2.2f,%2.2f)", colHitPos.x, colHitPos.y, colHitPos.z);
	}

	if (Collision::ColSphereToPlane(sphere,plane, &colHitPos)) {
		ImGui::Text("hit Plane to Sphere!");
		ImGui::Text("hitPos:(%2.2f,%2.2f,%2.2f)", colHitPos.x, colHitPos.y, colHitPos.z);
	}

	if (Collision::ColSphereToTriangle(sphere, triangle, &colHitPos)) {
		ImGui::Text("hit triangle to Sphere!");
		ImGui::Text("hitPos:(%2.2f,%2.2f,%2.2f)", colHitPos.x, colHitPos.y, colHitPos.z);
	}

	ImGui::End();

	skydomeObj->position = sphere.pos;
	skydomeObj->Update();
	planeObj->Update();

	rayObj->position = ray.start;
	rayObj->Update();

	//パーティクルを生成し続ける
	Vector3 particleEndPos1;
	Vector3 particleEndPos2;

	//パーティクル終点は始点からランダムに増減
	particleEndPos1 = particleStart1 + Vector3(Random(-10,10), Random(-10, 10), Random(-10, 10));
	particleEndPos2 = particleStart2 + Vector3(Random(-10,10), Random(-10, 10), Random(-10, 10));

	particle1->Add(ParticleManager::Type::Normal, 30, false, particleStart1, Vector3{ 0,0,0 }, particleEndPos1, 0.1f, 2,
		Vector4{ 1,1,1,1 }, Vector4{ 0,0,0,1 });

	particle2->Add(ParticleManager::Type::Normal, 30, false, particleStart2, Vector3{ 0,0,0 }, particleEndPos2,0.1f, 2,
		Vector4{ 0,0,0,1 }, Vector4{ 1,1,1,1 });

	particle1->Update();
	particle2->Update();

	sprite->SetSize(XMFLOAT2{ 64, 64 });
	sprite->SetPos(XMFLOAT2{ 700,0 });
	sprite->Update();

	sprite2->SetSize(XMFLOAT2{ 64, 64 });
	sprite2->Update();

	//----------------------ゲーム内ループはここまで---------------------//


}

void GamePlayScene::Draw()
{



	//-------背景スプライト描画処理-------//
	SpriteManager::GetInstance()->beginDraw();

	//backGroundSprite->Draw();
	// 

		//パーティクル描画処理
	ParticleManager::BeginDraw();

	particle1->Draw(camera);
	particle2->Draw(camera);

	ParticleManager::EndDraw();


	//-------3Dオブジェクト描画処理-------//
	Object3d::BeginDraw(camera);

	skydomeObj->Draw();
	//planeObj->Draw();
	triangleObj->Draw();
	rayObj->Draw();


	//-------前景スプライト描画処理-------//
	SpriteManager::GetInstance()->beginDraw();

	sprite->Draw();
	sprite2->Draw();
}
