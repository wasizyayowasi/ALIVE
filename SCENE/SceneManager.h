#pragma once
#include <deque>
#include <memory>

class SceneBase;
class InputState;

class SceneManager
{
public:
	void ChangeScene(std::shared_ptr<SceneBase> manager);			//シーン遷移
	void PushScene(std::shared_ptr<SceneBase> manager);			//メインのシーンを保ったまま別のシーンを前画面に表示する
	void SwapScene(std::shared_ptr<SceneBase> manager);			//シーンをすり替える
	void PopScene();							//ひとつ前のシーンに戻る
	void Update(const InputState& input);		//シーンの更新
	void Draw();								//シーンの描画
	bool End() { return isEnd_; };				//メインwhile文を終了させるため
	void SetEndFlag(bool flag) { isEnd_ = flag; }
private:
	std::deque<std::shared_ptr<SceneBase>> scenes_;				//シーン本体

	bool isEnd_ = false;

};

