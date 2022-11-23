#include "BossPhase_1.h"
#include"Util.h"


BossPhase_1::~BossPhase_1()
{
	delete model_;
	delete spriteHP;
	delete spriteHPBar;
	delete medamaModel;
}

void BossPhase_1::Initialize(SpriteManager* spriteManager)
{
	

	model_ = new Object3d();

	model_->Initialize("BossCube");

	for (int i = 0; i < 27; i++) {

		worldTransform_[i].Initialize();
		// フラグ
		AnnihilationFlag[i] = false;
	}
	int texHP= Texture::LoadTexture(L"Resources/bossBarNaka.png");
	int texHPBar = Texture::LoadTexture(L"Resources/bossBar.png");

	spriteHP = new Sprite();

	spriteHP->Initialize(spriteManager,texHP/*, { 330.0f,610.0f }, { 1,1,1,1 }, { 0,0 }*/);

	spriteHP->SetColor({ 1,1,1,1 });
	spriteHP->SetAnchorPoint({ 0,0 });

	spriteHP->SetPos({ 330.0f,610.0f });
	spriteHP->SetSize({ 620,50 });

	spriteHPBar = new Sprite();
	spriteHPBar->Initialize(spriteManager,texHPBar);

	spriteHPBar->SetColor({ 1,1,1,1 });
	spriteHPBar->SetAnchorPoint({ 0,0 });

	spriteHPBar->SetPos({ 320,600 });
	spriteHPBar->SetSize({ 640,50 });

	// 親
	worldTransform_[0].translation_ = { 0,10,50 };

	// ボスバレットの初期化
	bullet = std::make_unique<BossBullet>();
	bullet->SetPos(worldTransform_[0].translation_);

	// 子の座標設定

	// 真ん中の段
	worldTransform_[1].translation_ = { +2, 0,-2 };
	worldTransform_[2].translation_ = { 0, 0,-2 };
	worldTransform_[3].translation_ = { -2, 0,-2 };
	worldTransform_[4].translation_ = { -2, 0, 0 };
	worldTransform_[5].translation_ = { -2, 0,+2 };
	worldTransform_[6].translation_ = { 0, 0,+2 };
	worldTransform_[7].translation_ = { +2, 0,+2 };
	worldTransform_[8].translation_ = { +2, 0, 0 };
	// 上の段
	worldTransform_[9].translation_ = { 0,+2, 0 };
	worldTransform_[10].translation_ = { +2,+2,-2 };
	worldTransform_[11].translation_ = { 0,+2,-2 };
	worldTransform_[12].translation_ = { -2,+2,-2 };
	worldTransform_[13].translation_ = { -2,+2, 0 };
	worldTransform_[14].translation_ = { -2,+2,+2 };
	worldTransform_[15].translation_ = { 0,+2,+2 };
	worldTransform_[16].translation_ = { +2,+2,+2 };
	worldTransform_[17].translation_ = { +2,+2, 0 };
	// 下の段
	worldTransform_[18].translation_ = { 0,-2, 0 };
	worldTransform_[19].translation_ = { +2,-2,-2 };
	worldTransform_[20].translation_ = { 0,-2,-2 };
	worldTransform_[21].translation_ = { -2,-2,-2 };
	worldTransform_[22].translation_ = { -2,-2, 0 };
	worldTransform_[23].translation_ = { -2,-2,+2 };
	worldTransform_[24].translation_ = { 0,-2,+2 };
	worldTransform_[25].translation_ = { +2,-2,+2 };
	worldTransform_[26].translation_ = { +2,-2, 0 };

	// 子
	for (int i = 1; i < 27; i++) {
		worldTransform_[i].parent_ = &worldTransform_[0];
	}

	//形態変化で使う目玉用初期化
	medamaModel = new Object3d();
	medamaModel->Initialize("Medama");
	medamaWorldTransform.Initialize();
	medamaWorldTransform.translation_ = worldTransform_[0].translation_;
	medamaWorldTransform.TransferMatrix();

	damageSound.SoundLoadWave("Resources/Sound/bosssDamage.wav");
}

void BossPhase_1::Update(Vector3 playerPos)
{

	TurnBodyToPlayer(playerPos);

	rotaAngle = worldTransform_[0].rotation_;


	// 行列の更新と転送
	TransferMat();

	// ランダムに選ばれたブロックが消えたら弾の処理を開始
	FlyBlocks(playerPos);

	// 行列の更新と転送
	TransferMat();

	spriteHP->SetSize({ float((620 / maxHP)*HP), 25 });

	/*debugText_->SetPos(10,70);
	debugText_->Printf("boss1HP%d",HP);*/
}

