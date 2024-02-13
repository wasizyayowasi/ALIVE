#include "SceneMovie.h"
#include "SceneTitle.h"
#include "SceneManager.h"

#include "../util/game.h"
#include "../util/InputState.h"
#include "../util/FontsManager.h"

#include <string>
#include <DxLib.h>

SceneMovie::SceneMovie(SceneManager& manager):SceneBase(manager)
{
	movieHandle_[0] = LoadGraph("data/movie/movie1.mp4");
	movieHandle_[1] = LoadGraph("data/movie/movie2.mp4");
}

SceneMovie::~SceneMovie()
{
	for (auto& movie : movieHandle_) {
		DeleteGraph(movie);
	}
}

void SceneMovie::Init()
{
}

void SceneMovie::End()
{
}

void SceneMovie::Update()
{
	//短縮化
	auto& input = InputState::GetInstance();

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

	if (input.IsTriggered(InputType::Space)) {
		manager_.ChangeScene(std::shared_ptr<SceneBase>(std::make_shared<SceneTitle>(manager_)));
	}
}

void SceneMovie::Draw()
{
	PlayMovieToGraph(movieHandle_[currentMovieNum_]);
	DrawExtendGraph(0, 0, Game::screen_width, Game::screen_height, movieHandle_[currentMovieNum_], true);

	std::string str = "再生中";

	int width = GetDrawStringWidthToHandle(str.c_str(), static_cast<int>(str.size()), FontsManager::GetInstance().GetFontHandle("ピグモ 0042"));
	DrawStringToHandle(Game::screen_width - width * 2, Game::screen_height - 100, str.c_str(), 0xff0000, FontsManager::GetInstance().GetFontHandle("ピグモ 0042"));
}
