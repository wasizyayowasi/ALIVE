#include "SceneManager.h"
#include "SceneBase.h"

using namespace std;

void SceneManager::changeScene(std::shared_ptr<SceneBase> scene)
{
	//������V�[�������ׂč폜����
	while (!scenes_.empty())
	{
		//�����̃V�[�����폜����
		scenes_.pop_front();
	}
	//�擪�Ɉ����œ����V�[����ǉ�����
	scenes_.push_front(shared_ptr<SceneBase>(scene));
}

void SceneManager::pushScene(std::shared_ptr<SceneBase> scene)
{
	//�擪�Ɉ����œ����V�[����ǉ�����
	scenes_.push_front(scene);
}

void SceneManager::swapScene(std::shared_ptr<SceneBase> scene)
{
	//�擪�̃V�[�����폜����
	if (scenes_.size() > 1) {
		scenes_.pop_front();
	}
	//�擪�Ɉ����œ����V�[����ǉ�����
	scenes_.push_front(shared_ptr<SceneBase>(scene));
}

void SceneManager::popScene()
{
	//�����̃V�[���ȊO���폜����
	if (scenes_.size() > 1) {
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