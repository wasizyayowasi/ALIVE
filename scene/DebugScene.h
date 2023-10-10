#pragma once
#include "SceneBase.h"
#include <memory>
#include <vector>
#include <map>
#include <string>

enum class SceneType {
	main,
	title,
	end,
	pause,
	max,
};

struct Scene {
	SceneType scene;
	std::string sceneNameText;
};

class DebugScene : public SceneBase
{
public:

	DebugScene(SceneManager& manager);
	virtual ~DebugScene();

	void Init();
	void End();

	void Update(const InputState& input);
	void Draw();

private:

	int selectNum_ = 0;

	std::vector<std::shared_ptr<SceneBase>> scene_;
	std::map<int, Scene> sceneName_;
};

