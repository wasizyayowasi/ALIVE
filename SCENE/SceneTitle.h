#pragma once
#include "SceneBase.h"
#include<string>
#include <vector>
#include <memory>

class UIItemManager;

class SceneTitle : public SceneBase
{
public:
	SceneTitle(SceneManager& manager);
	virtual ~SceneTitle();

	void Init();
	void End();

	void Update();
	void Draw();


private:

	void SceneChange();

	void FadeInUpdate();
	void NormalUpdate();
	void FadeOutUpdate();

private:
	int fontHandle_ = -1;
	int titleWidth = 0;


	static constexpr int fadeInterval_ = 30;
	int fadeTimer_ = 0;
	int fadeValue_ = 0;
	int fadeColor_ = 0x000000;

	int titleHandle_ = -1;					//タイトル画像の保管変数
	int selectNum_ = 0;						//選択

	std::vector<std::string> menuName_;		//メニューの文字列
	std::shared_ptr<UIItemManager> UI_;		//UIマネージャーのスマートポインタ

	void (SceneTitle::* updateFunc_)();
};

