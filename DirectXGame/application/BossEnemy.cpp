//�{�X�G�N���X
//�Q�[���Ō�ɓo�ꂷ��{�X�̏������s��p

#include "BossEnemy.h"
#include"ImguiManager.h"
#include"Util.h"
#include"DebugLine.h"
#include"Input.h"
#include"SphereCollider.h"
#include"CollisionAttribute.h"
#include"GameSceneManager.h"
using namespace Utility;
using namespace Easing;

void BossEnemy::Initialize(Model* bodyModel_, Model* barrelModel_, Object3d* parent_)
{
	//���f����null�`�F�b�N�ƃ����o�ւ̃Z�b�g
	assert(bodyModel_);
	assert(barrelModel_);
	bodyModel = bodyModel_;
	barrelModel = barrelModel_;
	TextureData* tex = Texture::LoadTexture("reticle.png");

	//HP�o�[�̐ݒ�
	healthTexture = Texture::LoadTexture("white1x1.png");
	healthSprite = std::make_unique<Sprite>();
	healthSprite->Initialize(healthTexture);
	Vector2 healthSize, healthPos, window;
	window = { WindowsAPI::winW,WindowsAPI::winH };

	healthSize = { window.x - 64.0f,32.0f };
	//1HP�������UI�̉������Ƃ��Ă���
	healthWidthOneHp = healthSize.x / lifeMax;
	healthSize.x = 0.0f;

	healthPos = { 32.0f,window.y - 80.0f };
	healthSprite->SetSize(healthSize);
	healthSprite->SetPos(healthPos);


	for (size_t i = 0; i < 4; i++) {
		sp[i].Initialize(tex);
		sp[i].SetAnchorPoint({ 0.5f,0.5f });
	}

	//�{�̃I�u�W�F�N�g(�p����)�̏�����
	Object3d::Initialize();
	SetModel(bodyModel);

	parent = parent_;

	bulletModel = std::make_unique<Model>();
	bulletModel = Model::CreateModel("PlayerBullet");

	//�C��I�u�W�F�N�g�̏������A���f���Z�b�g
	for (int i = 0; i < barrelObject.size(); i++) {
		barrelObject[i].Initialize();
		barrelObject[i].SetModel(barrelModel);
		barrelObject[i].parent = this;

		//�I�u�W�F�N�g���瓙�Ԋu�ɗ���
		Vector3 pos;
		barrelRadian[i] = 90.0f * i;
		pos.x = sinf((float)PI / 180.0f * barrelRadian[i]) * baseBarrelDistance;
		pos.y = cosf((float)PI / 180.0f * barrelRadian[i]) * baseBarrelDistance;
		barrelObject[i].position = pos;

	}

	//�e�ˏo���W�̃I�t�Z�b�g��ݒ�
	bulletOutOffset = { 0,0,17.0f };

	//�������W�ݒ�
	position = { 0,0,240.0f };
	Vector3 center = { 0,0,0 };
	Vector3 leftEdge = { -100,0,0 };
	Vector3 leftTop = { -50,50,0 };
	Vector3 leftBottom = { -50,-50,0 };
	Vector3 rightEdge = { 100,0,0 };
	Vector3 rightTop = { 50,50,0 };
	Vector3 rightBottom = { 50,-50,0 };


	std::vector<Vector3> movePoints{ center,leftTop,leftEdge,leftBottom,center,rightTop,rightEdge,rightBottom };
	for (Vector3& p : movePoints) {
		p += position;
	}

	//�e�N�[���^�C���̏�����
	moveCooltime[(INT32)BossAct::Spawn] = 30;
	moveCooltime[(INT32)BossAct::Move] = 30;
	moveCooltime[(INT32)BossAct::AttackShot] = 30;
	moveCooltime[(INT32)BossAct::AttackLaser] = 30;
	moveCooltime[(INT32)BossAct::Death] = 30;

	//�s�����ԏ�����
	actTime[(INT32)BossAct::Spawn] = 300;
	actTime[(INT32)BossAct::Move] = 120;
	actTime[(INT32)BossAct::AttackShot] = 200;
	actTime[(INT32)BossAct::AttackLaser] = 200;
	actTime[(INT32)BossAct::Death] = 300;

	//ChangeAct(BossAct::Spawn);

	//�R���C�_�[�̃Z�b�g
	SetCollider(new SphereCollider({ 0,0,0 }, 12.0f));
	collider->SetAttribute(COLLISION_ATTR_INVINCIBLE);

	//�p�[�e�B�N���}�l�[�W��
	particleManager = std::make_unique<ParticleManager>();
	particleManager->Initialize(Texture::LoadTexture("white1x1.png"));
}

