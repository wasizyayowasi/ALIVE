#pragma once
#include <deque>
#include <memory>

class SceneBase;
class InputState;

class SceneManager
{
public:
	//シーン遷移
	void ChangeScene(std::shared_ptr<SceneBase> scene);
	//メインのシーンを保ったまま別のシーンを前画面に表示する
	void PushFrontScene(std::shared_ptr<SceneBase> scene);
	//メインのシーンを保ったまま別のシーンを後画面に表示する
	void PushBackScene(std::shared_ptr<SceneBase> scene);
	//シーンをすり替える
	void SwapScene(std::shared_ptr<SceneBase> scene);				
	//ひとつ前のシーンに戻る
	void PopFrontScene();											
	//シーンの更新
//	void Update(const InputState& input);							
	void Update();							
	//シーンの描画
	void Draw();													
	//メインwhile文を終了させるため
	bool End() { return isEnd_; };		
	//endフラグを変更する
	void SetEndFlag(bool flag) { isEnd_ = flag; }
	//ウィンドウモードを変更する
	void ChangeWindowMode(bool windowMode);
	//ウィンドウモードの取得
	bool GetWindowMode() { return windowMode_; }

private:

	std::deque<std::shared_ptr<SceneBase>> scenes_;					//シーン本体

	bool isEnd_ = false;

	int debugDrawTime_ = 0;
	int debugUpdateTime_ = 0;

	bool windowMode_ = true;

};

