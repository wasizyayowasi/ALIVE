#pragma once
#include "SceneBase.h"

class InputState;

class SoundSettingScene : public SceneBase
{
public:

	SoundSettingScene(SceneManager& manager);
	virtual ~SoundSettingScene();

	void Init();
	void End();

	void Update(const InputState& input);
	void Draw();

private:

	int barGraph_ = -1;
	int afterProcessingBarGraph_ = -1;
	int pictogramGraph_ = -1;

	int volumeBGM_ = 0;
	int volumeSE_ = 0;

};