void BossEnemy::Update(const Vector3& playerPos, EventCamera* eventCamera)
{
	targetPos = playerPos;
	eCamera = eventCamera;


	//�f�o�b�O�p:�L�[�Ŋe�s���J�n
	if (Input::GetInstance()->IsKeyTrigger(DIK_1)) {
		ChangeAct(BossAct::AttackShot);
	}
	else if (Input::GetInstance()->IsKeyTrigger(DIK_2)) {
		ChangeAct(BossAct::Spawn);
	}
	else if (Input::GetInstance()->IsKeyTrigger(DIK_3)) {
		ChangeAct(BossAct::AttackLaser);
	}

	//ImGui::SliderFloat("x", &position.x, -100.0f, 100.0f);
	//ImGui::SliderFloat("y", &position.y, -100.0f, 100.0f);
	//ImGui::SliderFloat("z", &position.z, -100.0f, 100.0f);
	//ImGui::SliderFloat("rotY", &rotation.y, -(float)PI,(float) PI);
	//Object3d::Update();

	for (size_t i = 0; i < shotPosMax; i++) {
		ImGui::Text("pos screen[%d] %f:%f", i, shotPos[i].x, shotPos[i].y);
	}



	//�e�̍X�V
	//����ł�e������
	bullets.remove_if([](std::unique_ptr<EnemyBullet>& bullet) {
		if (!bullet->IsAlive()) {
			return true;
		}
		return false;
		});


	for (std::unique_ptr<EnemyBullet>& bullet : bullets) {
		bullet->Update();
	}

	//����ł���Ȃ�X�V���Ȃ�
	if (!isAlive) {
		return;
	}

	ImGui::Text("life %d", life);

	//HP�o�[�̍X�V
	Vector2 sizeHp = healthSprite->GetSize();
	sizeHp.x = healthWidthOneHp * life;
	healthSprite->SetSize(sizeHp);

	//�_���[�W�̃C���^�[�o�������炷
	if (damageInterval > 0) {
		damageInterval--;
	}

	//�p�[�e�B�N���X�V
	particleManager->Update();

	//�s�����Ԃ����炷
	if (nowActTime > 0) {
		nowActTime--;

		ImGui::Text("act index %d", (INT32)bossAct);
		ImGui::Text("position %f,%f,%f", position.x, position.y, position.z);

		//�eact�ōX�V������
		switch (bossAct)
		{
		case BossAct::Spawn:
			UpdateSpawn();
			break;
		case BossAct::Move:
			UpdateMove();
			break;
		case BossAct::AttackShot:
			UpdateAtkShot();
			break;
		case BossAct::AttackLaser:
			UpdateAtkLaser();
			break;
		case BossAct::Death:
			UpdateDeath();
			break;
		default:
			break;
		}
	}
	else if (nowActTime == 0) {
		BossAct nextAct = BossAct::Move;

		switch (bossAct)
		{
		case BossAct::Spawn:



			break;
		case BossAct::Move:

			//TODO:�����_���Ŏˌ������[�U�[�����߂�

			//���͂Ƃ肠�����m��Ŏˌ�
			nextAct = BossAct::AttackShot;

			break;
		case BossAct::AttackShot:
			break;
		case BossAct::AttackLaser:
			break;
		case BossAct::Death:
			break;
		case BossAct::BossActMax:
			break;
		default:
			break;
		}

		ChangeAct(nextAct);
	}


}

void BossEnemy::Draw()
{
	for (std::unique_ptr<EnemyBullet>& bullet : bullets) {
		bullet->Draw();
	}


	//�̂ƖC��͎���ł�Ȃ�`�悵�Ȃ�(�e������O)
	if (!isAlive) {
		return;
	}

	//���f���̕`��
	Object3d::Draw();

	//TODO: �C��̕`��
	for (Object3d& barrel : barrelObject) {
		barrel.Draw();
	}
}

