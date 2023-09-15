#include "SceneManager.h"
#include "SceneBase.h"

using namespace std;

void SceneManager::changeScene(std::shared_ptr<SceneBase> scene)
{
	//今あるシーンをすべて削除する
	while (!scenes_.empty())
	{
		//末尾のシーンを削除する
		scenes_.pop_front();
	}
	//先頭に引数で得たシーンを追加する
	scenes_.push_front(shared_ptr<SceneBase>(scene));
}

void SceneManager::pushScene(std::shared_ptr<SceneBase> scene)
{
	//先頭に引数で得たシーンを追加する
	scenes_.push_front(scene);
}

void SceneManager::swapScene(std::shared_ptr<SceneBase> scene)
{
	//先頭のシーンを削除する
	if (scenes_.size() > 1) {
		scenes_.pop_front();
	}
	//先頭に引数で得たシーンを追加する
	scenes_.push_front(shared_ptr<SceneBase>(scene));
}

void SceneManager::popScene()
{
	//末尾のシーン以外を削除する
	if (scenes_.size() > 1) {
		scenes_.pop_front();
	}
}

void SceneManager::update(const InputState& input)
{
	//先頭にあるシーンの更新
	scenes_.front()->update(input);
}

void SceneManager::draw()
{
	//シーンすべての描画
	for (int i = scenes_.size() - 1; i >= 0; --i) {
		scenes_[i]->draw();
	}
}