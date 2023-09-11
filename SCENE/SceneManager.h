#pragma once
#include <deque>

class SceneBase;
class InputState;

class SceneManager
{
public:
	void changeScene(SceneBase* scene);			//シーン遷移
	void pushScene(SceneBase* scene);			//メインのシーンを保ったまま別のシーンを前画面に表示する
	void swapScene(SceneBase* scene);			//シーンをすり替える
	void popScene();							//ひとつ前のシーンに戻る
	void update(const InputState& input);		//シーンの更新
	void draw();								//シーンの描画
private:
	std::deque<SceneBase*> scenes_;				//シーン本体
};