void BossEnemy::DrawSprite()
{
	if (!isAlive) {
		return;
	}

	for (size_t i = 0; i < 4; i++) {
		//sp[i].Draw();
	}

	healthSprite->Draw();
}

void BossEnemy::DrawParticle()
{
	particleManager->Draw();
}

void BossEnemy::DrawDebugLine()
{
	if (bossAct == BossAct::Move) {

		curvePoints.clear();
		for (size_t i = 0; i < 360; i++) {
			Vector3 point;
			float theta = (float)PI / 180.0f * i;

			point.x = sinf(theta * radianX) * amplitudeX;
			point.y = sinf(theta * radianY) * amplitudeY;
			point.z = 240.0f;

			curvePoints.push_back(point);
		}

		DebugLine::Draw(curvePoints, { 0,1,0,1 });

	}
}

void BossEnemy::Finalize()
{
}

void BossEnemy::Spawn(const Matrix4& cameraMatWorld, const Vector3& spawnPos)
{
	//�ڋʂ̈ړ��J�n�n�_��ݒ�(�J�����s��Ɗ|����)
	Vector3 posFirst = Matrix4::transform(spawnPos, cameraMatWorld);
	//�����t���O�ƃ��C�t�̐ݒ�
	isAlive = true;
	life = lifeMax;

	//�t�F�[�Y���X�|�[���ɂ���
	ChangeAct(BossAct::Spawn);
}

void BossEnemy::UpdateSpawn()
{
	eDataMove.Update();

	//�C�[�W���O�̐i�݋���擾(�v�Z���₷�����邽��100�{)
	INT32 easeProgress = (INT32)(eDataMove.GetTimeRate() * 100.0f);

	ImGui::Text("progress %d", easeProgress);



	//�ŏ���40%�ŖC��ŖC��𓮂���(10%����)
	if (easeProgress < 40) {
		//10%���ƂɃC�[�W���O���J�n���Ă���
		ImGui::Text("mod %d", easeProgress % 10);
		ImGui::Text("index %d", easeProgress / 10);
		ImGui::Text("count %d", count);

		position = movePosBefore;
		matRotation.identity();
		//rotation.y = -PI;

		if (easeProgress % 10 == 0) {
			size_t index = (size_t)easeProgress / 10;
			eDataBarrelMove[index].Start(actTime[(INT32)BossAct::Spawn] / 10.0f);
			count++;

			//2�ڂ̖C�䂪�ړ����n�߂���J�����������n�߂�
			if (index == 1) {
				StartCameraSpawn();
			}
		}


		for (size_t i = 0; i < barrelMax; i++) {
			eDataBarrelMove[i].Update();
			Vector3 pos = Vector3::Lerp(movePosBeforeBarrel[i], movePosAfterBarrel[i], eDataBarrelMove[i].GetTimeRate());
			barrelObject[i].position = pos;
		}

	}
	else if (easeProgress < 80) {
		//�ŏ��̈ړ�
		float t = (easeProgress - 40.0f) / 40.0f;

		Vector3 pos = Vector3::Lerp(movePosBefore, movePosAfter, Out(t));
		position = pos;
	}
	else {
		//�e��߂���z�����Z�b�g
		for (size_t i = 0; i < barrelMax; i++) {
			barrelObject[i].parent = this;
			Vector3 pos{};
			barrelRadian[i] = 90.0f * i;
			pos.x = sinf((float)PI / 180.0f * barrelRadian[i]) * baseBarrelDistance;
			pos.y = cosf((float)PI / 180.0f * barrelRadian[i]) * baseBarrelDistance;
			barrelObject[i].position = pos;
		}

		//��]
		float t = (easeProgress - 80) / 20.0f;
		rotation.y = Lerp(0, PI, Out(t));

	}

	//HP���X�|�[�����o�Ɉˑ����Ă�����������
	Vector2 sizeUI = healthSprite->GetSize();
	sizeUI.x = Lerp(0.0f, healthWidthOneHp * lifeMax, eDataMove.GetTimeRate());
	healthSprite->SetSize(sizeUI);



	//�e�ƖC��I�u�W�F�N�g�X�V
	Object3d::Update();
	for (size_t i = 0; i < barrelMax; i++) {

		barrelObject[i].Update();

		ImGui::Text("pos %f,%f,%f", barrelObject[i].position.x, barrelObject[i].position.y, barrelObject[i].position.z);
	}



	if (easeProgress >= 100.0f) {
		ChangeAct(BossAct::Move);
		collider->SetAttribute(COLLISION_ATTR_ENEMYS);
	}

}

