#pragma once
#include "SceneBase.h"
#include <memory>
#include <vector>
#include <list>
#include <string>

class DebugScene : public SceneBase
{
public:

	DebugScene(SceneManager& manager);
	virtual ~DebugScene();

	//初期化
	void Init();
	//削除
	void End();

	//更新
	void Update(const InputState& input);
	//描画
	void Draw();

private:

	int selectNum_ = 0;

	std::list<std::string> sceneName_;
};

