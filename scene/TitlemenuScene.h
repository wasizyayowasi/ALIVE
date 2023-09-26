#pragma once
#include "SceneBase.h"
#include <vector>
#include <string>

class InputState;

class TitlemenuScene : public SceneBase
{
public:
	TitlemenuScene(SceneManager& manager);
	virtual ~TitlemenuScene();

	void update(const InputState& input);
	void draw();

	void sceneChange();

private:

	int selectNum_ = 0;

	std::vector<std::string> menuName_;

};

