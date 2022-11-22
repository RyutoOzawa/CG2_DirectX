#include "sky.h"
#include<assert.h>
#include"affine.h"
void sky::Initialize() {

	model_->Initialize("skydome");
	worldTransform_.Initialize();

	worldTransform_.scale_ = { 500.0f,500.0f,500.0f };
	affine::makeMatIdentity(worldTransform_.matWorld_);
	affine::makeMatScale(worldTransform_.matWorld_, worldTransform_.scale_);
	worldTransform_.TransferMatrix();
}

void sky::Update() {

}

void sky::Draw(ViewProjection& viewProjection_) {
	model_->Draw(worldTransform_, viewProjection_);
}

