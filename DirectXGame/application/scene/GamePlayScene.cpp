//�Q�[���V�[���N���X
//�Q�[���{�҂̏������s��

#include "GamePlayScene.h"
#include"Texture.h"
#include"DirectX.h"
#include"GameSceneManager.h"
#include"Collision.h"
#include"FbxLoader.h"
#include"FbxObject3d.h"
#include"Util.h"
#include"SphereCollider.h"
#include"DebugLine.h"

using namespace DirectX;
using namespace Utility;
using namespace Easing;

void GamePlayScene::Initialize()
{


	//--------------�Q�[�����ϐ�������--------------//

	//input�̃C���X�^���X�擾
	input = Input::GetInstance();

	//�Փ˃}�l�[�W���ɃC���X�^���X�擾
	collisionManager = CollisionManager::GetInstance();


	//�e�N�X�`���f�[�^������

	backGroundTexture = Texture::LoadTexture("dummyPlayGame.png");
	marioGraph = Texture::LoadTexture("mario.jpg");
	reimuGraph = Texture::LoadTexture("reimu.png");
	particleGraph = Texture::LoadTexture("particle.png");
	reticleGraph = Texture::LoadTexture("reticle.png");
	whiteGraph = Texture::LoadTexture("white1x1.png");
	backGroundSprite = std::make_unique<Sprite>();
	backGroundSprite->Initialize(backGroundTexture);

	//�摜�d���e�X�g
	testTex1 = Texture::LoadTexture("mario.jpg");
	testTex2 = Texture::LoadTexture("mario.jpg");
	testTex3 = Texture::LoadTexture("mario.jpg");

	texUIPlay = Texture::LoadTexture("UI_play.png");
	spriteUIPlay = std::make_unique<Sprite>();
	spriteUIPlay->Initialize(texUIPlay);
	spriteUIPlay->SetAnchorPoint({ 1.0f,0.0f });
	spriteUIPlay->SetPos({ WindowsAPI::winW,0.0f });

	skydome = std::make_unique<Model>();
	skydome = Model::CreateModel("skydome");

	defaultModel = std::make_unique<Model>();
	defaultModel = Model::CreateModel();
	defaultModel->SetTexture(reimuGraph);

	playerModel = std::make_unique<Model>();
	playerModel = Model::CreateModel("Player");

	bulletModel = std::make_unique<Model>();
	bulletModel = Model::CreateModel("PlayerBullet");

	enemyModel = std::make_unique<Model>();
	enemyModel = Model::CreateModel("ZakoEnemy");

	Enemy::EnemyInitialize(particleGraph, enemyModel.get(), bulletModel.get());


	skydomeObj = std::make_unique<Object3d>();
	skydomeObj->Initialize();
	skydomeObj->SetModel(skydome.get());
	Vector3 skydomeScale = { 1000.0f, 1000.0f,1000.0f };
	skydomeObj->scale = { skydomeScale };


	player = std::make_unique<Player>();
	player->Initialize(playerModel.get(), reticleGraph, whiteGraph);

	//�����蔻��e�X�g�p�I�u�W�F�N�g
	colTestObj = std::make_unique<Object3d>();
	colTestObj->Initialize();
	colTestObj->SetModel(defaultModel.get());
	//colTestObj->SetCollider(new SphereCollider({ 0,0,0 }, 1.0f));
	colTestObj->Update();


	newAudio = std::make_unique<AudioManager>();
	newAudio->SoundLoadWave("Resources/bgm_title.wav");

	//���f�������w�肵�ăt�@�C���ǂݍ���
	//model1 = std::make_unique<FbxModel>();
	model1 = FbxLoader::GetInstance()->LoadModelFromFile("boneTest");

	object1 = std::make_unique<FbxObject3d>();
	object1->Initialize();
	object1->SetModel(model1.get());



	//���[���J�����̏������ƃZ�b�g
	railCamera = std::make_unique< RailCamera>();
	railCamera->Initialize({ 0,0,0 }, { 0,0,0 });

	//���݂̃J�����������[���J�����̂��̂�
	currentCamera = railCamera->GetCamera();
	currentCamera->UpdateMatrix();

	//�C�x���g�V�[���p�J����
	eventCamera = std::make_unique<EventCamera>();
	eventCamera->Initialize({ 0,0,0 }, { 0,0,1 }, { 0,1,0 });

	//�f�o�C�X�Z�b�g
	FbxObject3d::SetCamera(currentCamera);

	//obj3d�N���X�ɃJ�������Z�b�g
	Object3d::SetCamera(railCamera->GetCamera());

	//�{�X�̏�����
	bossBodyModel = std::make_unique<Model>();
	bossBodyModel = Model::CreateModel("BossBody");

	bossBarrelModel = std::make_unique<Model>();
	bossBarrelModel = Model::CreateModel("BossBarrel");

	boss = std::make_unique<BossEnemy>();
	boss->Initialize(bossBodyModel.get(), bossBarrelModel.get(), railCamera->GetObject3d());

	//�X�e�[�W��obj�f�[�^�ǂݍ���
	stageData = JsonLoader::LoadJsonFile("gameStageData");

	//�ǂݍ��񂾃f�[�^�Ń��f���Aobj����
	JsonLoader::CreateObjectFromLevelData(stageData, stageObjects, stageModels);

	//�G�̔z�u�f�[�^�ǂݍ���
	enemyData = EnemyLoader::LoadEnemyData("enemyData.txt");

}

