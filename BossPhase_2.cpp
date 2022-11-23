#include "BossPhase_2.h"
#include <math.h>
#include <time.h>
#include<stdlib.h>
#include<cmath>


BossPhase_2::~BossPhase_2()
{
	delete model_;
	delete torunedoModel_;
	delete beamModel_;
	delete medamaModel_;
	delete spriteHP;
	delete spriteHPBar;
}

void BossPhase_2::Initialize(SpriteManager* spriteManager)
{
	model_ = new Object3d();
	torunedoModel_ = new Object3d();
	beamModel_ = new Object3d();
	medamaModel_ = new Object3d();

	model_->Initialize("BossCube");
	torunedoModel_->Initialize("Torunedo");
	beamModel_->Initialize("beam");
	medamaModel_->Initialize("Medama");

	int texHP=Texture::LoadTexture(L"Resources/bossBarNaka.png");
	int texHPBar = Texture::LoadTexture(L"Resources/bossBar.png");

	spriteHP = new Sprite;
	spriteHP->Initialize(spriteManager,texHP/*, { 330.0f,610.0f }, { 1,1,1,1 }, { 0,0 }*/);
	spriteHP->SetColor({ 1,1,1,1 });
	spriteHP->SetAnchorPoint({ 0,0 });

	spriteHP->SetPos({ 330.0f,610.0f });

	spriteHP->SetSize({ 620,50 });

	spriteHPBar = new Sprite;
	spriteHPBar->Initialize(spriteManager,texHPBar/*, { 320,600 }, { 1,1,1,1 }, { 0,0 }*/);
	spriteHPBar->SetColor({ 1,1,1,1 });
	spriteHPBar->SetAnchorPoint({ 0,0 });

	spriteHPBar->SetPos({ 320.0f,600.0f });
	spriteHPBar->SetSize({ 640,50 });

	for (int i = 0; i < 19; i++) {

		worldTransform_[i].Initialize();

	}

	// �e
	worldTransform_[0].translation_ = { 100,20,100 };
	worldTransform_[0].scale_ = { kyubuScale,kyubuScale,kyubuScale };

	// �q�̍��W�ݒ�
	// �^�񒆂̒i
	worldTransform_[1].translation_ = { +kyubuLengh, 0,-kyubuLengh };
	worldTransform_[2].translation_ = { 0, 0,-kyubuLengh };
	worldTransform_[3].translation_ = { -kyubuLengh, 0,-kyubuLengh };
	worldTransform_[4].translation_ = { -kyubuLengh, 0, 0 };
	worldTransform_[5].translation_ = { -kyubuLengh, 0,+kyubuLengh };
	worldTransform_[6].translation_ = { 0, 0,+kyubuLengh };
	worldTransform_[7].translation_ = { +kyubuLengh, 0,+kyubuLengh };
	worldTransform_[8].translation_ = { +kyubuLengh, 0, 0 };

	// ��̒i
	worldTransform_[9].translation_ = { 0,+kyubuLengh, 0 };
	worldTransform_[10].translation_ = { 0,+kyubuLengh,-kyubuLengh };
	worldTransform_[11].translation_ = { -kyubuLengh,+kyubuLengh, 0 };
	worldTransform_[12].translation_ = { 0,+kyubuLengh,+kyubuLengh };
	worldTransform_[13].translation_ = { +kyubuLengh,+kyubuLengh, 0 };

	// ���̒i
	worldTransform_[14].translation_ = { 0,-kyubuLengh, 0 };
	worldTransform_[15].translation_ = { 0,-kyubuLengh,-kyubuLengh };
	worldTransform_[16].translation_ = { -kyubuLengh,-kyubuLengh, 0 };
	worldTransform_[17].translation_ = { 0,-kyubuLengh,+kyubuLengh };
	worldTransform_[18].translation_ = { +kyubuLengh,-kyubuLengh, 0 };

	// �q
	for (int i = 1; i < 19; i++) {
		worldTransform_[i].parent_ = &worldTransform_[0];
	}
	beamWorldTransform_.parent_= &worldTransform_[0];
	medamaWT.parent_= &worldTransform_[0];

	//�u�[�������̏�����
	for (int i = 0; i < 5; i++) {

		upBoomerangWorldTransform[i].Initialize();
		downBoomerangWorldTransform[i].Initialize();
	}
	torunedoTrans.Initialize();
	//��̒i
	upBoomerangWorldTransform[0].scale_ = { kyubuScale,kyubuScale,kyubuScale };
	torunedoTrans.scale_ = { 10,35,10 };
	upBoomerangWorldTransform[1].translation_ = { 0,0,-kyubuLengh };
	upBoomerangWorldTransform[2].translation_ = { -kyubuLengh,0, 0 };
	upBoomerangWorldTransform[3].translation_ = { 0,0,+kyubuLengh };
	upBoomerangWorldTransform[4].translation_ = { +kyubuLengh,0, 0 };
	//���̒i
	downBoomerangWorldTransform[0].scale_ = { kyubuScale,kyubuScale,kyubuScale };
	downBoomerangWorldTransform[1].translation_ = { 0,0,-kyubuLengh };
	downBoomerangWorldTransform[2].translation_ = { -kyubuLengh,0, 0 };
	downBoomerangWorldTransform[3].translation_ = { 0,0,+kyubuLengh };
	downBoomerangWorldTransform[4].translation_ = { +kyubuLengh,0, 0 };

	// �q
	for (int i = 1; i < 5; i++) {
		upBoomerangWorldTransform[i].parent_ = &upBoomerangWorldTransform[0];
		downBoomerangWorldTransform[i].parent_ = &downBoomerangWorldTransform[0];
	}

	beamWorldTransform_.Initialize();

	medamaWT.Initialize();

	medamaWT.scale_ = { 1.2f,1.2f,1.2f };
	medamaWT.translation_ = { 0.0f,0.0f,-3.2f };

	upBoomerangSound.SoundLoadWave("Resources/Sound/bossUp.wav");
	downBoomerangSound.SoundLoadWave("Resources/Sound/bossDown.wav");
	rushSound.SoundLoadWave("Resources/Sound/bossRush.wav");
	beamSound.SoundLoadWave("Resources/Sound/bossBeam.wav");
	damageSound.SoundLoadWave("Resources/Sound/bosssDamage.wav");
}

