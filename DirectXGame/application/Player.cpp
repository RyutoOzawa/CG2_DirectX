//�v���C���[�N���X
//���[�U�[�����삷��v���C���[�S�ʂ̏����͂����ōs��

#include "Player.h"
#include"Input.h"
#include"SphereCollider.h"
#include"Util.h"
#include"CollisionAttribute.h"
#include"Enemy.h"
#include"Collision.h"
#include"ImguiManager.h"

void Player::Initialize(Model* model_, TextureData* reticleTexture, TextureData* healthTexture)
{
	Object3d::Initialize();
	SetModel(model_);

	position = { 0,-5.0f,distanceCamera };

	//�R���C�_�[�̒ǉ�
	float radius = 1.0f;
	//���a���������������W�����̒��S�ɂ���
	SetCollider(new SphereCollider(Vector3(0, 0, 0),radius));
	collider->SetAttribute(COLLISION_ATTR_ALLIES);


	//�����p�[�e�B�N��
	hitParticle = std::make_unique<ParticleManager>();
	hitParticle->Initialize(Texture::LoadTexture("particle.png"));

	//���e�B�N���I�u�W�F�N�g
	reticleObj = std::make_unique<Object3d>();
	reticleObj->Initialize();
	//���f���̓I�u�W�F�N�g�Ɠ�������
	reticleObj->SetModel(Object3d::model);

	//���e�B�N���X�v���C�g
	reticleSprite = std::make_unique<Sprite>();
	reticleSprite->Initialize(reticleTexture);
	reticleSprite->SetAnchorPoint({ 0.5f,0.5f });
	reticleSprite->SetColor({ 1,1,1,0.5f });

	//HP�̏�����
	health = healthMax;
	healthWidthMax = 720;

	//HP�X�v���C�g
	healthSprite = std::make_unique<Sprite>();
	healthSprite->Initialize(healthTexture);
	healthSprite->SetPos({ 32,32 });
	healthSprite->SetSize({ 720,32 });
	Vector3 healthColor = ColorCodeRGB(0x72b876);
	healthSprite->SetColor({ healthColor.x,healthColor.y,healthColor.z,1.0f });
	damageInterval = 0;
	isAlive = true;

	Vector2 window = { WindowsAPI::winW,WindowsAPI::winH };
	damageSprite = std::make_unique<Sprite>();
	damageSprite->Initialize(Texture::LoadTexture("player_lowhp.png"));
	damageSprite->SetSize(window);

	//���փ��f������
	haloModel = std::make_unique<Model>();
	haloModel = Model::CreateModel(MODEL_PLANE);
	haloModel->SetTexture(Texture::LoadTexture("halo.png"));

	//�e���f��
	bulletModel = std::make_unique<Model>();
	bulletModel = Model::CreateModel("PlayerBullet");

	for (std::unique_ptr<Object3d>& haloObject : haloObjects) {
		haloObject = std::make_unique<Object3d>();
		haloObject->Initialize();
		haloObject->SetModel(haloModel.get());
	}

	//�X�|�[������
	Spawn();
}

void Player::Spawn()
{
	for (size_t i = 0; i < haloMax; i++) {
		float sc = Random(1.0f, 3.0f);
		haloObjects[i]->scale = { sc ,sc ,sc };

		haloObjects[i]->color.z = Random(0.8f, 1.0f);

		//�傫���̑��x�������_����
		haloScaleVel[i] = Random(0.5f, 2.5f);

		//�F�̓��ߑ��x�������_����
		haloAlphaVel[i] = Random(0.01f, 0.05f);
//		haloAlphaVel[i] = 0.01f;

		haloRotaVel[i] = { Random(0,PI / 16.0f),Random(0,PI / 16.0f) ,Random(0,PI / 16.0f) };

		//�p�x�������_����
		haloObjects[i]->rotation = { Random(-PI,PI),Random(-PI,PI) ,Random(-PI,PI) };
		haloObjects[i]->parent = this;
		haloObjects[i]->color.w = 1.0f;
		haloObjects[i]->Update();

	}

	//�^�C�}�\�Z�b�g
	spawnTimer = spawnTimerMax;

	isSpawn = true;
}

