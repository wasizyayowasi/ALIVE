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

	//初期化
	void Init();
	//終了
	void End();

	//更新
	void Update(const InputState& input);
	//描画
	void Draw();

	//シーンの変更
	void SceneChange();

private:

	int titleHandle_ = -1;					//タイトル画像の保管変数
	int selectNum_ = 0;						//選択

	std::vector<std::string> menuName_;		//メニューの文字列
	std::shared_ptr<UIItemManager> UI_;		//UIマネージャーのスマートポインタ

};