void BossPhase_2::Update(Vector3 playerPos)
{
	playerPos_ = playerPos;

	if (rushFlag == false && isUpActive == false && isDownActive == false && blowUpFlag == false) {
		angle += 0.9 * affine::Deg2Rad;
		worldTransform_[0].translation_.x = 100 * cos(angle);
		worldTransform_[0].translation_.z = 100 * sin(angle);
		if (angle > 6.28)
		{
			angle = 0;
		}
	}
	if (blowUpFlag == false)
	{
		std::srand(time(NULL));
		if (rushFlag == false && isUpActive == false && isDownActive == false && beamFlag == false && isAction == Action::AttackInterval)
		{
			intervalFrame++;
			if (intervalFrame >= maxIntervalFrame)
			{
				intervalFrame = 0;
				isAction = Action::AttackSelection;
			}
		}
		if (rushFlag == false && isUpActive == false && isDownActive == false && beamFlag == false && isAction == Action::AttackSelection)
		{
			do
			{
				beamFlag == false;
				isUpActive == false;
				isDownActive = false;
				rushFlag = false;
				randAttack = rand() % 4;
				//randAttack %= 100;
				if (randAttack == 0)
				{
					Attack = 1;
				}
				if (randAttack == 1)
				{
					Attack = 2;
				}
				if (randAttack == 2)
				{
					Attack = 3;
				}
				if (randAttack == 3)
				{
					Attack = 4;
				}
			} while (Attack == oldAttack);
			oldAttack = Attack;
			if (Attack == 1)
			{
				beamReset();
			}
			if (Attack == 2)
			{
				rushReset();
			}
			if (Attack == 3)
			{
				boomerangSet(playerPos, true);
			}
			if (Attack == 4)
			{
				boomerangSet(playerPos, false);
			}
			isAction = Action::AttackInMotion;
		}
	}


	if (beamOBJSetFlag == false && rushFlag == false && isUpActive == false && isDownActive == false && blowUpFlag == false) {
		TurnBodyToPlayer(playerPos);

	}

	if (blowUpFlag == false)
	{
		boomerangUpdate(playerPos);

		beamUpdate(playerPos);

		rushUpdate(playerPos);
	}

	DeathblowUp();

	TransferMat();

	spriteHP->SetSize({ float((620 / maxHP) * HP), 25 });

	/*debugText_->SetPos(10, 130);
	debugText_->Printf("Attack=%d,%d,%d", Attack,oldAttack,randAttack);*/
}

void BossPhase_2::TitleUpdate()
{
	worldTransform_[0].translation_.x = 100 * cos(angle);
	worldTransform_[0].translation_.z = 100 * sin(angle);
	TransferMat();
}

void BossPhase_2::Draw(ViewProjection viewprojection)
{
	if (isUpActive == true)
	{
		for (int i = 0; i < 5; i++) {
			model_->Draw(upBoomerangWorldTransform[i], viewprojection);
		}
		if (isUpAttack)
		{
			torunedoModel_->Draw(torunedoTrans, viewprojection);
		}
	}
	if (isDownActive == true)
	{
		for (int i = 0; i < 5; i++) {
			model_->Draw(downBoomerangWorldTransform[i], viewprojection);
		}
	}
	for (int i = 0; i < 19; i++) {
		if (AnnihilationFlag[i] == false)
		{
			model_->Draw(worldTransform_[i], viewprojection);
		}
	}

	// �r�[����ł��n�߂���`��
	if (beamSetFlag == true) {
		beamModel_->Draw(beamWorldTransform_, viewprojection);
	}

	medamaModel_->Draw(medamaWT, viewprojection);
}

void BossPhase_2::DrawUI()
{
	spriteHP->Draw();
	spriteHPBar->Draw();
}

void BossPhase_2::OnCollision()
{
	damageSound.SoundPlayWave(false, 0.5);
	HP--;
}

bool BossPhase_2::GetBoomerangflg(bool UpOrDown)
{
	if (UpOrDown)
	{
		return isUpAttack;
	}
	return isDownAttack;
}

void BossPhase_2::SetIsDead(bool isDead)
{
	blowUpFlag = isDead;
	beamFlag == false;
	isUpActive == false;
	isDownActive = false;
	rushFlag = false;
}

