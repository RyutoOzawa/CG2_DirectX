//プレイヤークラス
//ユーザーが操作するプレイヤー全般の処理はここで行う

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

	//コライダーの追加
	float radius = 1.0f;
	//半径分だけ浮いた座標を球の中心にする
	SetCollider(new SphereCollider(Vector3(0, 0, 0),radius));
	collider->SetAttribute(COLLISION_ATTR_ALLIES);


	//命中パーティクル
	hitParticle = std::make_unique<ParticleManager>();
	hitParticle->Initialize(Texture::LoadTexture("particle.png"));

	//レティクルオブジェクト
	reticleObj = std::make_unique<Object3d>();
	reticleObj->Initialize();
	//モデルはオブジェクトと同じもの
	reticleObj->SetModel(Object3d::model);

	//レティクルスプライト
	reticleSprite = std::make_unique<Sprite>();
	reticleSprite->Initialize(reticleTexture);
	reticleSprite->SetAnchorPoint({ 0.5f,0.5f });
	reticleSprite->SetColor({ 1,1,1,0.5f });

	//HPの初期化
	health = healthMax;
	healthWidthMax = 720;

	//HPスプライト
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

	//光輪モデル生成
	haloModel = std::make_unique<Model>();
	haloModel = Model::CreateModel(MODEL_PLANE);
	haloModel->SetTexture(Texture::LoadTexture("halo.png"));

	//弾モデル
	bulletModel = std::make_unique<Model>();
	bulletModel = Model::CreateModel("PlayerBullet");

	for (std::unique_ptr<Object3d>& haloObject : haloObjects) {
		haloObject = std::make_unique<Object3d>();
		haloObject->Initialize();
		haloObject->SetModel(haloModel.get());
	}

	//スポーン処理
	Spawn();
}

void Player::Spawn()
{
	for (size_t i = 0; i < haloMax; i++) {
		float sc = Random(1.0f, 3.0f);
		haloObjects[i]->scale = { sc ,sc ,sc };

		haloObjects[i]->color.z = Random(0.8f, 1.0f);

		//大きさの速度をランダムに
		haloScaleVel[i] = Random(0.5f, 2.5f);

		//色の透過速度もランダムに
		haloAlphaVel[i] = Random(0.01f, 0.05f);
//		haloAlphaVel[i] = 0.01f;

		haloRotaVel[i] = { Random(0,PI / 16.0f),Random(0,PI / 16.0f) ,Random(0,PI / 16.0f) };

		//角度もランダムに
		haloObjects[i]->rotation = { Random(-PI,PI),Random(-PI,PI) ,Random(-PI,PI) };
		haloObjects[i]->parent = this;
		haloObjects[i]->color.w = 1.0f;
		haloObjects[i]->Update();

	}

	//タイマ―セット
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

	//UIのアルファ値を更新
	easeUIAlpha.Update();
	UIAlpha = Lerp(0.0f, 1.0f,  InBounce(easeUIAlpha.GetTimeRate()));


	if (isSpawn) {
		UpdateSpawn();
		UIAlpha = 0.0f;
	}

	//アルファ値をUIに適用
	Vector4 uiColor;
	uiColor = healthSprite->GetColor();
	uiColor.w = UIAlpha;
	healthSprite->SetColor(uiColor);
	uiColor = reticleSprite->GetColor();
	uiColor.w = UIAlpha;
	reticleSprite->SetColor(uiColor);



	//死んでる弾を消す
	bullets.remove_if([](std::unique_ptr<PlayerBullet>& bullet) {
		if (!bullet->IsAlive()) {
			return true;
		}
		return false;
		});

	//自機のHPが0、スポーンアニメ中なら操作させない
	if (health != 0  && !isSpawn) {

		//移動
		Move();

		//攻撃
		Attack();

		//レティクルのオブジェクトデータ更新
		ReticleUpdate(enemys);
	}
	//0なら死ぬアニメーション
	else if (health == 0) {
		UpdateDeath();
	}

	//弾の更新
	for (std::unique_ptr<PlayerBullet>& bullet : bullets) {
		bullet->Update();
	}

	ImGui::Text("bullet %d", bullets.size());

	//obj3dの更新
	Object3d::Update();
	//命中時パーティクル更新
	hitParticle->Update();
	//HPバーの更新
	HealthUpdate();

	ImGui::End();

}

