#pragma once
#include"GameBaseScene.h"
#include"Sprite.h"

class GameOverScene : public GameBaseScene
{
public:
	//������
	void Initialize() override;

	//�I��
	void Finalize() override;

	//�X�V
	void Update() override;

	//�`��
	void Draw() override;

private:

	uint32_t gameoverTexture = 0;
	Sprite* gameoverSprite = nullptr;
};