void BossPhase_2::Rset()
{
	// �e
	worldTransform_[0].translation_ = { 100,20,100 };
	worldTransform_[0].scale_ = { kyubuScale,kyubuScale,kyubuScale };

	

	// �q�̍��W�ݒ�
	// �^�񒆂̒i
	worldTransform_[1].translation_ = { +kyubuLengh, 0,-kyubuLengh };
	worldTransform_[2].translation_ = { 0, 0,-kyubuLengh };
	worldTransform_[3].translation_ = { -kyubuLengh, 0,-kyubuLengh };
	worldTransform_[4].translation_ = { -kyubuLengh, 0, 0 };
	worldTransform_[5].translation_ = { -kyubuLengh, 0,+kyubuLengh };
	worldTransform_[6].translation_ = { 0, 0,+kyubuLengh };
	worldTransform_[7].translation_ = { +kyubuLengh, 0,+kyubuLengh };
	worldTransform_[8].translation_ = { +kyubuLengh, 0, 0 };

	// ��̒i
	worldTransform_[9].translation_ = { 0,+kyubuLengh, 0 };
	worldTransform_[10].translation_ = { 0,+kyubuLengh,-kyubuLengh };
	worldTransform_[11].translation_ = { -kyubuLengh,+kyubuLengh, 0 };
	worldTransform_[12].translation_ = { 0,+kyubuLengh,+kyubuLengh };
	worldTransform_[13].translation_ = { +kyubuLengh,+kyubuLengh, 0 };

	// ���̒i
	worldTransform_[14].translation_ = { 0,-kyubuLengh, 0 };
	worldTransform_[15].translation_ = { 0,-kyubuLengh,-kyubuLengh };
	worldTransform_[16].translation_ = { -kyubuLengh,-kyubuLengh, 0 };
	worldTransform_[17].translation_ = { 0,-kyubuLengh,+kyubuLengh };
	worldTransform_[18].translation_ = { +kyubuLengh,-kyubuLengh, 0 };


	for (int i = 0; i < 19; i++)
	{
		worldTransform_[i].rotation_ = { 0,0,0 };
	}
	for (int i = 1; i < 19; i++) {
		worldTransform_[i].parent_ = &worldTransform_[0];
	}
	beamWorldTransform_.parent_ = &worldTransform_[0];
	medamaWT.parent_ = &worldTransform_[0];

	angle = 1.57;

	HP = maxHP;

	isAction = Action::AttackInterval;
	intervalFrame = 0;
	randAttack = 0;
	Attack = 0;
	oldAttack = 0;

	blowUpFlag = false;
	blowUpSetFlag = false;
	blowmatSetFlag = false;
	randomAngleX = 0;
	randomAngleY = 0;
	randomAngleZ = 0;
	blowStartAngle = (360 * 6) * affine::Deg2Rad;

	medamawaitTimer = 0;
	medamaRotaTimer = 0;
	medamaRotaEndTime = 5 * 60;
	startMedamaAngle = 0;
	endMedamaAngle = DegreeToRad(360.0f * 15.0f);
	medamaDownFlag = false;
	medamaGraviti = 0.1;

	AnnihilationFlag[0] = false;
	AnnihilationFlag[1] = false;
	AnnihilationFlag[2] = false;
	AnnihilationFlag[3] = false;
	AnnihilationFlag[4] = false;
	AnnihilationFlag[5] = false;
	AnnihilationFlag[6] = false;
	AnnihilationFlag[7] = false;
	AnnihilationFlag[8] = false;
	AnnihilationFlag[9] = false;
	AnnihilationFlag[10] = false;
	AnnihilationFlag[11] = false;
	AnnihilationFlag[12] = false;
	AnnihilationFlag[13] = false;
	AnnihilationFlag[14] = false;
	AnnihilationFlag[15] = false;
	AnnihilationFlag[16] = false;
	AnnihilationFlag[17] = false;
	AnnihilationFlag[18] = false;

	medamaWT.translation_ = { 0.0f,0.0f,-3.2f };
}

