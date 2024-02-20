#include "SceneMovie.h"
#include "SceneTitle.h"
#include "SceneManager.h"

#include "../util/game.h"
#include "../util/InputState.h"
#include "../util/FontsManager.h"

#include <string>
#include <DxLib.h>

//�R���X�g���N�^
SceneMovie::SceneMovie(SceneManager& manager):SceneBase(manager)
{
	movieHandle_[0] = LoadGraph("data/movie/movie1.mp4");
	movieHandle_[1] = LoadGraph("data/movie/movie2.mp4");
}

//�f�X�g���N�^
SceneMovie::~SceneMovie()
{
	for (auto& movie : movieHandle_) {
		DeleteGraph(movie);
	}
}

//������
void SceneMovie::Init()
{
}

//�I��
void SceneMovie::End()
{
}

//�X�V
void SceneMovie::Update()
{
	//�Z�k��
	auto& input = InputState::GetInstance();

	//�Đ����铮���ύX���邽�߂̕ϐ���ύX����
	if (GetMovieStateToGraph(movieHandle_[currentMovieNum_]) == 0) {
		switch (currentMovieNum_)
		{
		case 0:
			currentMovieNum_ = 1;
			break;
		case 1:
			currentMovieNum_ = 0;
			break;
		}
	}

	//�^�C�g���V�[���ɖ߂�
	if (input.IsTriggered(InputType::Space)) {
		manager_.ChangeScene(std::shared_ptr<SceneBase>(std::make_shared<SceneTitle>(manager_)));
	}
}

//�`��
void SceneMovie::Draw()
{
	//����̍Đ�
	PlayMovieToGraph(movieHandle_[currentMovieNum_]);
	DrawExtendGraph(0, 0, Game::screen_width, Game::screen_height, movieHandle_[currentMovieNum_], true);

	//������̕`��
	std::string str = "�Đ���";
	int width = GetDrawStringWidthToHandle(str.c_str(), static_cast<int>(str.size()), FontsManager::GetInstance().GetFontHandle("�s�O�� 0042"));
	DrawStringToHandle(Game::screen_width - width * 2, Game::screen_height - 100, str.c_str(), 0xff0000, FontsManager::GetInstance().GetFontHandle("�s�O�� 0042"));
}