void GamePlayScene::Finalize()
{
	//�Q�[�����[�v�Ŏg�p�������������A��ՃV�X�e���̌㏈���Ɖ�����s��

	//delete sprite;
	//delete skyDome;

	//delete railCamera;

	//-------------�����܂łɃ��[�v���Ŏg�p�������̂̌㏈��------------//



}

void GamePlayScene::Update()
{


	//----------------------�Q�[�������[�v�͂�������---------------------//




#pragma region �f�o�b�O�p�R�[�h

	if (ImGui::Button("Enemy spwan")) {
		EnemySpawn();
	}

	if (Input::GetInstance()->IsKeyTrigger(DIK_0)) {
		EnemySpawn();
	}

	//�X�^�[�g�{�^���Ń��[���J�����J�n
	if (Input::GetInstance()->IsPadTrigger(XINPUT_GAMEPAD_START) || Input::GetInstance()->IsKeyTrigger(DIK_F2)) {
		railCamera->Start();
	}

	if (ImGui::Button("game start")) {
		railCamera->Start();
	}

#pragma endregion �f�o�b�O�p�R�[�h

	////���[���J�����̍X�V
	//railCamera->Update();

	////���[���J������e�ɂ���
	//player->parent = railCamera->GetObject3d();
	//player->Update(&enemys);

	////���@�̎��S���m�F���ꂽ��V�[���ړ�
	//if (!player->IsAlive()) {
	//	//�V�[���̐؂�ւ����˗�
	//	sceneManager->ChangeScene("GAMEOVER");
	//}

	////�G�z��̍X�V
	//for (std::unique_ptr<Enemy>& enemy : enemys) {
	//	enemy->Update(player->GetWorldPosition(), railCamera->GetObject3d()->matWorld);
	//}

	////����ł�G������
	//enemys.remove_if([](std::unique_ptr<Enemy>& enemy) {
	//	if (!enemy->IsAlive()) {
	//		return true;
	//	}
	//	return false;
	//	});




	////���[���J������5%�i�ނ��ƂɓG����̃X�|�[��
	//float cameraProgressPercent = railCamera->GetProgress() * 100.0f;
	//if (fmodf(cameraProgressPercent, 10.0f) == 0.0f && cameraProgressPercent != 0) {
	//	EnemySpawn();
	//}

	//Enemy::EnemyParticleUpdate();

	ImGui::SliderFloat("cameraX", &currentCamera->eye.x, -100.0f, 100.0f);
	ImGui::SliderFloat("cameraY", &currentCamera->eye.y, -100.0f, 100.0f);
	ImGui::SliderFloat("cameraZ", &currentCamera->eye.z, -100.0f, 100.0f);

	//camera->target = camera->eye;
	//camera->target.z += 1.0f;


	//�V���̑���
	ImGui::Begin("skydome");
	ImGui::SliderFloat("rotateY", &skydomeObj->rotation.y, 0.0f, 5.0f);
	ImGui::SliderFloat("scaleX", &skydomeObj->scale.x, 0.0f, 5.0f);
	ImGui::SliderFloat("scaleY", &skydomeObj->scale.y, 0.0f, 5.0f);
	ImGui::SliderFloat("scaleZ", &skydomeObj->scale.z, 0.0f, 5.0f);
	ImGui::End();

	//�V����������Ɖ�]������
	skydomeObj->rotation.y += PI / 14400.0f;
	skydomeObj->Update();

	//�A�j���[�V�����J�n�{�^��
	if (ImGui::Button("animation start")) {
		object1->PlayAnitimation();
	}

	if (input->IsKeyTrigger(DIK_A)) {
		object1->PlayAnitimation();
	}

	ImGui::Text("current:%d", object1->BaGetCurrentTime());

	//object1->SetRot({ 0,3.14f / 2,0 });
	object1->Update();

	//�S�t�F�[�Y���ʂ̍X�V
	UpdateAllPhase();

	//̪��ނ��Ƃ̍X�V
	switch (gamePhase)
	{
	case GamePhase::Event_GameStart:
		UpdateGameStart();
		break;
	case GamePhase::Game_Main:
		UpdateGamePhase();
		UpdateMain();
		//���݂̃J�����������[���J�����̂��̂�
		currentCamera = railCamera->GetCamera();

		break;
	case GamePhase::Event_BossSpawn:
		UpdateBossSpawn();
		currentCamera = eventCamera->GetCamera();

		break;
	case GamePhase::Game_Boss:
		UpdateGamePhase();
		UpdateBoss();
		//���݂̃J�����������[���J�����̂��̂�
		currentCamera = railCamera->GetCamera();
		break;
	case GamePhase::Event_GameClear:
		UpdateClear();
		currentCamera = eventCamera->GetCamera();
		break;
	default:
		break;
	}

	currentCamera->UpdateMatrix();

	ImGui::Begin("camera");
	ImGui::Text("current");
	ImGui::Text("eye %f,%f,%f", currentCamera->eye.x, currentCamera->eye.y, currentCamera->eye.z);
	ImGui::Text("target %f,%f,%f", currentCamera->target.x, currentCamera->target.y, currentCamera->target.z);
	ImGui::Text("rail");
	ImGui::Text("eye %f,%f,%f", railCamera->GetCamera()->eye.x, railCamera->GetCamera()->eye.y, railCamera->GetCamera()->eye.z);
	ImGui::Text("target %f,%f,%f", railCamera->GetCamera()->target.x, railCamera->GetCamera()->target.y, railCamera->GetCamera()->target.z);
	ImGui::Text("event");
	ImGui::Text("eye %f,%f,%f", eventCamera->GetCamera()->eye.x, eventCamera->GetCamera()->eye.y, eventCamera->GetCamera()->eye.z);
	ImGui::Text("target %f,%f,%f", eventCamera->GetCamera()->target.x, eventCamera->GetCamera()->target.y, eventCamera->GetCamera()->target.z);
	ImGui::End();


	//�����蔻��`�F�b�N
	collisionManager->CheckAllCollisions();


	//�X�y�[�X�L�[�Ń��C���Q�[����
	if (input->IsKeyTrigger(DIK_F1))
	{
		//�V�[���̐؂�ւ����˗�
		sceneManager->ChangeScene("TITLE");
	}



	//----------------------�Q�[�������[�v�͂����܂�---------------------//


}

