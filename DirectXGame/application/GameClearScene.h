#pragma once
#include"GameBaseScene.h"
#include"Sprite.h"
class GameClearScene : public GameBaseScene
{
public:
	void Initialize() override;

	void Finalize() override;

	void Update() override;

	void Draw() override;
private:

	TextureData* gameClearTexture = 0;
	Sprite* gameClearSprite = nullptr;
};

