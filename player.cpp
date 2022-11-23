#include "player.h"
#include"affine.h"
player::~player()
{
	delete model_;
	for (int i = 0; i < maxHP; i++)
	{
		delete spriteHP[i];
	}
	delete input_;
}
void player::Initialize(SpriteManager* spriteManager) {
	input_ = new Input;

	model_ = new Object3d;

	model_->Initialize("player");

	uint32_t texHP = Texture::LoadTexture(L"Resources/heart.png");

	for (int i = 0; i < maxHP; i++)
	{
		spriteHP[i]=new Sprite();
		spriteHP[i]->Initialize(spriteManager, texHP);
		spriteHP[i]->SetColor({ 1,1,1,1 });
		spriteHP[i]->SetAnchorPoint({ 0,0 });

		spriteHP[i]->SetSize({ 41.6,34.1 });

		spriteHP[i]->SetPos({ 30 + spriteHP[i]->GetSize().x * i + i * 10,30 });
	}

	worldTransform_.Initialize();
	worldTransform_.translation_ = Vector3(0.0f, 0.0f, 0.0f);
}

void player::Update() {

	//デスフラグの立った弾を削除
	bullets_.remove_if([](std::unique_ptr<playerBullet>& bullet) {
		return bullet->IsDead();
		});

	if (isDamageInterval == true)
	{
		damageInterval--;
		if (damageInterval <= 0)
		{
			isDamageInterval = false;
		}
	}

	//自機のワールド座標から移動ベクトルを計算
	Vector3 vectorX = { 1.0,0,0 };
	vectorX = affine::MatVector(worldTransform_.matWorld_, vectorX);
	vectorX.normalize();
	Vector3 vectorZ = { 0,0,1.0 };
	vectorZ = affine::MatVector(worldTransform_.matWorld_, vectorZ);
	vectorZ.normalize();

	Vector3 move = { 0,0,0 };
	Vector3 rot = { 0,0,0 };
	////プレイヤー移動処理
	if (input_->Updatekeypad(0))
	{
		////プレイヤー移動処理
		move.x += input_->PadAnalogStickLX() * vectorX.x;
		move.z += input_->PadAnalogStickLX() * vectorX.z;
		move.x += input_->PadAnalogStickLY() * vectorZ.x;
		move.z += input_->PadAnalogStickLY() * vectorZ.z;
		rot.y = input_->PadAnalogStickRX();
	}

	float AR;
	float BR;

	AR = pow((worldTransform_.translation_.x + move.x) - 0, 2) + pow((0 + worldTransform_.translation_.z + move.z), 2);
	BR = pow((50 - worldTransform_.scale_.x * 2), 2);

	if (AR <= BR)
	{
		worldTransform_.translation_ += move;
	}

	/*worldTransform_.translation_ += move;*/
	worldTransform_.rotation_.y += rot.y * affine::Deg2Rad;
	worldTransform_.rotation_.x += rot.x * affine::Deg2Rad;
	//移動限界座標
	const float kRotLimit = 30.0 * affine::Deg2Rad;

	//範囲を超えない処理
	worldTransform_.rotation_.x = max(worldTransform_.rotation_.x, -kRotLimit);
	worldTransform_.rotation_.x = min(worldTransform_.rotation_.x, +kRotLimit);
		if(input_->TriggerPadKey(XINPUT_GAMEPAD_A)&&jumpFlag==false)
		{
			jumpFlag = 1;
		}

	jump();

	affine::makeAffine(worldTransform_);
	worldTransform_.TransferMatrix();

	//キャラクター攻撃処理
	Attack();

	//弾更新
	for (std::unique_ptr<playerBullet>& bullet : bullets_) {
		bullet->Update();
	}
	/*debugText_->SetPos(10, 10);
	debugText_->Printf("%f,%f",AR,BR);

	debugText_->SetPos(10, 30);
	debugText_->Printf("%f,%f,%f", worldTransform_.translation_.x, worldTransform_.translation_.y,worldTransform_.translation_.z);*/

	//debugText_->SetPos(10, 110);
	//debugText_->Printf("playerHP%d", HP);
}

