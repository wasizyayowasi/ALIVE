#include "SceneManager.h"
#include "SceneBase.h"

using namespace std;

void SceneManager::changeScene(std::shared_ptr<SceneBase> scene)
{
	while (!scenes_.empty())
	{
		scenes_.pop_front();
	}
	scenes_.push_front(shared_ptr<SceneBase>(scene));
}

void SceneManager::pushScene(std::shared_ptr<SceneBase> scene)
{
	scenes_.push_front(scene);
}

void SceneManager::swapScene(std::shared_ptr<SceneBase> scene)
{
	scenes_.pop_front();
	scenes_.push_front(shared_ptr<SceneBase>(scene));
}

void SceneManager::popScene()
{
	if (scenes_.size() > 1) {
		scenes_.pop_front();
	}
}

void SceneManager::update(const InputState& input)
{
	scenes_.front()->update(input);
}

void SceneManager::draw()
{
	for (int i = scenes_.size() - 1; i >= 0; --i) {
		scenes_[i]->draw();
	}
}