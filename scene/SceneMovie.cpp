#include "SceneMovie.h"
#include "SceneTitle.h"
#include "SceneManager.h"

#include "../util/game.h"
#include "../util/InputState.h"
#include "../util/FontsManager.h"

#include <string>
#include <DxLib.h>

//コンストラクタ
SceneMovie::SceneMovie(SceneManager& manager):SceneBase(manager)
{
	movieHandle_[0] = LoadGraph("data/movie/movie1.mp4");
	movieHandle_[1] = LoadGraph("data/movie/movie2.mp4");
}

//デストラクタ
SceneMovie::~SceneMovie()
{
	for (auto& movie : movieHandle_) {
		DeleteGraph(movie);
	}
}

//初期化
void SceneMovie::Init()
{
}

//終了
void SceneMovie::End()
{
}

//更新
void SceneMovie::Update()
{
	//短縮化
	auto& input = InputState::GetInstance();

	//再生する動画を変更するための変数を変更する
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

	//タイトルシーンに戻す
	if (input.IsTriggered(InputType::Space)) {
		manager_.ChangeScene(std::shared_ptr<SceneBase>(std::make_shared<SceneTitle>(manager_)));
	}
}

//描画
void SceneMovie::Draw()
{
	//動画の再生
	PlayMovieToGraph(movieHandle_[currentMovieNum_]);
	DrawExtendGraph(0, 0, Game::screen_width, Game::screen_height, movieHandle_[currentMovieNum_], true);

	//文字列の描画
	std::string str = "再生中";
	int width = GetDrawStringWidthToHandle(str.c_str(), static_cast<int>(str.size()), FontsManager::GetInstance().GetFontHandle("ピグモ 0042"));
	DrawStringToHandle(Game::screen_width - width * 2, Game::screen_height - 100, str.c_str(), 0xff0000, FontsManager::GetInstance().GetFontHandle("ピグモ 0042"));
}