void BossPhase_2::beamUpdate(Vector3 playerPos)
{
	if (beamFlag == true) {
		if (beamSetFlag == false) {
			beamWorldTransform_.scale_.x = 0.5f;
			beamWorldTransform_.scale_.y = 0.5f;
			worldTransform_[1].translation_ = { -kyubuLengh, 0, 0 };

			worldTransform_[4].parent_ = &worldTransform_[2];
			worldTransform_[3].parent_ = &worldTransform_[2];
			worldTransform_[10].parent_ = &worldTransform_[2];
			worldTransform_[15].parent_ = &worldTransform_[2];

			worldTransform_[4].translation_ = { -kyubuLengh,0,0 };
			worldTransform_[3].translation_ = { +kyubuLengh,0,0 };
			worldTransform_[10].translation_ = { 0,+kyubuLengh,0 };
			worldTransform_[15].translation_ = { 0,-kyubuLengh,0 };
			beamSetFlag = true;
		}

		// ���̎���

		// �ǂꂭ�炢�̊ԓ������̔����̎���
		float timerRate1 = min((float)beamTimer / (float)maxTimer, 1);
		float timerRate2 = min((float)(beamTimer - maxTimer) / (float)maxTimer, 1);

		const static float startAngle = DegreeToRad(0);
		// ���ԓ��ɂǂꂭ�炢��]�������邩
		const static float endAngle = DegreeToRad(360 * 6);

		const float powNum = 3;

		beamTimer++;

		if (beamTimer <= maxTimer)
		{
			worldTransform_[2].rotation_.z = ((endAngle - startAngle) * powf(timerRate1, powNum) + startAngle) * 2;

		}
		else
		{
			worldTransform_[2].rotation_.z = ((endAngle - startAngle) * (powf(timerRate2 - 1, powNum) + 1) + startAngle) * 2;
		}

		if (beamTimer < maxStartTimer) {
			beamToPlayerVel = playerPos - worldTransform_[0].translation_;
		}

		// �ł��Ă鎞�̓���
		if (beamTimer >= maxStartTimer) {
			if (beamOBJSetFlag == false) {
				//beamWorldTransform_.translation_ = worldTransform_[0].translation_;
				oldPlayerPos = playerPos;
				// �ł����Ɍ����ăI�u�W�F�N�g����]������
				Vector3 velocity = playerPos - worldTransform_[0].translation_;
				velocity.normalize();
				beamToPlayerVel = velocity;
				beamToPlayerVel *= beamSpeed;

				beamSound.SoundPlayWave(true,0.5);

				beamOBJSetFlag = true;
			}
			TurnBeamToPlayer();
			beamtoPTimer++;
			if (beamtoPTimer == beamUpdatePosIntaval) {
				beamtoPTimer = 0;
				oldVelocity = playerPos - oldPlayerPos;
				oldVelocity.normalize();
				oldVelocity *= 1.0f;
			}
			if ((oldPlayerPos.x < playerPos.x - 0.5f || oldPlayerPos.x > playerPos.x + 0.5f) ||
				(oldPlayerPos.z < playerPos.z - 0.5f || oldPlayerPos.z > playerPos.z + 0.5f) ||
				(oldPlayerPos.y < playerPos.y - 0.5f || oldPlayerPos.y > playerPos.y + 0.5f)) {
				oldPlayerPos += oldVelocity;
			}

			// �r�[���̒����̌Œ�
			if (beamWorldTransform_.scale_.z < 80)
			{
				beamWorldTransform_.scale_.z += beamSpeed;
				beamWorldTransform_.matWorld_.m[3][0] += beamToPlayerVel.x;
				beamWorldTransform_.matWorld_.m[3][1] += beamToPlayerVel.y;
				beamWorldTransform_.matWorld_.m[3][2] += beamToPlayerVel.z;
			}
		}

		// �����I���̏���
		if (beamTimer >= maxEndTimer) {
			beamWorldTransform_.scale_.x = (float)easing_Out(0.5f, 0.0f, convergenceTimer, maxConvergenceT);
			beamWorldTransform_.scale_.y = (float)easing_Out(0.5f, 0.0f, convergenceTimer, maxConvergenceT);
			convergenceTimer++;
			if (convergenceTimer >= maxConvergenceT) {
				convergenceTimer = maxConvergenceT;
			}
			if (beamWorldTransform_.scale_.x <= 0.0f) {
				beamFlag = false;
				beamOBJSetFlag = false;
				isAction = Action::AttackInterval;
				worldTransform_[4].parent_ = &worldTransform_[0];
				worldTransform_[3].parent_ = &worldTransform_[0];
				worldTransform_[10].parent_ = &worldTransform_[0];
				worldTransform_[15].parent_ = &worldTransform_[0];

				worldTransform_[1].translation_ = { +2, 0,-2 };
				worldTransform_[4].translation_ = { -kyubuLengh, 0, 0 };
				worldTransform_[3].translation_ = { -2, 0,-2 };
				worldTransform_[10].translation_ = { 0,+2,-2 };
				worldTransform_[15].translation_ = { 0,-2,-2 };
				beamSound.StopWave();
			}
		}
	}

	/*debugText_->SetPos(20, 100);
	debugText_->Printf("beamX:%f", beamWorldTransform_.translation_.x);
	debugText_->SetPos(20, 120);
	debugText_->Printf("pos:%f", worldTransform_[0].matWorld_.m[3][0]);
	debugText_->SetPos(20, 140);
	debugText_->Printf("scale:%f", beamWorldTransform_.scale_.z);*/
}