void BossPhase_1::ChangeUpdate(float animationTimer, float animationMax)
{
	for (int i = 0; i < 27; i++) {
		AnnihilationFlag[i] = false;
	}
	
	//8つの欠片の飛んでいく速度
	float breakSpdTemp = 0.25f;
	Vector3 cornerBreakSpd[8]{
		{ +breakSpdTemp, +breakSpdTemp, -breakSpdTemp},	//10
		{ -breakSpdTemp, +breakSpdTemp, -breakSpdTemp},	//12
		{ -breakSpdTemp, +breakSpdTemp, +breakSpdTemp},	//14
		{ +breakSpdTemp, +breakSpdTemp, +breakSpdTemp},	//16
		{ +breakSpdTemp, -breakSpdTemp, -breakSpdTemp},	//19
		{ -breakSpdTemp, -breakSpdTemp, -breakSpdTemp},	//21
		{ -breakSpdTemp, -breakSpdTemp, +breakSpdTemp},	//23
		{ +breakSpdTemp, -breakSpdTemp, +breakSpdTemp},	//25
	};

	//速度をワールド行列と掛け算し、速度として加算と行列転送
	for (int i = 0; i < 8; i++) {
		cornerBreakSpd[i] = affine::MatVector(worldTransform_[0].matWorld_, cornerBreakSpd[i]);
		//速度をほんの少しランダムに増減
		float temp = breakSpdTemp / 2.0f;
		cornerBreakSpd[i].x += Random(-temp, temp);
		cornerBreakSpd[i].y += Random(-temp, temp);
		cornerBreakSpd[i].z += Random(-temp, temp);

		switch (i)
		{
		case 0:
			worldTransform_[10].translation_ += cornerBreakSpd[i];
			worldTransform_[10].TransferMatrix();
			break;
		case 1:
			worldTransform_[12].translation_ += cornerBreakSpd[i];
			worldTransform_[12].TransferMatrix();
			break;
		case 2:
			worldTransform_[14].translation_ += cornerBreakSpd[i];
			worldTransform_[14].TransferMatrix();
			break;
		case 3:
			worldTransform_[16].translation_ += cornerBreakSpd[i];
			worldTransform_[16].TransferMatrix();
			break;
		case 4:
			worldTransform_[19].translation_ += cornerBreakSpd[i];
			worldTransform_[19].TransferMatrix();
			break;
		case 5:
			worldTransform_[21].translation_ += cornerBreakSpd[i];
			worldTransform_[21].TransferMatrix();
			break;
		case 6:
			worldTransform_[23].translation_ += cornerBreakSpd[i];
			worldTransform_[23].TransferMatrix();
			break;
		case 7:
			worldTransform_[25].translation_ += cornerBreakSpd[i];
			worldTransform_[25].TransferMatrix();
			break;
		default:
			break;
		}
	}

	float ease = animationTimer / animationMax*2;
	if (ease >= 1.0f) {
		ease = 1;
	}

	//目玉の座標を線形補完する
	Vector3 trans{}, afterPos;
	afterPos = { 0,0,-3.2f };
	afterPos = affine::MatVector(worldTransform_[0].matWorld_, afterPos);
	afterPos += worldTransform_[0].translation_;
	trans = trans.lerp(worldTransform_[0].translation_, afterPos, ease);
	medamaWorldTransform.translation_ = trans;
//	medamaWorldTransform.translation_ = afterPos;
	medamaWorldTransform.TransferMatrix();

	float frontBlockAlpha = 0;
}

void BossPhase_1::TitleUpdate()
{
	//横に回転しながら浮遊
	titleRadian += 0.75f;
	if (titleRadian >= 360.0f) {
		titleRadian -= 360.0f;
	}

	worldTransform_[0].rotation_.y =titleRadian*affine::Deg2Rad;
	worldTransform_[0].translation_.y = (sin(affine::Deg2Rad * titleRadian) * 2.0f) + 12;

	TransferMat();
}

void BossPhase_1::Draw(ViewProjection viewprojection)
{
	for (int i = 0; i < 27; i++) {
		if (AnnihilationFlag[i] == false)
		{
			model_->Draw(worldTransform_[i], viewprojection);
		}
	}

	// 発射開始したら弾の描画をする
	if (AnnihilationFlag[randomBlock] == true && bullet->GetToPlayerFlag() == false) {
		bullet->Draw(viewprojection);
	}
}

void BossPhase_1::MedamaDraw(ViewProjection viewprojection)
{
	medamaModel->Draw(medamaWorldTransform, viewprojection);
}

void BossPhase_1::DrawUI()
{
	spriteHP->Draw();
	spriteHPBar->Draw();
}

void BossPhase_1::OnCollision()
{
	HP--;
	damageSound.SoundPlayWave(false,1.0);
}

