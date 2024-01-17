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

	//‰Šú‰»
	void Init();
	//íœ
	void End();

	//XV
	void Update();
	//•`‰æ
	void Draw();

private:

	int selectNum_ = 0;

	std::list<std::string> sceneName_;
};

