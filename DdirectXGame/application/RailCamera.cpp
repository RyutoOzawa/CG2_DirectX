#include "RailCamera.h"
#include"ImguiManager.h"
#include"Util.h"

RailCamera::~RailCamera()
{
	delete camera;
	delete world;

}

void RailCamera::Initialize(const Vector3& position, const Vector3& rotation)
{
	world = new Object3d();
	camera = new Camera();

	//ワールド座標初期設定
	world->Initialize();
	world->position = position;
	world->rotation = rotation;
	world->Update();

	Vector3 eye = { 0,0,0 };
	Vector3 target = { 0,0,10 };
	Vector3 up = { 0,1,0 };

	camera->Initialize(eye, target, up);


	Vector3 start{ 0,0,0 };
	Vector3 p1{ -30,50,-50 };
	Vector3 p2{ 30,-50,50 };
	Vector3 end{ 100,0,-100 };

	std::vector<Vector3> c{ start,p1,p2,end };

	spline.SetPositions(c);


}

void RailCamera::Update()
{
	//レールカメラ用デバッグテキスト
	ImGui::Begin("railCamera");

	if (ImGui::Button("start spline")) {
		spline.Start(240.0f);
	}

	spline.Update();

	//ワールド座標の更新
	ImGui::SliderFloat("posX", &world->position.x, -100.0f, 100.0f);
	ImGui::SliderFloat("posY", &world->position.y, -100.0f, 100.0f);
	ImGui::SliderFloat("posZ", &world->position.z, -100.0f, 100.0f);

	//スプライン曲線に沿って移動
	world->position = spline.GetPosition();

	Vector3 rotation = world->rotation;
	ImGui::SliderFloat("rotX", &rotation.x, (float)-PI, (float)PI);
	ImGui::SliderFloat("rotY", &rotation.y, (float)-PI, (float)PI);
	ImGui::SliderFloat("rotZ", &rotation.z, (float)-PI, (float)PI);
	world->rotation = rotation;

	world->Update();

	camera->eye = world->position;
	//ワールド前方ベクトル
	Vector3 forward(0, 0, 1);
	//レールカメラの回転を反映
	forward = Matrix4::transform(forward, world->matWorld);
	//視点+前方ベクトルで注視点
	camera->target = camera->eye + forward;
	camera->target = forward;
	//上方向ベクトル
	Vector3 up(0, 1, 0);
	//カメラの上方向をforwardと同様に変換
	//camera->up = Matrix4::transform(up, world->matWorld);
	//クリップ距離を変更 
	camera->farZ = 2000.0f;
	//カメラ更新
	camera->UpdateMatrix();


	ImGui::Text("eye:%f,%f,%f", camera->eye.x, camera->eye.y, camera->eye.z);
	ImGui::Text("target:%f,%f,%f", camera->target.x, camera->target.y, camera->target.z);
	ImGui::End();

}