void GamePlayScene::Draw()
{
	//-------�w�i�X�v���C�g�`�揈��-------//
	Sprite::BeginDraw();

	//backGroundSprite->Draw();

	//-------3D�I�u�W�F�N�g�`�揈��-------//
	Object3d::BeginDraw(currentCamera);


	//�V��
	skydomeObj->Draw();

	//�v���C���[
	player->Draw();

	colTestObj->Draw();

	//�G
	for (std::unique_ptr<Enemy>& enemy : enemys) {
		enemy->Draw();
	}

	//�{�X
	boss->Draw();

	//�X�e�[�W��obj�`��
	for (int i = 0; i < stageObjects.size(); i++) {
		stageObjects[i]->Draw();
	}

	//FBX
	//object1->Draw();

	//�p�[�e�B�N���`��
	ParticleManager::BeginDraw(currentCamera);

	player->DrawParticle();

	Enemy::DrawParticle();

	boss->DrawParticle();

	//------�f�o�b�O���`�揈��------//
	DebugLine::BeginDraw(currentCamera);

	//DebugLine::Draw({ { -10,0,100 }, { 20,30,100 } }, { 1,0,0,1 });
	//railCamera->DrawMoveLine({ 1,0,0,1 });

	//-------�O�i�X�v���C�g�`�揈��-------//
	Sprite::BeginDraw();

	player->DrawUI();

	spriteUIPlay->Draw();

	boss->DrawSprite();

}