void BossEnemy::UpdateMove()
{
	ImGui::SliderFloat("x", &position.x, -100.0f, 100.0f);
	ImGui::SliderFloat("y", &position.y, -100.0f, 100.0f);
	ImGui::SliderFloat("z", &position.z, -100.0f, 100.0f);

	lThetaSpd = 0.1f;

	lissajousTheta += lThetaSpd;
	if (lissajousTheta > 360.0f) {
		lissajousTheta -= 360.0f;
	}


	ImGui::SliderFloat("amplitude X", &amplitudeX, 0.0f, 200.0f);
	ImGui::SliderFloat("amplitude Y", &amplitudeY, 0.0f, 200.0f);

	if (ImGui::Button("X+")) {
		radianX++;
	}
	ImGui::SameLine();
	if (ImGui::Button("X-")) {
		radianX--;
	}
	ImGui::SameLine();
	ImGui::Text("radX:%1f", radianX);


	if (ImGui::Button("Y+")) {
		radianY++;
	}
	ImGui::SameLine();
	if (ImGui::Button("Y-")) {
		radianY--;
	}
	ImGui::SameLine();
	ImGui::Text("radY:%1f", radianY);

	Vector3 point;
	point.x = sinf(lissajousTheta * radianX * (float)PI / 180.0f) * amplitudeX;
	point.y = sinf(lissajousTheta * radianY * (float)PI / 180.0f) * amplitudeY;
	point.z = position.z;
	position = point;

	matRotation = Matrix4::CreateMatRot(GetWorldPosition(), targetPos, camera->up);

	Object3d::Update();

	for (size_t i = 0; i < barrelObject.size(); i++) {
		barrelRadian[i] += 1.0f;
		if (barrelRadian[i] >= 360.0f) {
			barrelRadian[i] -= 360.0f;
		}

		Vector3 pos;
		pos.x = sinf((float)PI / 180.0f * barrelRadian[i]) * baseBarrelDistance;
		pos.y = cosf((float)PI / 180.0f * barrelRadian[i]) * baseBarrelDistance;
		barrelObject[i].position = pos;

		barrelObject[i].Update();

	}
}

void BossEnemy::UpdateAtkShot()
{
	if (!isAlive) {
		return;
	}

	ImGui::Text("target %f,%f,%f", targetPos.x, targetPos.y, targetPos.z);

	//�ˌ����Ƃ̎���
	INT32 shotTimeOnce = (actTime[(INT32)BossAct::AttackShot] - 30) / shotPosMax;
	//���x�ڂ̎ˌ���
	size_t currentPosIndex = shotPosMax - (INT32)nowActTime / shotTimeOnce;
	if (nowActTime % shotTimeOnce == 0) {
		if (currentPosIndex < 3) {
			//movePosAfter = shotPos[currentPosIndex];
			movePosBefore = shotPos[currentPosIndex];
			movePosAfter = shotPos[currentPosIndex + 1];
			eDataMove.Start(20.0f);
		}
		else if (currentPosIndex == 3) {
			movePosBefore = shotPos[currentPosIndex];
			movePosAfter = lastPosActMove;
			eDataMove.Start(30.0f);
		}


	}
	else if (nowActTime % shotTimeOnce == 22) {
		//�C��̐������s��
		for (size_t i = 0; i < barrelMax; i++) {

			Vector3 outPos = Matrix4::transform(bulletOutOffset, barrelObject[i].matWorld) + barrelObject[i].GetWorldPosition();
			float bulletSpd = 4.0f;
			Vector3 vel = targetPos - outPos;
			vel.normalize();
			vel *= bulletSpd;
			std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
			newBullet->Initialize(bulletModel.get(), outPos, vel);

			bullets.push_back(std::move(newBullet));
		}

	}

	eDataMove.Update();
	if (eDataMove.GetTimeRate() < 1.0f) {
		float t = Out(eDataMove.GetTimeRate());
		position = Vector3::Lerp(movePosBefore, movePosAfter, t);
	}

	ImGui::Text("current index %d", currentPosIndex);
	ImGui::Text("before %f,%f,%f", movePosBefore.x, movePosBefore.y, movePosBefore.z);
	ImGui::Text("after %f,%f,%f", movePosAfter.x, movePosAfter.y, movePosAfter.z);

	matRotation = Matrix4::CreateMatRot(GetWorldPosition(), targetPos, camera->up);
	Object3d::Update();

	//�C����W�͏�ɐe�ɒǏ]
	for (size_t i = 0; i < barrelMax; i++) {
		barrelObject[i].Update();
	}

}