void BossPhase_2::boomerangUpdate(Vector3 playerPos)
{
	if (isUpActive == true)
	{
		upBoomerangWorldTransform[0].rotation_.y += 60 * affine::Deg2Rad;
		torunedoTrans.rotation_ = upBoomerangWorldTransform[0].rotation_;
		if (isUpPreparation == true)
		{
			upBoomerangWorldTransform[0].translation_.y += 15.0f / 60;
			if (upBoomerangWorldTransform[0].translation_.y >= worldTransform_[0].translation_.y + kyubuLengh + 15)
			{
				isUpPreparation = false;
				isUpAttack = true;
				upBoomerangSound.SoundPlayWave(true,0.5);
			}
		}
		else if (isUpAttack == true)
		{
			upBoomerangWorldTransform[0].translation_.x += upVector.x * 2;
			upBoomerangWorldTransform[0].translation_.y += upVector.y * 2;
			upBoomerangWorldTransform[0].translation_.z += upVector.z * 2;

			torunedoTrans.translation_ = upBoomerangWorldTransform[0].translation_;
			torunedoTrans.translation_.y = 0;

			float AR;
			float BR;

			AR = pow((upBoomerangWorldTransform[0].translation_.x), 2) + pow((upBoomerangWorldTransform[0].translation_.z), 2);
			BR = pow((120 - upBoomerangWorldTransform[0].scale_.x * 1.5), 2);

			if (AR > BR)
			{
				isUpAttack = false;

				AnnihilationFlag[9] = false;
				AnnihilationFlag[10] = false;
				AnnihilationFlag[11] = false;
				AnnihilationFlag[12] = false;
				AnnihilationFlag[13] = false;
			}
		}
		else
		{
			worldTransform_[9].translation_.y += kyubuLengh / 20;
			worldTransform_[10].translation_.y += kyubuLengh / 20;
			worldTransform_[11].translation_.y += kyubuLengh / 20;
			worldTransform_[12].translation_.y += kyubuLengh / 20;
			worldTransform_[13].translation_.y += kyubuLengh / 20;
			if (worldTransform_[9].translation_.y >= kyubuLengh)
			{
				isUpActive = false;
				upBoomerangWorldTransform[0].rotation_.y = 0;
				
				upBoomerangSound.StopWave();

				isAction = Action::AttackInterval;
			}
		}
		for (int i = 0; i < 5; i++) {
			affine::makeAffine(upBoomerangWorldTransform[i]);
			affine::makeAffine(torunedoTrans);
			if (i != 0)
			{
				upBoomerangWorldTransform[i].matWorld_ *= upBoomerangWorldTransform[i].parent_->matWorld_;
			}
			upBoomerangWorldTransform[i].TransferMatrix();
			torunedoTrans.TransferMatrix();
		}
	}
	if (isDownActive == true)
	{
		downBoomerangWorldTransform[0].rotation_.y += 60 * affine::Deg2Rad;
		if (isDownPreparation == true)
		{
			downBoomerangWorldTransform[0].translation_.y -= (worldTransform_[0].translation_.y - kyubuLengh) / 60;
			if (downBoomerangWorldTransform[0].translation_.y <= 2)
			{
				isDownPreparation = false;
				isDownAttack = true;
				downBoomerangSound.SoundPlayWave(true,0.5);
			}
		}
		else if (isDownAttack == true)
		{
			if (downBoomerangWorldTransform[0].rotation_.y <= 12000.0f * affine::Deg2Rad)
			{
				downVector = playerPos - downBoomerangWorldTransform[0].translation_;
				downVector.normalize();
			}
			else
			{
				downVector = { 1,0,1 };
			}

			downBoomerangWorldTransform[0].translation_.x += downVector.x;
			downBoomerangWorldTransform[0].translation_.z += downVector.z;

			float AR;
			float BR;

			AR = pow((downBoomerangWorldTransform[0].translation_.x), 2) + pow((downBoomerangWorldTransform[0].translation_.z), 2);
			BR = pow((120 - downBoomerangWorldTransform[0].scale_.x * 1.5), 2);

			if (AR > BR && downBoomerangWorldTransform[0].rotation_.y >= 12000.0f * affine::Deg2Rad)
			{
				isDownAttack = false;

				AnnihilationFlag[14] = false;
				AnnihilationFlag[15] = false;
				AnnihilationFlag[16] = false;
				AnnihilationFlag[17] = false;
				AnnihilationFlag[18] = false;
			}
		}
		else
		{
			worldTransform_[14].translation_.y -= kyubuLengh / 20;
			worldTransform_[15].translation_.y -= kyubuLengh / 20;
			worldTransform_[16].translation_.y -= kyubuLengh / 20;
			worldTransform_[17].translation_.y -= kyubuLengh / 20;
			worldTransform_[18].translation_.y -= kyubuLengh / 20;
			if (worldTransform_[14].translation_.y <= -kyubuLengh)
			{
				isDownActive = false;
				isAction = Action::AttackInterval;
				downBoomerangWorldTransform[0].rotation_.y = 0;

				downBoomerangSound.StopWave();
			}
		}
		for (int i = 0; i < 5; i++) {
			affine::makeAffine(downBoomerangWorldTransform[i]);
			if (i != 0)
			{
				downBoomerangWorldTransform[i].matWorld_ *= downBoomerangWorldTransform[i].parent_->matWorld_;
			}
			downBoomerangWorldTransform[i].TransferMatrix();
		}
	}
	/*debugText_->SetPos(10, 10);
	debugText_->Printf("Active:%d", isDownActive);
	debugText_->SetPos(10, 30);
	debugText_->Printf("Attack:%d", isDownAttack);
	debugText_->SetPos(10, 50);
	debugText_->Printf("translation_:%f,%f,%f", downBoomerangWorldTransform[0].translation_.x, downBoomerangWorldTransform[0].translation_.y, upBoomerangWorldTransform[0].translation_.z);*/
}

void BossPhase_2::boomerangSet(Vector3 playerPos, bool UpOrDown)
{
	if (UpOrDown == true)
	{
		upBoomerangWorldTransform[0].translation_ = worldTransform_[0].translation_;
		upBoomerangWorldTransform[0].translation_.y = worldTransform_[0].translation_.y + kyubuLengh;
		AnnihilationFlag[9] = true;
		AnnihilationFlag[10] = true;
		AnnihilationFlag[11] = true;
		AnnihilationFlag[12] = true;
		AnnihilationFlag[13] = true;

		worldTransform_[9].translation_.y = 0;
		worldTransform_[10].translation_.y = 0;
		worldTransform_[11].translation_.y = 0;
		worldTransform_[12].translation_.y = 0;
		worldTransform_[13].translation_.y = 0;

		upVector = playerPos - worldTransform_[0].translation_;
		upVector.y = 0;
		upVector.normalize();

		isUpActive = true;
		isUpPreparation = true;

		worldTransform_[0].rotation_.x = 0;
	}
	if (UpOrDown == false)
	{
		downBoomerangWorldTransform[0].translation_ = worldTransform_[0].translation_;
		downBoomerangWorldTransform[0].translation_.y = worldTransform_[0].translation_.y - kyubuLengh;
		AnnihilationFlag[14] = true;
		AnnihilationFlag[15] = true;
		AnnihilationFlag[16] = true;
		AnnihilationFlag[17] = true;
		AnnihilationFlag[18] = true;

		worldTransform_[14].translation_.y = 0;
		worldTransform_[15].translation_.y = 0;
		worldTransform_[16].translation_.y = 0;
		worldTransform_[17].translation_.y = 0;
		worldTransform_[18].translation_.y = 0;

		/*upVector = playerPos - worldTransform_[0].translation_;
		upVector.y = 0;
		upVector.normalize();*/
		isDownActive = true;
		isDownPreparation = true;

		worldTransform_[0].rotation_.x = 0;
	}

}