void GamePlayScene::EnemySpawn()
{
	float posZ = 0.0f;


	//Z���W�̓J�������
	//posZ += railCamera->GetObject3d()->GetWorldPosition().z;

	//Vector3 start{ -60,0,posZ };
	//Vector3 p1 = { 0,30,posZ };
	//Vector3 p2 = { -30,0,posZ };
	//Vector3 p3 = { 0,-30,posZ };
	//Vector3 p4 = { 30,0,posZ };
	//Vector3 end = { 60,0,posZ };

	//Vector3 lTop, lBtm, rTop, rBtm;
	//lTop = { -60,30,posZ };
	//lBtm = { -60,-30,posZ };
	//rTop = { 60,30,posZ };
	//rBtm = { 60,-30,posZ };

	//std::vector<Vector3> enemyMovePoints = { start,p1,p2,p3,p4,end };
	//std::vector<Vector3> rightForLeftUpper;//�㑤�E���獶
	//std::vector<Vector3> rightForLeftLower;//�����E���獶
	//std::vector<Vector3> leftForRightUpper;//�㑤������E
	//std::vector<Vector3> leftForRightLower;//����������E

	//rightForLeftUpper = { rTop, rTop,lTop,lTop };
	//rightForLeftLower = { rBtm, rBtm,lBtm,lBtm };
	//leftForRightUpper = { lTop, lTop,rTop,rTop };
	//leftForRightLower = { lBtm, lBtm,rBtm,rBtm };


	////�Ȑ����J�������
	////for (auto& p : enemyMovePoints) {
	////	//p += railCamera->GetObject3d()->GetWorldPosition();
	////}

	//int pasent = (INT32)Random(0, 100) % 4;

	//if (pasent == 0) {
	//	enemyMovePoints = rightForLeftUpper;
	//}
	//else if (pasent == 1) {
	//	enemyMovePoints = rightForLeftLower;
	//}
	//else if (pasent == 2) {
	//	enemyMovePoints = leftForRightUpper;
	//}
	//else {
	//	enemyMovePoints = leftForRightLower;
	//}


	////�G�̐����Ə�����
	//std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();
	//newEnemy->Initialize(enemyMovePoints);
	//newEnemy->Spawn();

	//�G�̃f�[�^�z��𑖍�
	for (size_t i = 0; i < enemyData.size(); i++) {
		//�t���[���J�E���g�ŃX�|�[������G������΃X�|�[��������
		if (enemyData[i].spawnTime == frameCount) {

			std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();
			Vector3 newEnemyPosition = { 0,0,1 };
			newEnemyPosition.x = enemyData[i].spawnPos.x;
			newEnemyPosition.y = enemyData[i].spawnPos.y;

			//�X�N���[�����烏�[���h�ɕϊ�
			Matrix4 matScreen2World;
			matScreen2World.identity();
			matScreen2World.m[0][0] = WindowsAPI::winW / 2.0f;
			matScreen2World.m[1][1] = -(WindowsAPI::winH / 2.0f);
			matScreen2World.m[3][0] = WindowsAPI::winW / 2.0f;
			matScreen2World.m[3][1] = WindowsAPI::winH / 2.0f;

			matScreen2World = Object3d::camera->GetViewProjection() * matScreen2World;

			matScreen2World.Inverse();

			newEnemyPosition = Matrix4::transformDivW(newEnemyPosition, matScreen2World);


			posZ = Utility::Random(100.0f, 150.0f);
			newEnemyPosition.z = posZ;
			
			newEnemyPosition.x = enemyData[i].spawnPos.x;
			newEnemyPosition.y = enemyData[i].spawnPos.y;


			newEnemy->Initialize(newEnemyPosition, enemyData[i].leaveTime);
			newEnemy->Spawn();

			//���X�g�ɓo�^
			enemys.push_back(std::move(newEnemy));

			//�f�[�^�z�񂩂�Y���f�[�^���폜+for����1�񊪂��߂�
			enemyData.erase(enemyData.begin() + i);
			i--;

		}

	}

}