void BossEnemy::UpdateAtkLaser()
{
	eDataMove.Update();
	Vector3 pos = Vector3::Lerp(movePosBefore, movePosAfter, eDataMove.GetTimeRate());

	position = pos;

	Object3d::Update();
	for (size_t i = 0; i < barrelMax; i++) {
		barrelObject[i].Update();
		ImGui::Text("pos %f,%f,%f", barrelObject[i].position.x, barrelObject[i].position.y, barrelObject[i].position.z);
	}


}

void BossEnemy::UpdateDeath()
{

	eDataMove.Update();
	float moveRate = eDataMove.GetTimeRate();


	Vector3 cEye = GetWorldPosition();
	cEye.z -= 125.0f;
	//eCamera->SetEye(cEye);


	if (moveRate < 1.0f) {



		//�ړ���
		if (deathPhase == BossDeathPhase::Move) {

			//�C�x���g�J�����Z�b�g
			eCamera->SetTarget(GetWorldPosition());

			//��f��1�񔚔�������
			if (explosionCount > 0) {
				explosionCount--;
			}
			else {
				explosionCount = explosionCountMax;
				for (size_t i = 0; i < 32; i++) {
					Vector3 pos = GetWorldPosition();
					pos += {Random(-5.0f, 5.0f), Random(-5.0f, 5.0f), Random(-5.0f, 5.0f)};
					Vector3 vel = { Random(-1.0f,1.0f),Random(-1.0f,1.0f),Random(-1.0f,1.0f) };
					Vector3 acc = { Random(0.25f,-0.1f),Random(0.1f,-0.1f) ,Random(0.1f,-0.1f) };
					Vector3 rgb = { 255,Random(0,128),Random(0,64) };
					rgb = ConvertColor(rgb);
					particleManager->Add(20, pos, vel, acc, 0.0f, 10.0f, { rgb.x,rgb.y,rgb.z,1.0f });
				}
			}
		}
		else if (deathPhase == BossDeathPhase::Fall) {
			//�~��̪��ނȂ�timerate�̓C�[�W���O������
			moveRate = In(moveRate);
		}

		//���W���
		position = Vector3::Lerp(movePosBefore, movePosAfter, moveRate);
	}
	else {
		//�C�[�W���O�I������玀�S��������̪��ޕς�����
		if (deathPhase == BossDeathPhase::Move) {
			//���s���܂ł̃N�[���^�C������
			if (nextPhaseInterval > 0) {
				nextPhaseInterval--;
			}
			else {
				nextPhaseInterval = nextPhaseIntervalMax;
				//̪��ޕς���2�b�ŗ��Ƃ�
				deathPhase = BossDeathPhase::Fall;
				eDataMove.Start(120.0f);
				movePosBefore = position;
				movePosAfter = movePosBefore;
				movePosAfter.y -= 300.0f;

				//�J�����̈ړ�
				Vector3 cPos = targetPos;
				cPos.z -= 50.0f;

				eCamera->MoveEye(cPos, 60, InterType::EaseOut, false);
			}
		}
		else if (deathPhase == BossDeathPhase::Fall) {
			//�����t���O������
			isAlive = false;

		}
	}

	Object3d::Update();

	for (size_t i = 0; i < barrelMax; i++) {
		barrelObject[i].Update();
	}
}