void Player::Update(std::list<std::unique_ptr<Enemy>>* enemys)
{
	ImGui::Begin("Player");

	ImGui::Text("health %d", health);

	if (isAlive) {
		ImGui::Text("alive");
	}
	else {
		ImGui::Text("alive false");
	}



	if (Input::GetInstance()->IsKeyTrigger(DIK_2)) {
		Spawn();
	}

	//UI�̃A���t�@�l���X�V
	easeUIAlpha.Update();
	UIAlpha = Lerp(0.0f, 1.0f,  InBounce(easeUIAlpha.GetTimeRate()));


	if (isSpawn) {
		UpdateSpawn();
		UIAlpha = 0.0f;
	}

	//�A���t�@�l��UI�ɓK�p
	Vector4 uiColor;
	uiColor = healthSprite->GetColor();
	uiColor.w = UIAlpha;
	healthSprite->SetColor(uiColor);
	uiColor = reticleSprite->GetColor();
	uiColor.w = UIAlpha;
	reticleSprite->SetColor(uiColor);



	//����ł�e������
	bullets.remove_if([](std::unique_ptr<PlayerBullet>& bullet) {
		if (!bullet->IsAlive()) {
			return true;
		}
		return false;
		});

	//���@��HP��0�A�X�|�[���A�j�����Ȃ瑀�삳���Ȃ�
	if (health != 0  && !isSpawn) {

		//�ړ�
		Move();

		//�U��
		Attack();

		//���e�B�N���̃I�u�W�F�N�g�f�[�^�X�V
		ReticleUpdate(enemys);
	}
	//0�Ȃ玀�ʃA�j���[�V����
	else if (health == 0) {
		UpdateDeath();
	}

	//�e�̍X�V
	for (std::unique_ptr<PlayerBullet>& bullet : bullets) {
		bullet->Update();
	}

	ImGui::Text("bullet %d", bullets.size());

	//obj3d�̍X�V
	Object3d::Update();
	//�������p�[�e�B�N���X�V
	hitParticle->Update();
	//HP�o�[�̍X�V
	HealthUpdate();

	ImGui::End();

}

void Player::Draw()
{
	//�����̕`��

	//�_���[�W�̃N�[���^�C���ɂ���ē_��
	//�����ɂ���Ă̕`��t���O
	bool isPlayerDraw = (health != 0 || deathCount > 0);

	if (damageInterval % 4 < 1 && isPlayerDraw) {
		if (spawnTimer <= 0) {
			Object3d::Draw();
		}
	}

	//�e�̕`��
	for (std::unique_ptr<PlayerBullet>& bullet : bullets) {
		bullet->Draw();
	}

	//�X�|�[�����̌���
	if (isSpawn) {
		for (std::unique_ptr<Object3d>& haloObject : haloObjects) {
			haloObject->Draw();
		}
	}

	//reticleObj.Draw();
}

void Player::DrawParticle()
{
	hitParticle->Draw();
	
}

void Player::DrawUI()
{
	reticleSprite->Draw();
	healthSprite->Draw();
	damageSprite->Draw();
}

void Player::OnCollision([[maybe_unused]] const CollisionInfo& info)
{

	//�_���[�W�̃N�[���^�C�����I����ĂȂ��Ȃ牽�����Ȃ�
	if (damageInterval > 0) {
		return;
	}

	//�p�[�e�B�N���̑��x
	for (int i = 0; i < 25; i++) {
		Vector3 vel = { 0,0,0 };
		float absAcc = 0.5f;
		Vector3 acc = { Random(-absAcc,absAcc),Random(-absAcc,absAcc) ,Random(-absAcc,absAcc) };


		//�p�[�e�B�N���ǉ�
		hitParticle->Add(15, GetWorldPosition(), vel, acc, 6.0f, 0.0f);
	}

	//�_���[�W���󂯂�
	Damage();

}