void GamePlayScene::EnemySpawnRandom()
{
	float posZ = Utility::Random(100.0f, 150.0f);


	//Z���W�̓J�������
	//posZ += railCamera->GetObject3d()->GetWorldPosition().z;

	Vector3 start{ -60,0,posZ };
	Vector3 p1 = { 0,30,posZ };
	Vector3 p2 = { -30,0,posZ };
	Vector3 p3 = { 0,-30,posZ };
	Vector3 p4 = { 30,0,posZ };
	Vector3 end = { 60,0,posZ };

	Vector3 lTop, lBtm, rTop, rBtm;
	lTop = { -60,30,posZ };
	lBtm = { -60,-30,posZ };
	rTop = { 60,30,posZ };
	rBtm = { 60,-30,posZ };

	std::vector<Vector3> enemyMovePoints = { start,p1,p2,p3,p4,end };
	std::vector<Vector3> rightForLeftUpper;//�㑤�E���獶
	std::vector<Vector3> rightForLeftLower;//�����E���獶
	std::vector<Vector3> leftForRightUpper;//�㑤������E
	std::vector<Vector3> leftForRightLower;//����������E

	rightForLeftUpper = { rTop, rTop,lTop,lTop };
	rightForLeftLower = { rBtm, rBtm,lBtm,lBtm };
	leftForRightUpper = { lTop, lTop,rTop,rTop };
	leftForRightLower = { lBtm, lBtm,rBtm,rBtm };


	//�Ȑ����J�������
	//for (auto& p : enemyMovePoints) {
	//	//p += railCamera->GetObject3d()->GetWorldPosition();
	//}

	int pasent = (INT32)Random(0, 100) % 4;

	if (pasent == 0) {
		enemyMovePoints = rightForLeftUpper;
	}
	else if (pasent == 1) {
		enemyMovePoints = rightForLeftLower;
	}
	else if (pasent == 2) {
		enemyMovePoints = leftForRightUpper;
	}
	else {
		enemyMovePoints = leftForRightLower;
	}


	//�G�̐����Ə�����
	std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();
	newEnemy->Initialize(enemyMovePoints);
	newEnemy->Spawn();

	//���X�g�ɓo�^
	enemys.push_back(std::move(newEnemy));
}

void GamePlayScene::UpdateAllPhase()
{
	//���[���J�����̍X�V
	railCamera->Update();



	//���[���J������e�ɂ���
	player->parent = railCamera->GetObject3d();
	player->Update(&enemys);



}

void GamePlayScene::UpdateGamePhase()
{


	//���@�̎��S���m�F���ꂽ��V�[���ړ�
	if (!player->IsAlive()) {
		//�V�[���̐؂�ւ����˗�
		sceneManager->ChangeScene("GAMEOVER");
	}

}

void GamePlayScene::UpdateGameStart()
{
	//���@�̃X�|�[���I���Ń��[���J�������J�n
	if (player->GetSpawnTimer() == 0) {
		railCamera->Start(600.0f);

		//̪��ނ����C���Q�[���Ɉڍs
		gamePhase = GamePhase::Game_Main;

		//�t���[���J�E���g�����Z�b�g
		frameCount = 0;
	}

}

