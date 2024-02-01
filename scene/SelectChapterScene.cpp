#include "SelectChapterScene.h"
#include "GameMain.h"
#include "SceneManager.h"

#include "../object/ObjectData.h"

#include "../util/game.h"
#include "../util/Model.h"
#include "../util/InputState.h"
#include "../util/ExternalFile.h"
#include "../util/ModelManager.h"

#include <algorithm>
#include <string>

SelectChapterScene::SelectChapterScene(SceneManager& manager) : SceneBase(manager),updateFunc_(&SelectChapterScene::NormalUpdate)
{
}

SelectChapterScene::~SelectChapterScene()
{
}

void SelectChapterScene::Init()
{
	//インスタンス化
	model_ = std::make_shared<Model>(ModelManager::GetInstance().GetModelHandle(ObjectType::Book),Material::Other);

	//オブジェクト配置データ
	auto info = ExternalFile::GetInstance().GetSpecifiedInfo("title", "Book");

	//モデルの情報設定
	model_->SetPos(info.pos);
	model_->SetRot(info.rot);
	model_->SetScale(info.scale);

	textureHandle_[0] = LoadGraph("data/model/room/texture/Chapter1.png");
	textureHandle_[1] = LoadGraph("data/model/room/texture/Chapter2.png");
	textureHandle_[2] = LoadGraph("data/model/room/texture/Chapter3.png");

	model_->SetAnimation(static_cast<int>(BookAnim::normal), false, false);
}

void SelectChapterScene::End()
{
}

void SelectChapterScene::Update()
{
	(this->*updateFunc_)();
}

void SelectChapterScene::Draw()
{
	DrawString(0, 0, "aaaaaaaaaaaaaaaaaaaa", 0xffffff);

	model_->Draw();

	//画面全体を真っ黒に塗りつぶす
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeValue_);
	DrawBox(0, 0, Game::screen_width, Game::screen_height, fadeColor_, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void SelectChapterScene::ChangeChapter()
{
	//短縮化
	auto& file = ExternalFile::GetInstance();

	//選ばれているチャプターの文字列を取得する
	std::string str = "Chapter" + std::to_string(selectNum_);

	//スタートポジションを取得する
	VECTOR startPos = file.GetStartPos(str);

	//プレイヤーの開始位置を設定する
	file.SetPlayerInfo(startPos);
}

void SelectChapterScene::NormalUpdate()
{
	//モデルの更新
	model_->Update();

	//短縮化
	auto& input = InputState::GetInstance();

	if (model_->IsAnimEnd()) {
		model_->ChangeAnimation(static_cast<int>(BookAnim::normal), false, false, 10);
	}
	else {
		return;
	}

	//チャプター選択
	if (input.IsTriggered(InputType::left)) {
		if (selectNum_ < 2) {
			model_->ChangeAnimation(static_cast<int>(BookAnim::open), false, false, 10);
			MV1SetTextureGraphHandle(model_->GetModelHandle(), 0, textureHandle_[(std::min)(selectNum_ + 1, 2)], true);
		}
		selectNum_ = (std::min)(selectNum_ + 1, 2);
	}
	if (input.IsTriggered(InputType::right)) {
		if (selectNum_ > 0) {
			model_->ChangeAnimation(static_cast<int>(BookAnim::close), false, false, 10);
			MV1SetTextureGraphHandle(model_->GetModelHandle(), 0, textureHandle_[(std::max)(selectNum_ - 1, 0)], true);
		}
		selectNum_ = (std::max)(selectNum_ - 1, 0);
	}

	//決定
	if (input.IsTriggered(InputType::space)) {
		updateFunc_ = &SelectChapterScene::FadeOutUpdate;
	}
}

void SelectChapterScene::FadeOutUpdate()
{
	fadeValue_ = static_cast <int>(255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_)));
	if (++fadeTimer_ == fadeInterval_) {
		ChangeChapter();
		manager_.ChangeScene(std::shared_ptr<SceneBase>(std::make_shared<GameMain>(manager_)));
		return;
	}
}
