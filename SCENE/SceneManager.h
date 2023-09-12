#pragma once
#include <deque>
#include <memory>

class SceneBase;
class InputState;

class SceneManager
{
public:
	void changeScene(std::shared_ptr<SceneBase> manager);			//シーン遷移
	void pushScene(std::shared_ptr<SceneBase> manager);			//メインのシーンを保ったまま別のシーンを前画面に表示する
	void swapScene(std::shared_ptr<SceneBase> manager);			//シーンをすり替える
	void popScene();							//ひとつ前のシーンに戻る
	void update(const InputState& input);		//シーンの更新
	void draw();								//シーンの描画
private:
	std::deque<std::shared_ptr<SceneBase>> scenes_;				//シーン本体
};