void BossPhase_1::Rset()
{
	// 親
	worldTransform_[0].translation_ = { 0,10,50 };
	worldTransform_[0].rotation_ = { 0,0,0 };


	// 上の段
	worldTransform_[10].translation_ = { +2,+2,-2 };
	worldTransform_[12].translation_ = { -2,+2,-2 };
	worldTransform_[14].translation_ = { -2,+2,+2 };
	worldTransform_[16].translation_ = { +2,+2,+2 };
	// 下の段
	worldTransform_[19].translation_ = { +2,-2,-2 };
	worldTransform_[21].translation_ = { -2,-2,-2 };
	worldTransform_[23].translation_ = { -2,-2,+2 };
	worldTransform_[25].translation_ = { +2,-2,+2 };

	worldTransform_[randomBlock].scale_ = { 1.0f,1.0f,1.0f };
	AnnihilationFlag[randomBlock] = false;
	randomBlock = 0;
	bullet->Reset();
	bullet->SetPos(worldTransform_[0].translation_);

	// ブロックを浮かし終わるまでのフラグ
	FloatBlockFlagM = false; // 座標をマイナス
	FloatBlockFlagP = false; // 座標をプラス

	//タイトル挙動用回転角
	titleRadian = 0;

	HP = maxHP;
}

void BossPhase_1::SetRotation(Vector3 rotation)
{
	worldTransform_[0].rotation_ = rotation;
	TransferMat();
}

void BossPhase_1::FlyBlocks(Vector3 playerPos)
{
	// ランダムに抽選を行う
	FloatRandomBlocks();

	// 発射開始したら弾の更新を行う
	if (AnnihilationFlag[randomBlock] == true) {
		bullet->Update(playerPos);
		// スケールを小さくする
		worldTransform_[randomBlock].scale_ = { 0.0f,0.0f,0.0f };
	}


	// 弾がプレイヤーの位置に行ったら
	if (bullet->GetToPlayerFlag() == true) {
		// 徐々に元のものを拡大して再生させる
		expansionScale += expansionScaleSpeed;

		worldTransform_[randomBlock].scale_.x = min(expansionScale.x, 1);
		worldTransform_[randomBlock].scale_.y = min(expansionScale.y, 1);
		worldTransform_[randomBlock].scale_.z = min(expansionScale.z, 1);

		AnnihilationFlag[randomBlock] = false;
		bullet->SetPos(worldTransform_[0].translation_ + oldPos);

	}
	else
	{
		expansionScale = { 0,0,0 };
	}

	// 元のものがスケール元に戻ったらリセットをする
	if (worldTransform_[randomBlock].scale_.x >= 1.0f) {
		ResetFlyBlocks();
	}


}

void BossPhase_1::FloatRandomBlocks()
{
	// 拒否してるブロック以外だったら再抽選を終わらせる
	while (randomBlock == 0 || randomBlock == 2 || randomBlock == 11 || randomBlock == 20 || randomBlock == 6 || randomBlock == 15 || randomBlock == 24) {
		randomBlock = rand() % 26 + 1;
		oldPos = worldTransform_[randomBlock].translation_;
		// 弾の登録
		bullet->Initialize(model_, worldTransform_[randomBlock], worldTransform_[0].translation_, rotaAngle);
	}

	// 再抽選が終わったら
	if (worldTransform_[randomBlock].translation_.x > worldTransform_[0].translation_.x ||
		worldTransform_[randomBlock].translation_.x < worldTransform_[0].translation_.x)
	{
		AnnihilationFlag[randomBlock] = true;
	}

}

// 浮くブロックに関する初期化の処理
void BossPhase_1::ResetFlyBlocks()
{
	randomBlock = 0;
	FloatBlockFlagM = false;
	FloatBlockFlagP = false;
	for (int i = 0; i < 27; i++) {
		// 元になるものを消すフラグのリセット
		AnnihilationFlag[i] = false;
	}
}

void BossPhase_1::TransferMat()
{
	for (int i = 0; i < 27; i++) {
		affine::makeAffine(worldTransform_[i]);
		if (i != 0) {
			worldTransform_[i].matWorld_ *= worldTransform_[0].matWorld_;
		}
		worldTransform_[i].TransferMatrix();
	}
}

void BossPhase_1::TurnBodyToPlayer(Vector3 playerPos)
{
	// 打つ方向に向けてオブジェクトを回転させる
	Vector3 velocity = playerPos - worldTransform_[0].translation_;
	velocity.normalize();
	// Y軸周り角度(θy)
	worldTransform_[0].rotation_.y = std::atan2(velocity.x, velocity.z);
	// Y軸周りに-θy回す回転行列を計算
	Matrix4 RotY;
	affine::makeMatRotY(RotY, -worldTransform_[0].rotation_.y);
	// velosity_に回転行列を掛け算してvelosityZを求める
	Vector3 velosityZ = velocity;
	velosityZ = affine::MatVector(RotY, velosityZ);

	// X軸周り角度(θx)
	worldTransform_[0].rotation_.x = std::atan2(-velosityZ.y, velosityZ.z);
}