void Player::Draw()
{
	//自分の描画

	//ダメージのクールタイムによって点滅
	//生存によっての描画フラグ
	bool isPlayerDraw = (health != 0 || deathCount > 0);

	if (damageInterval % 4 < 1 && isPlayerDraw) {
		if (spawnTimer <= 0) {
			Object3d::Draw();
		}
	}

	//弾の描画
	for (std::unique_ptr<PlayerBullet>& bullet : bullets) {
		bullet->Draw();
	}

	//スポーン時の光輪
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

	//ダメージのクールタイムが終わってないなら何もしない
	if (damageInterval > 0) {
		return;
	}

	//パーティクルの速度
	for (int i = 0; i < 25; i++) {
		Vector3 vel = { 0,0,0 };
		float absAcc = 0.5f;
		Vector3 acc = { Random(-absAcc,absAcc),Random(-absAcc,absAcc) ,Random(-absAcc,absAcc) };


		//パーティクル追加
		hitParticle->Add(15, GetWorldPosition(), vel, acc, 6.0f, 0.0f);
	}

	//ダメージを受ける
	Damage();

}


void Player::Move()
{
	//入力
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

	//座標に加算する速度
	Vector3 spd{};
	float baseSpd = 0.5f;


	spd.x += baseSpd * inputHorizontal;
	spd.y += baseSpd * inputVertical;
	



	//現在座標を取得
	localPos = position;
	//加算して代入
	localPos += spd;
	position = localPos;

	const float bodyTurnBase = (float)PI / 180.0f;

	//旋回の挙動
	rotation.z -= bodyTurnBase * inputHorizontal;
	rotation.x -= bodyTurnBase * inputVertical;

	//入力がなければ
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


	//傾きの限界を超えないように
	if (fabs(rotation.x) > (float)PI /180.0f *15.0f) {
		rotation.x = -inputVertical * (float)PI / 180.0f * 15.0f;
	}

	if (fabs(rotation.z) > (float)PI / 180.0f * 15.0f) {
		rotation.z = -inputHorizontal * (float)PI / 180.0f * 15.0f;
	}


}