void GamePlayScene::UpdateMain()
{
	//�G�z��̍X�V
	for (std::unique_ptr<Enemy>& enemy : enemys) {
		enemy->Update(player->GetWorldPosition(), railCamera->GetObject3d()->matWorld);
		//���[���J�������i�s����������G��S����
		if (railCamera->GetProgress() >= 1.0f) {
			//enemy->Death();
		}
	}

	ImGui::Text("enemy count %d", enemys.size());

	//�Q�[���t�F�[�Y���{�X��(�{�X��C�x���g)�ɂ��ă{�X���X�|�[��
	if (railCamera->GetProgress() >= 1.0f) {

		//�G���S���|���ꂽ��Q�[���i�s
		if (enemys.empty()) {

			boss->Spawn(railCamera->GetObject3d()->matWorld);

			////�{�X�X�|�[���֑J��
			gamePhase = GamePhase::Event_BossSpawn;
			////���[���J�����̍��W���C�x���g�J�����ɂ��R�s�[
			eventCamera->SetEye(railCamera->GetCamera()->eye);
			eventCamera->SetTarget(railCamera->GetCamera()->target);

		}
	}
	else {
		//�G�o������p�̃t���[���J�E���g��i�߂�
		frameCount++;

		EnemySpawn();
		//���[���J������5%�i�ނ��ƂɓG����̃X�|�[��
		float cameraProgressPercent = railCamera->GetProgress() * 100.0f;
		if (fmodf(cameraProgressPercent, 10.0f) == 0.0f && cameraProgressPercent != 0) {
			//EnemySpawnRandom();
		}
	}

	//����ł�G������
	enemys.remove_if([](std::unique_ptr<Enemy>& enemy) {
		if (!enemy->IsAlive()) {
			return true;
		}
		return false;
		});



	//�G�̃p�[�e�B�N���X�V
	Enemy::EnemyParticleUpdate();

}

void GamePlayScene::UpdateBossSpawn()
{


	//�{�X�̍X�V
	boss->Update(player->GetWorldPosition(), eventCamera.get());
	//�J�����X�V
	eventCamera->Update();

	//�����Ń{�X��֑J��
	if (boss->GetBossAct() == BossAct::Move) {
		gamePhase = GamePhase::Game_Boss;
	}

	//�Ō�̓G������ł��p�[�e�B�N�����X�V�����悤��
	Enemy::EnemyParticleUpdate();

}

void GamePlayScene::UpdateBoss()
{


	boss->Update(player->GetWorldPosition(), eventCamera.get());

	//�{�X��HP��0�ɂȂ�����Q�[���N���A���o�J�n
	if (boss->GetHealth() == 0) {

		//���@�̉��o�J�n
		//player->Leave();


		//sceneManager->ChangeScene("GAMECLEAR");

		//�C�x���g�J�����̈ړ��ƃZ�b�g
		//eventCamera->SetEye(railCamera->GetCamera()->eye);
		//eventCamera->SetTarget(railCamera->GetCamera()->target);
		Vector3 cameraTargetAfter = eventCamera->GetCamera()->target;
		cameraTargetAfter += {10, 20, 30};
		//eventCamera->MoveTarget(cameraTargetAfter, 60, InterType::EaseIn);
		//�Q�[���t�F�[�Y�̕ύX
		gamePhase = GamePhase::Event_GameClear;

		//���[���J�����̍��W���C�x���g�J�����ɂ��R�s�[
		eventCamera->SetEye(railCamera->GetCamera()->eye);
		eventCamera->SetTarget(railCamera->GetCamera()->target);

	}
}

void GamePlayScene::UpdateClear()
{

	//�{�X
	boss->Update(player->GetWorldPosition(), eventCamera.get());

	//�C�x���g�J�����̍X�V
	eventCamera->Update();

	//�{�X�����񂾂�N���A�V�[����
	if (!boss->IsAlive()) {
		sceneManager->ChangeScene("GAMECLEAR");
	}

}