#pragma once
#include"GameBaseScene.h"
#include"Sprite.h"

class GameOverScene : public GameBaseScene
{
public:
	//‰Šú‰»
	void Initialize() override;

	//I—¹
	void Finalize() override;

	//XV
	void Update() override;

	//•`‰æ
	void Draw() override;

private:

	TextureData* gameoverTexture = 0;
	Sprite* gameoverSprite = nullptr;
};

