#include "SceneManager.h"
#include "SceneBase.h"
#include "DxLib.h"
#include "util/game.h"

//シーン遷移
void SceneManager::ChangeScene(std::shared_ptr<SceneBase> scene)
{
	//今あるシーンをすべて削除する
	while (!scenes_.empty())
	{
		//先頭のシーンを削除する
		scenes_.front()->End();
		scenes_.pop_front();
	}
	//先頭に引数で得たシーンを追加する
	scenes_.push_front(std::shared_ptr<SceneBase>(scene));
	scenes_.front()->Init();
}

//メインのシーンを保ったまま別のシーンを前画面に表示する
void SceneManager::PushFrontScene(std::shared_ptr<SceneBase> scene)
{
	//先頭に引数で得たシーンを追加する
	scenes_.push_front(scene);
	scenes_.front()->Init();
}

//シーンをすり替える
void SceneManager::SwapScene(std::shared_ptr<SceneBase> scene)
{
	//先頭のシーンを削除する
	while(scenes_.size() > 1) {
		scenes_.front()->End();
		scenes_.pop_front();
	}
	//先頭に引数で得たシーンを追加する
	scenes_.push_front(std::shared_ptr<SceneBase>(scene));
	scenes_.front()->Init();
}

//ひとつ前のシーンに戻る
void SceneManager::PopFrontScene()
{
	//先頭のシーンを削除する
	if (scenes_.size() > 1) {
		scenes_.front()->End();
		scenes_.pop_front();
	}
}

//更新
void SceneManager::Update()
{
	LONGLONG start = GetNowHiPerformanceCount();
	
	//先頭にあるシーンの更新
	scenes_.front()->Update();

	debugUpdateTime_ = static_cast<int>(GetNowHiPerformanceCount() - start);
}

//描画
void SceneManager::Draw()
{

	LONGLONG start = GetNowHiPerformanceCount();

	//シーンすべての描画
	for (int i = static_cast<int>(scenes_.size() - 1); i >= 0; --i) {
		scenes_[i]->Draw();
	}

	debugDrawTime_ = static_cast<int>(GetNowHiPerformanceCount() - start);

#ifdef _DEBUG
	float rate = static_cast<float>(debugUpdateTime_ + debugDrawTime_) / 16666.6f;
	int width = static_cast<int>(Game::screen_width * rate);
	DrawBox(0, Game::screen_height - 16, width, Game::screen_height, 0xff0000, true);

	rate = static_cast<float>(debugUpdateTime_) / 16666.6f;
	width = static_cast<int>(Game::screen_width * rate);
	DrawBox(0, Game::screen_height - 16, width, Game::screen_height, 0x0000ff, true);

	DrawBox( 0, Game::screen_height - 48, 16, Game::screen_height - 32, 0x0000ff, true);
	DrawBox(64, Game::screen_height - 48, 80, Game::screen_height - 32, 0xff0000, true);

	DrawString( 0, Game::screen_height - 32, "update", 0xffffff);
	DrawString(64, Game::screen_height - 32, "draw", 0xffffff);
#endif
}

//ウィンドウモードを変更する
void SceneManager::ChangeWindowMode(bool windowMode)
{
	windowMode_ = windowMode;
	DxLib::ChangeWindowMode(windowMode_);
}
