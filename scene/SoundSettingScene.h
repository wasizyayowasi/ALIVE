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

	float volumeBGM_ = 0.0f;
	float volumeSE_ = 0.0f;

};