void Player::Move()
{
	//����
	INT32 inputHorizontal = 0;
	INT32 inputVertical = 0;

	if (Input::GetInstance()->IsKeyPress(DIK_A) || Input::GetInstance()->IsDownLStickLeft()) {
		inputHorizontal = -1;
	}
	else if (Input::GetInstance()->IsKeyPress(DIK_D) || Input::GetInstance()->IsDownLStickRight()) {
		inputHorizontal = 1;
	}

	if (Input::GetInstance()->IsKeyPress(DIK_W) || Input::GetInstance()->IsDownLStickUp()) {
		inputVertical = 1;
	}
	else if (Input::GetInstance()->IsKeyPress(DIK_S) || Input::GetInstance()->IsDownLStickDown()) {
		inputVertical = -1;
	}

	//���W�ɉ��Z���鑬�x
	Vector3 spd{};
	float baseSpd = 0.5f;


	spd.x += baseSpd * inputHorizontal;
	spd.y += baseSpd * inputVertical;
	



	//���ݍ��W���擾
	localPos = position;
	//���Z���đ��
	localPos += spd;
	position = localPos;

	const float bodyTurnBase = (float)PI / 180.0f;

	//����̋���
	rotation.z -= bodyTurnBase * inputHorizontal;
	rotation.x -= bodyTurnBase * inputVertical;

	//���͂��Ȃ����
	if (inputHorizontal == 0) {
		if (rotation.z > 0 + bodyTurnBase) {
			rotation.z -= bodyTurnBase * 3.0f;
		}
		else if (rotation.z < 0 - bodyTurnBase) {
			rotation.z += bodyTurnBase * 3.0f;
		}
		
		if(fabs(rotation.z) <= bodyTurnBase*3.0f) {
			rotation.z = 0.0f;
		}
	}

	if (inputVertical == 0) {
		if (rotation.x > 0 + bodyTurnBase) {
			rotation.x -= bodyTurnBase * 3.0f;
		}
		else if (rotation.x < 0 - bodyTurnBase) {
			rotation.x += bodyTurnBase * 3.0f;
		}
		
		if (fabs(rotation.x) <= bodyTurnBase * 3.0f) {
			rotation.x = 0.0f;
		}
	}


	//�X���̌��E�𒴂��Ȃ��悤��
	if (fabs(rotation.x) > (float)PI /180.0f *15.0f) {
		rotation.x = -inputVertical * (float)PI / 180.0f * 15.0f;
	}

	if (fabs(rotation.z) > (float)PI / 180.0f * 15.0f) {
		rotation.z = -inputHorizontal * (float)PI / 180.0f * 15.0f;
	}


}

void Player::Attack()
{
	//�X�y�[�X�L�[�Œe����
	if (shotInterval == 0) {

		if (Input::GetInstance()->IsKeyPress(DIK_SPACE) || Input::GetInstance()->IsPadPress(XINPUT_GAMEPAD_RIGHT_SHOULDER)) {
			
			shotInterval = shotCooltime;

			//�e�̑��x
			const float bulletSpdBase = 32.0f;
			Vector3 velocity(0, 0, bulletSpdBase);

			velocity = reticleObj->GetWorldPosition() - Object3d::GetWorldPosition();
			velocity.normalize();
			velocity *= bulletSpdBase;

			//�e�̐����Ə�����
			std::unique_ptr< PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
			newBullet->Initialize(bulletModel.get(), GetWorldPosition(), velocity);

			//�e�̓o�^
			bullets.push_back(std::move(newBullet));
		}
	}
	else {
		shotInterval--;
	}


}

