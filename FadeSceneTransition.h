#pragma once
#include"BaseSceneTransition.h"


class FadeSceneTransition:public BaseSceneTransition
{
public:

	void Initialize() override;

	void Finalize()override;

	void Update()override;

	void Draw()override;

	void Close()override;

	void Change()override;

	void Open()override;
};

