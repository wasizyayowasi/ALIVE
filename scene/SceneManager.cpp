#include "SceneManager.h"
#include "SceneBase.h"
#include "DxLib.h"
#include "util/game.h"

//�V�[���J��
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
	scenes_.push_front(std::shared_ptr<SceneBase>(scene));
	scenes_.front()->Init();
}

//���C���̃V�[����ۂ����܂ܕʂ̃V�[����O��ʂɕ\������
void SceneManager::PushFrontScene(std::shared_ptr<SceneBase> scene)
{
	//�擪�Ɉ����œ����V�[����ǉ�����
	scenes_.push_front(scene);
	scenes_.front()->Init();
}

//�V�[��������ւ���
void SceneManager::SwapScene(std::shared_ptr<SceneBase> scene)
{
	//�擪�̃V�[�����폜����
	while(scenes_.size() > 1) {
		scenes_.front()->End();
		scenes_.pop_front();
	}
	//�擪�Ɉ����œ����V�[����ǉ�����
	scenes_.push_front(std::shared_ptr<SceneBase>(scene));
	scenes_.front()->Init();
}

//�ЂƂO�̃V�[���ɖ߂�
void SceneManager::PopFrontScene()
{
	//�擪�̃V�[�����폜����
	if (scenes_.size() > 1) {
		scenes_.front()->End();
		scenes_.pop_front();
	}
}

//�X�V
void SceneManager::Update()
{
	LONGLONG start = GetNowHiPerformanceCount();
	
	//�擪�ɂ���V�[���̍X�V
	scenes_.front()->Update();

	debugUpdateTime_ = static_cast<int>(GetNowHiPerformanceCount() - start);
}

//�`��
void SceneManager::Draw()
{

	LONGLONG start = GetNowHiPerformanceCount();

	//�V�[�����ׂĂ̕`��
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

//�E�B���h�E���[�h��ύX����
void SceneManager::ChangeWindowMode(bool windowMode)
{
	windowMode_ = windowMode;
	DxLib::ChangeWindowMode(windowMode_);
}