void Player::ReticleUpdate(std::list<std::unique_ptr<Enemy>>* enemys)
{
	reticleObj->SetModel(bulletModel.get());

	//�r���[�|�[�g�s��
	Matrix4 matViewPort;
	matViewPort.identity();
	matViewPort.m[0][0] = WindowsAPI::winW / 2.0f;
	matViewPort.m[1][1] = -(WindowsAPI::winH / 2.0f);
	matViewPort.m[3][0] = WindowsAPI::winW / 2.0f;
	matViewPort.m[3][1] = WindowsAPI::winH / 2.0f;

	//�J�����s��Ƃ̍���
	Matrix4 matViewProViewPort = Object3d::camera->GetViewProjection() * matViewPort;


	//��ʏ�̃��e�B�N�����W�𓮂���
	Vector2 reticleSpd = { 0,0 };
	float reticleSpdBase = 8.0f;

	//����
	INT32 inputHorizontal = 0;
	INT32 inputVertical = 0;

	if (Input::GetInstance()->IsKeyPress(DIK_LEFT) || Input::GetInstance()->IsDownRStickLeft()) {
		inputHorizontal = -1;
	}
	else if (Input::GetInstance()->IsKeyPress(DIK_RIGHT) || Input::GetInstance()->IsDownRStickRight()) {
		inputHorizontal = 1;
	}

	if (Input::GetInstance()->IsKeyPress(DIK_UP) || Input::GetInstance()->IsDownRStickUp()) {
		inputVertical = -1;
	}
	else if (Input::GetInstance()->IsKeyPress(DIK_DOWN) || Input::GetInstance()->IsDownRStickDown()) {
		inputVertical = 1;
	}


	
	//���͒l�~���e�B�N���X�s�[�h�ňړ�
	reticleSpd.x = inputHorizontal * reticleSpdBase;
	reticleSpd.y = inputVertical * reticleSpdBase;
	reticlePosScreen += reticleSpd;

	//���e�B�N�����W�̈ړ�����
	Vector2 reticlePosMin = { reticleRadius,reticleRadius };
	Vector2 reticlePosMax = { WindowsAPI::winW - reticleRadius,WindowsAPI::winH - reticleRadius };
	if (reticlePosScreen.x > reticlePosMax.x) {
		reticlePosScreen.x = reticlePosMax.x;
	}
	else if (reticlePosScreen.x < reticlePosMin.x) {
		reticlePosScreen.x = reticlePosMin.x;
	}

	if (reticlePosScreen.y > reticlePosMax.y) {
		reticlePosScreen.y = reticlePosMax.y;
	}
	else if (reticlePosScreen.y < reticlePosMin.y) {
		reticlePosScreen.y = reticlePosMin.y;
	}

	//���W���X�v���C�g�ɃZ�b�g
	reticleSprite->SetPos(reticlePosScreen);

	//���e�B�N���̐F��������
	reticleColor = reticleSprite->GetColor();
	reticleColor.z = 1.0f;

	

	//�J�������烌�e�B�N��(3D)�ւ̋���
	float distanceReticle3D = distanceCamera + 100.0f;

	//�X�N���[�����W�ϊ��������@�̍��W
	Vector3 posPlayerScreen = Matrix4::transformDivW(GetWorldPosition(), matViewProViewPort);

	//ImGui::Text("screen Z player %f", posPlayerScreen.z);

	//�G�̍��W���X�N���[�����W�ɕϊ�
	std::list<std::unique_ptr<Enemy>>::iterator itE;
	itE = enemys->begin();
	for (; itE != enemys->end(); itE++) {
		//�G�̃��[���h���W���擾
		Vector3 posEnemyWorld = itE->get()->GetWorldPosition();

		//�r���[�A�v���W�F�N�V�����A�r���[�|�[�g�s����|����W���Z
		posEnemyWorld = Matrix4::transformDivW(posEnemyWorld, matViewProViewPort);
		Vector2 posEnemyScreen = { posEnemyWorld.x,posEnemyWorld.y };
		
		//ImGui::Text("screen Z eyemy %f", posEnemyWorld.z)
		Circle reticleC{ reticlePosScreen,reticleRadius };
		Circle enemyC{ posEnemyScreen,1.0f };

		//�J�������烌�e�B�N���̋�����(�G�̃��[���h�~�r���[)��z�ɂ���
		Matrix4 matEnemyView = itE->get()->GetMatWorld() * camera->GetViewProjection();

		ImGui::Text("posenemyView %f,%f,%f", matEnemyView.m[3][0], matEnemyView.m[3][1], matEnemyView.m[3][2]);

		//ImGui::Text("enemyView.z %f", posEnemyView.z);



		//���e�B�N�����G�ɓ������Ă���Ȃ�
		if (Collision::ColCircleToCircle(reticleC, enemyC)) {
			
			//�J��������G�̋��������@�Ƃ̋�����菬�����Ȃ�_��Ȃ�
			if (matEnemyView.m[3][2] < distanceCamera) {
				continue;
			}

			//distanceReticle3D = matEnemyView.z;
			//���e�B�N���������Ă���Ȃ烍�b�N�I��
			if (inputHorizontal != 0 || inputVertical != 0) {
				reticlePosScreen = posEnemyScreen;
				reticleColor.z = 0.0f;
			}
		}
	}

	//���e�B�N���̐F��ݒ�
	reticleSprite->SetColor(reticleColor);

//	ImGui::Text("distance retilce3d %f", distanceReticle3D);

	//�r���[�A�ˉe�A�r���[�|�[�g�̍s�������
	Matrix4 matVBVInverse = matViewProViewPort;
	matVBVInverse.Inverse();

	//�X�N���[�����W
	Vector3 posNear = { reticlePosScreen.x, reticlePosScreen.y, 0 };
	Vector3 posFar = { reticlePosScreen.x, reticlePosScreen.y, 1 };

	//���[���h���W�n�ɕϊ�
	posNear = Matrix4::transformDivW(posNear, matVBVInverse);
	posFar = Matrix4::transformDivW(posFar, matVBVInverse);

	//���C�̕���
	Vector3 direction = posFar - posNear;
	direction.normalize();

	reticleObj->position = posNear + direction * distanceReticle3D;
	reticleObj->Update();

}

