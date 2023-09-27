#pragma once
#include "SceneBase.h"
#include <vector>
#include <string>
#include <memory>

class InputState;
class UIItemManager;

class TitlemenuScene : public SceneBase
{
public:
	TitlemenuScene(SceneManager& manager);
	virtual ~TitlemenuScene();

	void init();
	void end();

	void update(const InputState& input);
	void draw();

	void sceneChange();

private:

	int titleHandle_ = -1;
	int selectNum_ = 0;

	std::vector<std::string> menuName_;
	std::shared_ptr<UIItemManager> UI_;

};

