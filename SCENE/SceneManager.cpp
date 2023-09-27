#include "SceneManager.h"
#include "SceneBase.h"

using namespace std;

void SceneManager::changeScene(std::shared_ptr<SceneBase> scene)
{
	//������V�[�������ׂč폜����
	while (!scenes_.empty())
	{
		//�擪�̃V�[�����폜����
		scenes_.front()->end();
		scenes_.pop_front();
	}
	//�擪�Ɉ����œ����V�[����ǉ�����
	scenes_.push_front(shared_ptr<SceneBase>(scene));
	scenes_.front()->init();
}

void SceneManager::pushScene(std::shared_ptr<SceneBase> scene)
{
	//�擪�Ɉ����œ����V�[����ǉ�����
	scenes_.push_front(scene);
	scenes_.front()->init();
}

void SceneManager::swapScene(std::shared_ptr<SceneBase> scene)
{
	//�擪�̃V�[�����폜����
	while(scenes_.size() > 1) {
		scenes_.front()->end();
		scenes_.pop_front();
	}
	//�擪�Ɉ����œ����V�[����ǉ�����
	scenes_.push_front(shared_ptr<SceneBase>(scene));
	scenes_.front()->init();
}

void SceneManager::popScene()
{
	//�擪�̃V�[���ȊO���폜����
	if (scenes_.size() > 1) {
		scenes_.front()->end();
		scenes_.pop_front();
	}
}

void SceneManager::update(const InputState& input)
{
	//�擪�ɂ���V�[���̍X�V
	scenes_.front()->update(input);
}

void SceneManager::draw()
{
	//�V�[�����ׂĂ̕`��
	for (int i = scenes_.size() - 1; i >= 0; --i) {
		scenes_[i]->draw();
	}
}