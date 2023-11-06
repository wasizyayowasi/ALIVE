#include "SceneManager.h"
#include "SceneBase.h"

using namespace std;

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
	scenes_.push_front(shared_ptr<SceneBase>(scene));
	scenes_.front()->Init();
}

void SceneManager::PushFrontScene(std::shared_ptr<SceneBase> scene)
{
	//先頭に引数で得たシーンを追加する
	scenes_.push_front(scene);
	scenes_.front()->Init();
}

void SceneManager::PushBackScene(std::shared_ptr<SceneBase> scene)
{
	//後尾に引数で得たシーンを追加
	scenes_.push_back(scene);
	scenes_.back()->Init();
}

void SceneManager::SwapScene(std::shared_ptr<SceneBase> scene)
{
	//先頭のシーンを削除する
	while(scenes_.size() > 1) {
		scenes_.front()->End();
		scenes_.pop_front();
	}
	//先頭に引数で得たシーンを追加する
	scenes_.push_front(shared_ptr<SceneBase>(scene));
	scenes_.front()->Init();
}

void SceneManager::PopScene()
{
	//先頭のシーン以外を削除する
	if (scenes_.size() > 1) {
		scenes_.front()->End();
		scenes_.pop_front();
	}
}

void SceneManager::Update(const InputState& input)
{
	//先頭にあるシーンの更新
	scenes_.front()->Update(input);
}

void SceneManager::Draw()
{
	//シーンすべての描画
	for (int i = scenes_.size() - 1; i >= 0; --i) {
		scenes_[i]->Draw();
	}
}