void BossEnemy::InitSpawn()
{
	//�e(�J�����̃��[���h���)���Z�b�g����ĂȂ��Ȃ�return
	if (parent == nullptr) {
		return;
	}


	//�{�X�̃X�|�[���n�_�̓J�����̌��
	spawnPosOffsetCamera = Matrix4::transform({ 0,0,-50 }, parent->matWorld);

	//�ړ��p�C�[�W���O���J�n
	movePosBefore = spawnPosOffsetCamera;
	movePosAfter = Matrix4::transform({ 0,0,240 }, parent->matWorld);
	eDataMove.Start((float)actTime[(INT32)BossAct::Spawn]);

	life = 3;

	//�C��I�u�W�F�N�g���e�͈�U�J����obj�ɂ���(�ړ��I�������ڋ�obj�ɖ߂�)
	for (size_t i = 0; i < barrelMax; i++) {
		barrelObject[i].parent = parent;
		//Z�͖ڋʂƓ���
		//x,y�͈ړ���Ƃ��܂����킹��
		//�I�u�W�F�N�g���瓙�Ԋu�ɗ���
		Vector3 pos;
		barrelRadian[i] = 90.0f * i;
		pos.x = sinf((float)PI / 180.0f * barrelRadian[i]) * baseBarrelDistance;
		pos.y = cosf((float)PI / 180.0f * barrelRadian[i]) * baseBarrelDistance;
		pos.z = movePosBefore.z;
		movePosBeforeBarrel[i] = pos;
		pos.z = movePosAfter.z;
		movePosAfterBarrel[i] = pos;

		barrelObject[i].position = pos;
		//barrelObject[i].rotation.y = -PI;
		barrelObject[i].Update();
	}

	eDataBarrelMove[0].Start(actTime[(INT32)BossAct::Spawn] / 10.0f);

}

void BossEnemy::InitMove()
{
}

void BossEnemy::InitAtkShot()
{
	//�����_�����W�̃G���A
	float randomPosAreaX = WindowsAPI::winW / 2;
	float randomPosAreaY = WindowsAPI::winH / 2;

	for (size_t i = 0; i < shotPosMax; i++) {
		//x,y���W�̂݌��߂�(�X�N���[�����W�Ō��߂�)
		shotPos[i].x = Random(0, randomPosAreaX);
		shotPos[i].y = Random(0, randomPosAreaY);

		shotPos[i].x = randomPosAreaX / 2;
		shotPos[i].y = randomPosAreaY / 2;
	}
	//�I�t�Z�b�g
	shotPos[1].y += randomPosAreaY;
	shotPos[2].x += randomPosAreaX;
	shotPos[3].x += randomPosAreaX;
	shotPos[3].y += randomPosAreaY;
	for (size_t i = 0; i < 4; i++) {
		sp[i].SetPos({ shotPos[i].x,shotPos[i].y });
	}

	//��ذݍ��W�����[���h��
	float posZ = position.z;
	Matrix4 matViewPort;
	matViewPort.identity();
	matViewPort.m[0][0] = WindowsAPI::winW / 2.0f;
	matViewPort.m[1][1] = -(WindowsAPI::winH / 2.0f);
	matViewPort.m[3][0] = WindowsAPI::winW / 2.0f;
	matViewPort.m[3][1] = WindowsAPI::winH / 2.0f;

	Matrix4 matVBV = Object3d::camera->GetViewProjection() * matViewPort;

	for (size_t i = 0; i < shotPosMax; i++) {
		shotPos[i] = ConvertScreenToWorld({ shotPos[i].x,shotPos[i].y }, 240.0f, matVBV);
		shotPos[i].z = posZ;
	}


	//���ݍ��W���ړ��p���W�ɃZ�b�g
	movePosBefore = position;
	lastPosActMove = position;
	movePosAfter = shotPos.front();
	eDataMove.Start(20.0f);

	//�s���ύX

}

void BossEnemy::InitAtkLaser()
{
	//���ʂ�����
	matRotBefore = matRotation;
	matRotAfter.identity();

	Object3d::Update();
	//�{�̂�^�񒆂Ɉړ�
	eDataMove.Start(60.0f);
	//�ړ��O�͍��̍��W�A�ړ���̓J��������
	movePosBefore = position;
	movePosAfter = movePosBefore;
	movePosAfter.x = 0.0f;
	movePosAfter.y = 0.0f;
}