void BossPhase_2::rushReset()
{
	rushFlag = true;
	rushStartSetFlag = false;
	wheelTimer = 0;
	wheelSpeedX = 0.0f;
	RtoPFlag = false;
	rushFinsh = false;
	rushFinshSet = false;
	wheelTimer2 = 0;
}

void BossPhase_2::DeathblowUp()
{
	if (blowUpFlag == true) {
		
		if (blowUpSetFlag == false) {
			for (int i = 0; i < 19; i++) {

				if (i != 0) {
					randomAngleX = (float)((rand() % 360 - 180) * affine::Deg2Rad);
					randomAngleY = (float)((rand() % 360 - 180) * affine::Deg2Rad);
					randomAngleZ = (float)((rand() % 360 - 180) * affine::Deg2Rad);
					blowUpVel[i] = { randomAngleX,randomAngleY,randomAngleZ };
					blowUpVel[i].normalize();
					blowUpVel[i] *= 0.8f;
					// �e�q�֌W���тȂ���
					worldTransform_[i].parent_ = &worldTransform_[0];
				}
				else {
					blowUpVel[i] = { 0.0f,90.0f * affine::Deg2Rad,0.0f };
					blowUpVel[i].normalize();
					blowUpVel[i] *= 0.6f;
				}
				
			}
			beamWorldTransform_.parent_ = &worldTransform_[0];
			medamaWT.parent_ = &worldTransform_[0];

			AnnihilationFlag[0] = true;
			blowUpSetFlag = true;
		}
		if (worldTransform_[0].rotation_.x < 0.0f) {
			worldTransform_[0].rotation_.x += 0.01f;
		}
		else if (worldTransform_[0].rotation_.y < blowStartAngle) {
			worldTransform_[0].rotation_.y += 0.5f;

		}
		else {
			// ��юU�鏈��
			for (int i = 1; i < 19; i++) {
				worldTransform_[i].translation_ += blowUpVel[i];
				worldTransform_[i].rotation_ += blowUpRotaVel;
			}

			// �ڋʒP�̂̉�]����
			medamaRotaTimer++;
			if (medamaRotaTimer >= medamaRotaEndTime) {
				medamaRotaTimer = medamaRotaEndTime;
				medamaDownFlag = true;
			}
			medamaWT.rotation_.y = easing_Out(startMedamaAngle, endMedamaAngle, medamaRotaTimer, medamaRotaEndTime);

			// ��������
			if (medamaDownFlag == true) {
				medamawaitTimer++;
				if (medamawaitTimer >= 2.5f * 60) {
					medamaGraviti += 0.03;
					medamaWT.translation_.y -= medamaGraviti;
				}
			}


		}

	}
}

