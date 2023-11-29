#include "SceneManager.h"
#include "SceneBase.h"
#include "DxLib.h"
#include "util/game.h"

using namespace std;

void SceneManager::ChangeScene(std::shared_ptr<SceneBase> scene)
{
	//������V�[�������ׂč폜����
	while (!scenes_.empty())
	{
		//�擪�̃V�[�����폜����
		scenes_.front()->End();
		scenes_.pop_front();
	}
	//�擪�Ɉ����œ����V�[����ǉ�����
	scenes_.push_front(shared_ptr<SceneBase>(scene));
	scenes_.front()->Init();
}

void SceneManager::PushFrontScene(std::shared_ptr<SceneBase> scene)
{
	//�擪�Ɉ����œ����V�[����ǉ�����
	scenes_.push_front(scene);
	scenes_.front()->Init();
}

void SceneManager::PushBackScene(std::shared_ptr<SceneBase> scene)
{
	//����Ɉ����œ����V�[����ǉ�
	scenes_.push_back(scene);
	scenes_.back()->Init();
}

void SceneManager::SwapScene(std::shared_ptr<SceneBase> scene)
{
	//�擪�̃V�[�����폜����
	while(scenes_.size() > 1) {
		scenes_.front()->End();
		scenes_.pop_front();
	}
	//�擪�Ɉ����œ����V�[����ǉ�����
	scenes_.push_front(shared_ptr<SceneBase>(scene));
	scenes_.front()->Init();
}

void SceneManager::PopFrontScene()
{
	//�擪�̃V�[�����폜����
	if (scenes_.size() > 1) {
		scenes_.front()->End();
		scenes_.pop_front();
	}
}

void SceneManager::Update(const InputState& input)
{

	LONGLONG start = GetNowHiPerformanceCount();
	
	//�擪�ɂ���V�[���̍X�V
	scenes_.front()->Update(input);

	debugUpdateTime_ = GetNowHiPerformanceCount() - start;
}

void SceneManager::Draw()
{

	LONGLONG start = GetNowHiPerformanceCount();

	//�V�[�����ׂĂ̕`��
	for (int i = scenes_.size() - 1; i >= 0; --i) {
		scenes_[i]->Draw();
	}

	debugDrawTime_ = GetNowHiPerformanceCount() - start;

	float rate = static_cast<float>(debugUpdateTime_ + debugDrawTime_) / 16666.6f;
	int width = static_cast<int>(Game::screen_width * rate);
	DrawBox(0, Game::screen_height - 16, width, Game::screen_height, 0xff0000, true);

	rate = static_cast<float>(debugUpdateTime_) / 16666.6f;
	width = static_cast<int>(Game::screen_width * rate);
	DrawBox(0, Game::screen_height - 16, width, Game::screen_height, 0x0000ff, true);

}