void BossEnemy::InitDeath()
{
	//���݂̍��W���ړ��O���W�Ƃ��ĕۑ�
	movePosBefore = position;
	//�ړ���̍��W�͒�����
	movePosAfter = movePosBefore;
	movePosAfter.x = 0.0f;
	movePosAfter.y = 0.0f;

	//���W��Ԃ̊J�n
	eDataMove.Start(120.0f);

	//�����𖳓G��
	collider->SetAttribute(COLLISION_ATTR_INVINCIBLE);

	//̪��ސݒ�
	deathPhase = BossDeathPhase::Move;

	//�C�x���g�J�����̈ړ�����W�ƒ����_�̐ݒ�
	Vector3 cameraEye, cameraTarget;
	cameraTarget = GetWorldPosition();
	cameraEye = cameraTarget;
	cameraEye.z -= 125.0f;

	eCamera->SetTarget(cameraTarget);
	eCamera->MoveEye(cameraEye, 30, InterType::EaseOut, false);
}

void BossEnemy::ChangeAct(BossAct nextAct)
{
	bossAct = nextAct;
	nowActTime = actTime[(INT32)nextAct];
	switch (nextAct)
	{
	case BossAct::Spawn:
		InitSpawn();
		break;
	case BossAct::Move:
		InitMove();
		break;
	case BossAct::AttackShot:
		InitAtkShot();
		break;
	case BossAct::AttackLaser:
		InitAtkLaser();
		break;
	case BossAct::Death:
		InitDeath();
		break;
	default:
		break;
	}
}

void BossEnemy::OnCollision([[maybe_unused]] const CollisionInfo& info)
{
	//�_���[�W���󂯂鏈��
	Damage(info.inter);

}

void BossEnemy::Damage(const Vector3& hitPos, uint16_t damage)
{
	//�_���[�W�̃N�[���^�C�����Ȃ�H���Ȃ�
	if (damageInterval > 0) {
		return;
	}
	damageInterval = damageIntervalMax;

	life -= damage;
	//�Փ˒n�_����p�[�e�B�N���̔���
	uint16_t particleCount = 32;
	for (uint16_t i = 0; i < particleCount; i++) {
		Vector3 particlePos = hitPos;
		particlePos += {Random(-1.0f, 1.0f), Random(-1.0f, 1.0f), Random(-1.0f, 1.0f)};
		Vector3 vel = { Random(-0.5f,0.5f),Random(-0.1f,-1.0f),0 };
		float rgbBase = Random(0.0f, 128.0f);
		Vector3 rgb = { rgbBase,rgbBase ,rgbBase };
		rgb = ConvertColor(rgb);
		particleManager->Add(15, particlePos, vel, { 0,0,0 }, 10.0f, 0.0f, { rgb.x,rgb.y,rgb.z ,1.0f });
	}

	//HP��0�ȉ��Ȃ����玀�S����
	if (life <= 0) {
		//�s�������S��
		ChangeAct(BossAct::Death);


	}
}

void BossEnemy::StartCameraSpawn()
{
	//1�ڂ̖C��ƍŌ�̖C��̒��_����J�����̘��Ս��W���Z�o
	Vector3 cameraEye = barrelObject.back().GetWorldPosition() - barrelObject.front().GetWorldPosition();
	cameraEye /= 2.0f;
	cameraEye += barrelObject.back().GetWorldPosition();

	//�����ɂ��炷
	cameraEye.x -= 100.0f;

	//�ړ��O��̍��W�̒��_�ɒ���
	Vector3 cameraTarget = targetPos;
	targetPos.x = 0.0f;
	targetPos.y = 0.0f;
	//cameraTarget.x;

	cameraEye = cameraTarget;
	cameraEye.x -= 100.0f;

	//�J�������[�N�p�x�W�G�Ȑ�
	BezierCurve bezireCurve;
	std::vector<Vector3> positions{ cameraTarget ,cameraTarget ,cameraTarget ,cameraTarget ,cameraTarget };
	//��]�ł���悤�ɍ��W�����������炷
	positions[1].x -= 100.0f;
	positions[2].z += 100.0f;
	positions[3].x += 100.0f;
	positions[4].z -= 100.0f;

	bezireCurve.SetPositions(positions);



	//�ړ��J�n
//	eCamera->MoveEye(cameraEye, 60, InterType::Lerp,false);

	eCamera->MoveEye(bezireCurve, 120, InterType::Lerp, false);
	eCamera->MoveTarget(cameraTarget, 120, InterType::EaseOut, false);

}
