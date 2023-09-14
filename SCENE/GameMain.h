#pragma once
#include "SceneBase.h"
#include "DxLib.h"
#include <memory>
#include <vector>

class Camera;
class Player;
class Broom;
class DepthOfField;
class Model;

class GameMain : public SceneBase
{
public:
	GameMain(SceneManager& manager);
	virtual ~GameMain();

	void update(const InputState& input);
	void draw();

private:

	void fadeInUpdate(const InputState& input);
	void normalUpdate(const InputState& input);
	void fadeOutUpdate(const InputState& input);

	static constexpr int fadeInterval_ = 60;
	int fadeTimer_ = fadeInterval_;
	int fadeValue_ = 255;
	int fadeColor_ = 0x000000;

	std::shared_ptr<Camera> camera_;
	std::shared_ptr<Player> player_;
	std::shared_ptr<Broom> broom_;
	std::shared_ptr<DepthOfField> depthOfField_;
	
	std::vector<std::shared_ptr<Model>> models_;

	void (GameMain::* updateFunc_)(const InputState& input);

};