void BossPhase_2::rushUpdate(Vector3 playerPos)
{
	if (rushFlag == true) {
		// �{�X��������O�̃|�W���L�^
		if (rushStartSetFlag == false) {
			// �����̎ԗ�
			worldTransform_[13].parent_ = &worldTransform_[4];
			worldTransform_[5].parent_ = &worldTransform_[4];
			worldTransform_[3].parent_ = &worldTransform_[4];
			worldTransform_[18].parent_ = &worldTransform_[4];

			worldTransform_[13].translation_ = { 0,+kyubuLengh,0 };
			worldTransform_[5].translation_ = { 0,0,+kyubuLengh };
			worldTransform_[3].translation_ = { 0,0,-kyubuLengh };
			worldTransform_[18].translation_ = { 0,-kyubuLengh,0 };
			// �E���̎ԗ�
			worldTransform_[11].parent_ = &worldTransform_[8];
			worldTransform_[7].parent_ = &worldTransform_[8];
			worldTransform_[1].parent_ = &worldTransform_[8];
			worldTransform_[16].parent_ = &worldTransform_[8];


			worldTransform_[11].translation_ = { 0,+kyubuLengh,0 };
			worldTransform_[7].translation_ = { 0,0,+kyubuLengh };
			worldTransform_[1].translation_ = { 0,0,-kyubuLengh };
			worldTransform_[16].translation_ = { 0,-kyubuLengh,0 };
			// ���̍�����ۑ�
			originPosY = worldTransform_[0].translation_.y;
			rushStartSetFlag = true;
		}

		// �{�X�̈ʒu���_�E���|�W�̂Ƃ���܂ŉ�����
		if (rushFinsh == false) {
			if (worldTransform_[0].translation_.y >= downPosY) {
				worldTransform_[0].translation_.y -= 0.1f;
				if (worldTransform_[0].rotation_.x <= 0.0f) {
					worldTransform_[0].rotation_.x += 0.005f;
				}
			}

		}
		if (worldTransform_[0].translation_.y < downPosY)
		{
			wheelTimer++;

			if (wheelSpeedX < wheelDepartureTime)
			{
				TurnRushToPlayer(playerPos);
			}
			if (wheelTimer >= wheelEndTime) {
				wheelTimer = wheelEndTime;
			}
			wheelSpeedX = easing_In(wheelStart, wheelEnd, wheelTimer, wheelEndTime);
			worldTransform_[4].rotation_.x -= wheelSpeedX;
			worldTransform_[8].rotation_.x -= wheelSpeedX;
		}
		// �ԗւ̃X�s�[�h���ːi�������X�s�[�h�ɍs������
		if (wheelSpeedX >= wheelDepartureTime)
		{
			if (RtoPFlag == false) {
				rushToPlayer = playerPos - worldTransform_[0].translation_;
				rushToPlayer.normalize();
				rushToPlayer.y = 0.0f;
				rushToPlayer *= 2.5f;
				RtoPFlag = true;
				rushSound.SoundPlayWave(false,0.5);
			}
			if (rushFinsh == false) {
				worldTransform_[0].translation_ += rushToPlayer;
			}

			float AR;
			float BR;

			AR = pow((worldTransform_[0].translation_.x), 2) + pow((worldTransform_[0].translation_.z), 2);
			BR = pow(100, 2);

			if (AR > BR)
			{
				rushFinsh = true;
			}
		}

		//�ːi�I���t���O����������
		if (rushFinsh == true) {
			if (rushFinshSet == false) {
				wheelStart2 = worldTransform_[0].rotation_.x;
			}
			wheelTimer2++;

			worldTransform_[4].rotation_.x = easing_In(wheelStart2, wheelEnd2, wheelTimer2, wheelEndTime2);
			worldTransform_[8].rotation_.x = easing_In(wheelStart2, wheelEnd2, wheelTimer2, wheelEndTime2);
			if (worldTransform_[0].translation_.y < originPosY) {
				worldTransform_[0].translation_.y += 0.05f;
			}
			else if (worldTransform_[0].translation_.y >= originPosY) {
				worldTransform_[0].translation_.y = originPosY;

				worldTransform_[13].parent_ = &worldTransform_[0];
				worldTransform_[5].parent_ = &worldTransform_[0];
				worldTransform_[3].parent_ = &worldTransform_[0];
				worldTransform_[18].parent_ = &worldTransform_[0];
				worldTransform_[11].parent_ = &worldTransform_[0];
				worldTransform_[7].parent_ = &worldTransform_[0];
				worldTransform_[1].parent_ = &worldTransform_[0];
				worldTransform_[16].parent_ = &worldTransform_[0];

				worldTransform_[1].translation_ = { +kyubuLengh, 0,-kyubuLengh };
				worldTransform_[3].translation_ = { -kyubuLengh, 0,-kyubuLengh };
				worldTransform_[5].translation_ = { -kyubuLengh, 0,+kyubuLengh };
				worldTransform_[7].translation_ = { +kyubuLengh, 0,+kyubuLengh };
				worldTransform_[11].translation_ = { -kyubuLengh,+kyubuLengh, 0 };
				worldTransform_[13].translation_ = { +kyubuLengh,+kyubuLengh, 0 };
				worldTransform_[16].translation_ = { -kyubuLengh,-kyubuLengh, 0 };
				worldTransform_[18].translation_ = { +kyubuLengh,-kyubuLengh, 0 };

				float vectorX = worldTransform_->translation_.x;
				float vectorZ = worldTransform_->translation_.z;
				angle = atan2(vectorX, vectorZ);
				angle = atan2(vectorX, vectorZ);
				angle -= 1.57;
				if (angle > 0)
				{
					angle -= 6.28;
				}
				angle = abs(angle);

				rushFlag = false;
				isAction = Action::AttackInterval;
			}
		}
		/*debugText_->SetPos(20, 140);
		debugText_->Printf("rotaX:%f", worldTransform_[0].rotation_.x);
		debugText_->SetPos(20, 160);
		debugText_->Printf("speedX:%f", wheelSpeedX);*/
	}
}

void BossPhase_2::TransferMat()
{

	for (int i = 0; i < 19; i++) {

		affine::makeAffine(worldTransform_[i]);
		affine::makeAffine(beamWorldTransform_);

		// �r�[���ł��n�߂���s��̊|���Z�𒲐�
		if (beamFlag == true) {
			// �r�[���Ŏg�����̂��s��v�Z����
			if (i == 10 || i == 3 || i == 4 || i == 15) {
				//worldTransform_[2].matWorld_ *= worldTransform_[0].matWorld_;
				worldTransform_[i].matWorld_ *= worldTransform_[2].matWorld_;

			}
			else if (i != 0) {
				worldTransform_[i].matWorld_ *= worldTransform_[0].matWorld_;

			}
			// �r�[���I�u�W�F�̍s��̓]��

			beamWorldTransform_.matWorld_ *= worldTransform_[0].matWorld_;
		}
		// �ːi�t���O�������Ă�����
		else if (rushFlag == true) {
			// �ԗւɕK�v�Ȃ��̂��s��v�Z����
			if (i == 3 || i == 5 || i == 13 || i == 18) {
				// �����̎ԗ�
				worldTransform_[i].matWorld_ *= worldTransform_[4].matWorld_;
			}
			else if (i == 1 || i == 7 || i == 11 || i == 16) {
				// �E���̎ԗ�
				worldTransform_[i].matWorld_ *= worldTransform_[8].matWorld_;
			}
			else if (i != 0) {
				worldTransform_[i].matWorld_ *= worldTransform_[0].matWorld_;

			}

		}
		// ����ȊO�̎��͊�{�I�ɐe�ƍs��v�Z���s��
		else if (i != 0) {
			worldTransform_[i].matWorld_ *= worldTransform_[0].matWorld_;

		}

		worldTransform_[i].TransferMatrix();
	}

	if (blowUpFlag == false) {
		affine::makeAffine(medamaWT);
		medamaWT.matWorld_ *= worldTransform_[0].matWorld_;
		medamaWT.TransferMatrix();
	}
	else if (blowUpFlag == true) {
		if (worldTransform_[0].rotation_.y < blowStartAngle) {
			affine::makeAffine(medamaWT);
			medamaWT.matWorld_ *= worldTransform_[0].matWorld_;
			medamaWT.TransferMatrix();

		}
		else {
			if (blowmatSetFlag == false) {
				Kari = worldTransform_[0];
				TurnDeadToPlayer(playerPos_);
				Kari.rotation_.x = 0.0f;
				affine::makeAffine(Kari);
				medamaWT.parent_ = &Kari;
				blowmatSetFlag = true;
			}
			affine::makeAffine(medamaWT);
			medamaWT.matWorld_ *= Kari.matWorld_;
			medamaWT.TransferMatrix();
		}
	}
	/*debugText_->SetPos(20, 200);
	debugText_->Printf("medama:%f,%f,%f", medamaWT.rotation_.x, medamaWT.rotation_.y, medamaWT.rotation_.z);*/


	beamWorldTransform_.TransferMatrix();

}

