#pragma once
#include "SceneBase.h"

class GameEnd : public SceneBase
{
public:
	GameEnd(SceneManager& manager);
	virtual ~GameEnd();

	void Init();
	void End();

	void Update();
	void Draw();

private:
	void fadeInUpdate();
	void normalUpdate();
	void fadeOutUpdate();

	static constexpr int fadeInterval_ = 60;
	int fadeTimer_ = fadeInterval_;
	int fadeValue_ = 255;
	int fadeColor_ = 0x000000;

	void (GameEnd::* updateFunc_)();

};