void Player::HealthUpdate()
{
	//�_���[�W�̃N�[���^�C�������炷
	if (damageInterval > 0) {
		damageInterval--;
	}


	//�{�^���ő̗͌��炵���葝�₵����
	if (ImGui::Button("damage!!")) {
		health--;
	}

	if (ImGui::Button("healing")) {
		health++;
	}

	ImGui::Text("interval %d", damageInterval);

	//�����ő�l��HP�ő�l�ŕ�������1HP������̉������v�Z
	float widthOnce = (float)healthWidthMax / (float)healthMax;
	//���݂�HP�Ɋ|���Z���ĉ���������o��
	float nowWidth = widthOnce * health;
	//HP�o�[�T�C�Y�擾
	Vector2 hp = healthSprite->GetSize();
	hp.x = nowWidth;
	healthSprite->SetSize(hp);

	float dmgSpriteAlpha = (float)damageInterval / (float)damageCooltime;
	Vector4 dmgColor = damageSprite->GetColor();
	dmgColor.w = dmgSpriteAlpha / 2.0f;
	damageSprite->SetColor(dmgColor);
}

void Player::Damage()
{
	//hp�����炷
	health--;

	//���H�炤�N�[���^�C����ݒ�
	damageInterval = damageCooltime;

	//HP��0�ɂȂ����玀�S������
	if (health == 0) {
		Death();
		return;
	}


}

void Player::Death()
{
	//�����蔻����Ƃ肽���Ȃ��̂ő����𖳓G�ɂ���
	collider->SetAttribute(COLLISION_ATTR_INVINCIBLE);
	//���V�[���Ɉڍs����܂ł̃J�E���g�_�E���J�n
	deathCount = deathCountMax;

	
}

void Player::UpdateDeath()
{
	if (deathCount == 0) {
		isAlive = false;
		return;
	}

	Object3d::Update();

	//���̃J�E���g�_�E�������炵�Ȃ���p�[�e�B�N�����o��
	deathCount--;
	//�p�[�e�B�N���̑��x
	for (int i = 0; i < 5; i++) {
	Vector3 vel = { Random(-1.0f,1.0f),Random(-1.0f,1.0f) ,Random(-1.0f,1.0f) };
	Vector3 acc = { 0.0f,Random(-0.1f,-1.0f),0.0f,};
	Vector3 pos = GetWorldPosition();
	pos += {Random(-3.0f, 3.0f), Random(-3.0f, 3.0f), Random(-3.0f, 3.0f)};

		hitParticle->Add((int)Random(10,20),pos,vel,acc,3.0f,0.0f);
	}
}

void Player::UpdateSpawn()
{
	eDataPlayerScale.Update();

	ImGui::Text("scale timerate %f", eDataPlayerScale.GetTimeRate());

	//�p�[�e�B�N������������
	if(spawnTimer > 0){
		float absPos = 30.0f;

		Vector3 pos = { Random(-absPos, absPos),Random(-absPos, absPos),Random(-absPos, absPos) };
		pos += GetWorldPosition();
		float startScale = Random(6.0f, 8.0f);

		hitParticle->AddLerp(15, pos, GetWorldPosition(), startScale, 0.0f,InterType::EaseOut);

		Vector3 vel = { 0,0,0 };
		Vector3 acc = { Random(-10.0f,10.0f),Random(-10.0f,10.0f) ,Random(-10.0f,10.0f) };

		spawnTimer--;
		eDataPlayerScale.Start(30);

	}//���ւ��o���Ȃ��炨�������Ȃ�
	else {
		Vector3 defaultScale = { 1,1,1 };
		scale = defaultScale * EaseOut(eDataPlayerScale.GetTimeRate());
		//���ւ����������Ȃ�Ȃ��甖��
		for (size_t i = 0; i < haloMax; i++) {

			haloObjects[i]->scale += {haloScaleVel[i], haloScaleVel[i], haloScaleVel[i]};
			haloObjects[i]->color.w -= haloAlphaVel[i];
			
		}

		//���@���傫���Ȃ肫������X�|�[�����������
		if (eDataPlayerScale.GetTimeRate() >= 1.0f) {
			isSpawn = false;
			//UI�n�̃��l�̃C�[�W���O�J�n
			easeUIAlpha.Start(60.0f);
		}

	}

	for (size_t i = 0; i < haloMax; i++) {
		haloObjects[i]->rotation += haloRotaVel[i];
		haloObjects[i]->Update();
	}
	

	//if (haloObject.color.w < 0.0f) {
	//	isSpawn = false;
	//}

}

