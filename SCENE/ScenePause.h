#pragma once
#include "SceneBase.h"
#include "DxLib.h"
#include <memory>
#include <vector>
#include <string>

class ScenePause : public SceneBase
{
public:
	//コンストラクタ
	ScenePause(SceneManager& manager);
	//デストラクタ
	virtual ~ScenePause();

	//初期化用
	void Init();
	//削除用
	void End();

	//更新
	void Update(const InputState& input);
	//描画
	void Draw();

private:

	int selectionNum_ = 0;		//選択番号

	bool isDecision_ = false;

	std::vector<std::string> menuName_;		//ポーズメニューの名前用
};

