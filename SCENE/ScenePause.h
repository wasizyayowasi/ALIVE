#pragma once
#include "SceneBase.h"
#include "DxLib.h"
#include <memory>

class ScenePause : public SceneBase
{
public:
	ScenePause(SceneManager& manager);
	virtual ~ScenePause();

	void update(const InputState& input);
	void draw();

private:

	int selectionNum_ = 0;		//‘I‘ğ”Ô†

	bool isDecision_ = false;
};

