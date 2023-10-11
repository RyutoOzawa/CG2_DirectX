#pragma once
#include"BaseSceneTransition.h"
#include"Sprite.h"
#include"EasingData.h"

class FadeSceneTransition:public BaseSceneTransition
{
public:

	void Initialize() override;

	void Finalize()override;

	void Update()override;

	void Draw()override;

	void Close()override;


	void Open()override;

private:

	std::unique_ptr<Sprite> blackSprite = nullptr;
	TextureData* whiteTex = nullptr;

	EasingData  easeSpriteAlpha;

};