float BossPhase_2::Lerp(const float& startPos, const float& endPos, const float& timeRate)
{
	float dis = endPos - startPos;
	return dis * timeRate + startPos;
}

double BossPhase_2::easing_Out(double start, double end, double time, double max_time)
{
	time /= max_time;
	double move = end - start;
	return start + (move * (1 - (1 - time) * (1 - time)));
}

double BossPhase_2::easing_In(double start, double end, double time, double max_time)
{
	time /= max_time;
	double move = end - start;
	return start + (move * time * time);
}

void BossPhase_2::beamReset()
{
	beamFlag = true;
	beamSetFlag = false;
	beamOBJSetFlag = false;

	beamWorldTransform_.translation_ = { 0.0f,0.0f,0.0f };
	beamWorldTransform_.scale_ = { 1.0f,1.0f,1.0f };
	beamWorldTransform_.rotation_ = { 0.0f,0.0f,0.0f };

	worldTransform_[2].rotation_ = { 0.0f,0.0f,0.0f };
	worldTransform_[1].translation_ = { +kyubuLengh, 0,-kyubuLengh };
	worldTransform_[3].translation_ = { -kyubuLengh, 0,-kyubuLengh };
	worldTransform_[10].translation_ = { 0,+kyubuLengh,-kyubuLengh };
	worldTransform_[15].translation_ = { 0,-kyubuLengh,-kyubuLengh };

	beamTimer = 0;
	beamtoPTimer = 0;
	convergenceTimer = 0;
}

void BossPhase_2::TurnBodyToPlayer(Vector3 playerPos)
{
	// �ł����Ɍ����ăI�u�W�F�N�g����]������
	Vector3 velocity = playerPos - worldTransform_[0].translation_;
	velocity.normalize();

	// Y������p�x(��y)
	worldTransform_[0].rotation_.y = -std::atan2(velocity.z, velocity.x) - DegreeToRad(90);

	// Y�������-��y�񂷉�]�s����v�Z
	Matrix4 RotY;
	affine::makeMatRotY(RotY, -worldTransform_[0].rotation_.y);

	// velosity_�ɉ�]�s����|���Z����velosityZ�����߂�
	Vector3 velocityZ = velocity;
	velocityZ = affine::MatVector(RotY, velocityZ);

	// X������p�x(��x)
	worldTransform_[0].rotation_.x = std::atan2(velocityZ.y, -velocityZ.z);
}

void BossPhase_2::TurnDeadToPlayer(Vector3 playerPos)
{
	// �ł����Ɍ����ăI�u�W�F�N�g����]������
	Vector3 velocity = playerPos - Kari.translation_;
	velocity.normalize();

	// Y������p�x(��y)
	Kari.rotation_.y = -std::atan2(velocity.z, velocity.x) - DegreeToRad(90);

	// Y�������-��y�񂷉�]�s����v�Z
	Matrix4 RotY;
	affine::makeMatRotY(RotY, -Kari.rotation_.y);

	// velosity_�ɉ�]�s����|���Z����velosityZ�����߂�
	Vector3 velocityZ = velocity;
	velocityZ = affine::MatVector(RotY, velocityZ);

	// X������p�x(��x)
	//Kari.rotation_.x = std::atan2(velocityZ.y, -velocityZ.z);
}

void BossPhase_2::TurnBeamToPlayer()
{
	// �ł����Ɍ����ăI�u�W�F�N�g����]������
	Vector3 velocity = oldPlayerPos - worldTransform_[0].translation_;
	velocity.normalize();

	// Y������p�x(��y)
	worldTransform_[0].rotation_.y = -std::atan2(velocity.z, velocity.x) - DegreeToRad(90);

	// Y�������-��y�񂷉�]�s����v�Z
	Matrix4 RotY;
	affine::makeMatRotY(RotY, -worldTransform_[0].rotation_.y);

	// velosity_�ɉ�]�s����|���Z����velosityZ�����߂�
	Vector3 velocityZ = velocity;
	velocityZ = affine::MatVector(RotY, velocityZ);

	// X������p�x(��x)
	worldTransform_[0].rotation_.x = std::atan2(velocityZ.y, -velocityZ.z);

}

void BossPhase_2::TurnRushToPlayer(Vector3 playerPos)
{
	// �ł����Ɍ����ăI�u�W�F�N�g����]������
	Vector3 velocity = playerPos - worldTransform_[0].translation_;
	velocity.normalize();

	// Y������p�x(��y)
	worldTransform_[0].rotation_.y = -std::atan2(velocity.z, velocity.x) - DegreeToRad(90);
}