void player::Attack() {
	/*XINPUT_STATE joyState;
	if (!Input::GetInstance()->GetJoystickState(0, joyState))
	{
		return;
	}*/
	if (input_->TriggerPadKey(XINPUT_GAMEPAD_RIGHT_SHOULDER)) {
		//弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);
		velocity = affine::MatVector(worldTransform_.matWorld_, velocity);
		float len = sqrt(velocity.x * velocity.x + velocity.y * velocity.y + velocity.z * velocity.z);
		if (len != 0)
		{
			velocity /= len;
		}
		velocity *= kBulletSpeed;

		//弾の生成し、初期化
		std::unique_ptr<playerBullet> newBullet = std::make_unique<playerBullet>();
		newBullet->Initialize(worldTransform_.translation_, worldTransform_.rotation_, velocity);

		//弾の登録する
		bullets_.push_back(std::move(newBullet));
	}
}

void player::Draw(ViewProjection& viewProjection_) {
	if (HP > 0)
	{
		if (isDamageInterval == false)
		{
			model_->Draw(worldTransform_, viewProjection_);
		}
		else
		{
			if (damageInterval % 3 == 0)
			{
				model_->Draw(worldTransform_, viewProjection_);
			}
		}
	}

	/*model_->Draw(worldTransform3DReticle_, viewProjection_, textureHandle_);*/
	//弾描画
	for (std::unique_ptr<playerBullet>& bullet : bullets_) {
		bullet->Draw(viewProjection_);
	}
}
void player::DrawUI() {
	for (int i = 0; i < HP; i++)
	{
		spriteHP[i]->Draw();
	}
}
Vector3 player::GetworldPosition()
{
	//ワールド座標を入れる変数
	Vector3 worldpos;
	//ワールド行列の平行移動成分を取得（ワールド座標）
	worldpos.x = worldTransform_.matWorld_.m[3][0];
	worldpos.y = worldTransform_.matWorld_.m[3][1];
	worldpos.z = worldTransform_.matWorld_.m[3][2];
	return worldpos;
}
void player::SetEndMoveRotation(Vector3 Transform)
{
	worldTransform_.translation_ = { 0,0,0 };
	// 打つ方向に向けてオブジェクトを回転させる
	Vector3 velocity = Transform - worldTransform_.translation_;
	velocity.normalize();
	// Y軸周り角度(θy)
	worldTransform_.rotation_.y = std::atan2(velocity.x, velocity.z);
}
void player::TransformRset(bool endOrStart)
{
	if (endOrStart == false)
	{
		worldTransform_.translation_ = { 0,0,0 };
		worldTransform_.rotation_ = { 0,0,0 };
		jumpFlag = 0;
		gravitySpeed = defGravitySpeed;
		affine::makeAffine(worldTransform_);
		worldTransform_.TransferMatrix();
	}
	else
	{
		jumpFlag = 0;
		gravitySpeed = defGravitySpeed;
		worldTransform_.translation_.y = 0;
		affine::makeAffine(worldTransform_);
		worldTransform_.TransferMatrix();
	}
}
void player::Rset()
{
	HP = maxHP;

	TransformRset(false);

	jumpFlag = 0u;

	gravitySpeed = defGravitySpeed;

	isDamageInterval = false;

}
void player::OnCollision()
{
	if (isDamageInterval == false)
	{
		HP--;
		isDamageInterval = true;
		damageInterval = 60 * 5;
	}
}
void player::jump()
{
	if (jumpFlag == 1)
	{
		worldTransform_.translation_.y += gravitySpeed;
		gravitySpeed -= 0.06f;

		if (gravitySpeed <= 0)
		{
			jumpFlag = 2;
		}
	}

	//下がる
	if (jumpFlag == 2)
	{
		worldTransform_.translation_.y -= gravitySpeed;
		gravitySpeed += 0.06f;

		if (gravitySpeed >= defGravitySpeed)
		{
			jumpFlag = 0;
			gravitySpeed = defGravitySpeed;
			worldTransform_.translation_.y = 0.0f;
		}
	}
}