void Player::Attack()
{
	//スペースキーで弾発射
	if (shotInterval == 0) {

		if (Input::GetInstance()->IsKeyPress(DIK_SPACE) || Input::GetInstance()->IsPadPress(XINPUT_GAMEPAD_RIGHT_SHOULDER)) {
			
			shotInterval = shotCooltime;

			//弾の速度
			const float bulletSpdBase = 32.0f;
			Vector3 velocity(0, 0, bulletSpdBase);

			velocity = reticleObj->GetWorldPosition() - Object3d::GetWorldPosition();
			velocity.normalize();
			velocity *= bulletSpdBase;

			//弾の生成と初期化
			std::unique_ptr< PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
			newBullet->Initialize(bulletModel.get(), GetWorldPosition(), velocity);

			//弾の登録
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

	//ビューポート行列
	Matrix4 matViewPort;
	matViewPort.identity();
	matViewPort.m[0][0] = WindowsAPI::winW / 2.0f;
	matViewPort.m[1][1] = -(WindowsAPI::winH / 2.0f);
	matViewPort.m[3][0] = WindowsAPI::winW / 2.0f;
	matViewPort.m[3][1] = WindowsAPI::winH / 2.0f;

	//カメラ行列との合成
	Matrix4 matViewProViewPort = Object3d::camera->GetViewProjection() * matViewPort;


	//画面上のレティクル座標を動かす
	Vector2 reticleSpd = { 0,0 };
	float reticleSpdBase = 8.0f;

	//入力
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


	
	//入力値×レティクルスピードで移動
	reticleSpd.x = inputHorizontal * reticleSpdBase;
	reticleSpd.y = inputVertical * reticleSpdBase;
	reticlePosScreen += reticleSpd;

	//レティクル座標の移動制限
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

	//座標をスプライトにセット
	reticleSprite->SetPos(reticlePosScreen);

	//レティクルの色を初期化
	reticleColor = reticleSprite->GetColor();
	reticleColor.z = 1.0f;

	

	//カメラからレティクル(3D)への距離
	float distanceReticle3D = distanceCamera + 100.0f;

	//スクリーン座標変換した自機の座標
	Vector3 posPlayerScreen = Matrix4::transformDivW(GetWorldPosition(), matViewProViewPort);

	//ImGui::Text("screen Z player %f", posPlayerScreen.z);

	//敵の座標をスクリーン座標に変換
	std::list<std::unique_ptr<Enemy>>::iterator itE;
	itE = enemys->begin();
	for (; itE != enemys->end(); itE++) {
		//敵のワールド座標を取得
		Vector3 posEnemyWorld = itE->get()->GetWorldPosition();

		//ビュー、プロジェクション、ビューポート行列を掛けてW除算
		posEnemyWorld = Matrix4::transformDivW(posEnemyWorld, matViewProViewPort);
		Vector2 posEnemyScreen = { posEnemyWorld.x,posEnemyWorld.y };
		
		//ImGui::Text("screen Z eyemy %f", posEnemyWorld.z)
		Circle reticleC{ reticlePosScreen,reticleRadius };
		Circle enemyC{ posEnemyScreen,1.0f };

		//カメラからレティクルの距離を(敵のワールド×ビュー)のzにする
		Matrix4 matEnemyView = itE->get()->GetMatWorld() * camera->GetViewProjection();

		ImGui::Text("posenemyView %f,%f,%f", matEnemyView.m[3][0], matEnemyView.m[3][1], matEnemyView.m[3][2]);

		//ImGui::Text("enemyView.z %f", posEnemyView.z);



		//レティクルが敵に当たっているなら
		if (Collision::ColCircleToCircle(reticleC, enemyC)) {
			
			//カメラから敵の距離が自機との距離より小さいなら狙わない
			if (matEnemyView.m[3][2] < distanceCamera) {
				continue;
			}

			//distanceReticle3D = matEnemyView.z;
			//レティクルが動いているならロックオン
			if (inputHorizontal != 0 || inputVertical != 0) {
				reticlePosScreen = posEnemyScreen;
				reticleColor.z = 0.0f;
			}
		}
	}

	//レティクルの色を設定
	reticleSprite->SetColor(reticleColor);

//	ImGui::Text("distance retilce3d %f", distanceReticle3D);

	//ビュー、射影、ビューポートの行列を合成
	Matrix4 matVBVInverse = matViewProViewPort;
	matVBVInverse.Inverse();

	//スクリーン座標
	Vector3 posNear = { reticlePosScreen.x, reticlePosScreen.y, 0 };
	Vector3 posFar = { reticlePosScreen.x, reticlePosScreen.y, 1 };

	//ワールド座標系に変換
	posNear = Matrix4::transformDivW(posNear, matVBVInverse);
	posFar = Matrix4::transformDivW(posFar, matVBVInverse);

	//レイの方向
	Vector3 direction = posFar - posNear;
	direction.normalize();

	reticleObj->position = posNear + direction * distanceReticle3D;
	reticleObj->Update();

}

void Player::HealthUpdate()
{
	//ダメージのクールタイムを減らす
	if (damageInterval > 0) {
		damageInterval--;
	}


	//ボタンで体力減らしたり増やしたり
	if (ImGui::Button("damage!!")) {
		health--;
	}

	if (ImGui::Button("healing")) {
		health++;
	}

	ImGui::Text("interval %d", damageInterval);

	//横幅最大値をHP最大値で分割して1HP当たりの横幅を計算
	float widthOnce = (float)healthWidthMax / (float)healthMax;
	//現在のHPに掛け算して横幅を割り出す
	float nowWidth = widthOnce * health;
	//HPバーサイズ取得
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
	//hpを減らす
	health--;

	//次食らうクールタイムを設定
	damageInterval = damageCooltime;

	//HPが0になったら死亡させる
	if (health == 0) {
		Death();
		return;
	}


}

void Player::Death()
{
	//当たり判定をとりたくないので属性を無敵にする
	collider->SetAttribute(COLLISION_ATTR_INVINCIBLE);
	//次シーンに移行するまでのカウントダウン開始
	deathCount = deathCountMax;

	
}

void Player::UpdateDeath()
{
	if (deathCount == 0) {
		isAlive = false;
		return;
	}

	Object3d::Update();

	//死のカウントダウンを減らしながらパーティクルを出す
	deathCount--;
	//パーティクルの速度
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

	//パーティクルが収束する
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

	}//光輪を出しながらおおきくなる
	else {
		Vector3 defaultScale = { 1,1,1 };
		scale = defaultScale * EaseOut(eDataPlayerScale.GetTimeRate());
		//光輪がおおきくなりながら薄く
		for (size_t i = 0; i < haloMax; i++) {

			haloObjects[i]->scale += {haloScaleVel[i], haloScaleVel[i], haloScaleVel[i]};
			haloObjects[i]->color.w -= haloAlphaVel[i];
			
		}

		//自機が大きくなりきったらスポーン処理おわり
		if (eDataPlayerScale.GetTimeRate() >= 1.0f) {
			isSpawn = false;
			//UI系のα値のイージング開始
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

