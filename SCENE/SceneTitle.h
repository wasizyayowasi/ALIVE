#pragma once
#include "SceneBase.h"
#include<string>
#include <vector>

class SceneTitle : public SceneBase
{
public:
	SceneTitle(SceneManager& manager);
	virtual ~SceneTitle();

	void Init();
	void End();

	void Update(const InputState& input);
	void Draw();

private:

	int fontHandle_ = -1;
	int titleWidth = 0;

	void fadeInUpdate(const InputState& input);
	void normalUpdate(const InputState& input);
	void fadeOutUpdate(const InputState& input);

	static constexpr int fadeInterval_ = 60;
	int fadeTimer_ = fadeInterval_;
	int fadeValue_ = 255;
	int fadeColor_ = 0x000000;

	void (SceneTitle::* updateFunc_)(const InputState& input);
};

