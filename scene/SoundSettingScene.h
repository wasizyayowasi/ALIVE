#pragma once
#include "SceneBase.h"

class InputState;

class SoundSettingScene : public SceneBase
{
public:

	SoundSettingScene(SceneManager& manager);
	virtual ~SoundSettingScene();

	void init();
	void end();

	void update(const InputState& input);
	void draw();

private:

	int barGraph_ = -1;
	int afterProcessingBarGraph_ = -1;
	int pictogramGraph_ = -1;

	int volumeBGM_ = 0;
	int volumeSE_ = 0;

};

