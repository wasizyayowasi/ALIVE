#include "SelectChapterScene.h"
#include "GameMain.h"
#include "SceneManager.h"

#include "../object/ObjectData.h"

#include "../util/game.h"
#include "../util/Model.h"
#include "../UTIL/Easing.h"
#include "../util/InputState.h"
#include "../util/ExternalFile.h"
#include "../util/ModelManager.h"
#include "../util/GraphManager.h"

#include <algorithm>
#include <string>

namespace {
	//総時間
	constexpr float total_time = 60.0f;
}

//コンストラクタ
SelectChapterScene::SelectChapterScene(SceneManager& manager) : SceneBase(manager),updateFunc_(&SelectChapterScene::SlideInBook)
{
}

//デストラクタ
SelectChapterScene::~SelectChapterScene()
{
}

//初期化
void SelectChapterScene::Init()
{
	//インスタンス化
	model_ = std::make_shared<Model>(ModelManager::GetInstance().GetModelHandle(objData[static_cast<int>(ObjectType::Book)].name),Material::Other);

	//オブジェクト配置データ
	auto& file = ExternalFile::GetInstance();

	//モデル配置情報の取得
	auto info = file.GetSpecifiedInfo("title", "Book");

	//モデルの情報設定
	model_->SetPos(info.pos);
	model_->SetRot(info.rot);
	model_->SetScale(info.scale);

	//目標の座標の取得
	targetPosX_ = file.GetSpecifiedInfo("title", "BookPutPos").pos.x;

	model_->SetAnimation(static_cast<int>(BookAnim::normal), false, false);
}

//終了
void SelectChapterScene::End()
{
}

//更新
void SelectChapterScene::Update()
{
	(this->*updateFunc_)();
}

//描画
void SelectChapterScene::Draw()
{
	//モデルの描画
	model_->Draw();

	//画面全体を真っ黒に塗りつぶす
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeValue_);
	DrawBox(0, 0, Game::screen_width, Game::screen_height, fadeColor_, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

//チャプターを変更する
void SelectChapterScene::ChangeChapter()
{
	//短縮化
	auto& file = ExternalFile::GetInstance();

	//選ばれているチャプターの文字列を取得する
	std::string str = "Chapter" + std::to_string(selectNum_);

	//プレイヤーの開始位置を設定する
	file.SetStartName(str);
}

//本がスライドインしてくる
void SelectChapterScene::SlideInBook()
{
	//短縮化
	auto& file = ExternalFile::GetInstance();

	//モデルの座標
	VECTOR pos = model_->GetPos();

	//時間を増加させる
	elapsedTime_ = (std::min)(elapsedTime_ + 1.0f, total_time);

	//イージング
	pos.x = Easing::InOutCubic(elapsedTime_, total_time, targetPosX_, model_->GetPos().x);

	//モデルのポジション座標
	model_->SetPos(pos);

	if (elapsedTime_ == total_time) {
		updateFunc_ = &SelectChapterScene::NormalUpdate;
		targetPosX_ = file.GetSpecifiedInfo("title", "Book").pos.x;
		elapsedTime_ = 0;
	}
}

//通常の更新
void SelectChapterScene::NormalUpdate()
{
	//短縮化
	auto& input = InputState::GetInstance();
	auto& graph = GraphManager::GetInstance();

	//モデルの更新
	model_->Update();

	if (model_->IsAnimEnd()) {
		model_->ChangeAnimation(static_cast<int>(BookAnim::normal), false, false, 10);
	}
	else {
		return;
	}

	//チャプター選択
	if (input.IsTriggered(InputType::Right)) {
		if (selectNum_ < 2) {
			model_->ChangeAnimation(static_cast<int>(BookAnim::open), false, false, 10);
		}
		selectNum_ = (std::min)(selectNum_ + 1, 2);
	}
	if (input.IsTriggered(InputType::Left)) {
		if (selectNum_ > 0) {
			model_->ChangeAnimation(static_cast<int>(BookAnim::close), false, false, 10);
		}
		selectNum_ = (std::max)(selectNum_ - 1, 0);
	}

	//画像名の文字列の取得
	std::string str = "chapter" + std::to_string(selectNum_ + 1);

	MV1SetTextureGraphHandle(model_->GetModelHandle(), 0, graph.GetGraph(str), true);

	//戻る
	if (input.IsTriggered(InputType::Down)) {
		updateFunc_ = &SelectChapterScene::SlideOutBook;
	}

	//決定
	if (input.IsTriggered(InputType::Space)) {
		updateFunc_ = &SelectChapterScene::FadeOutUpdate;
	}
}

//本がスライドアウトしていく
void SelectChapterScene::SlideOutBook()
{
	//短縮化
	auto& file = ExternalFile::GetInstance();

	//モデルの座標
	VECTOR pos = model_->GetPos();

	//時間を増加させる
	elapsedTime_ = (std::min)(elapsedTime_ + 1.0f, total_time);

	//イージング
	pos.x = Easing::InOutCubic(elapsedTime_, total_time, targetPosX_, model_->GetPos().x);

	//モデルのポジション座標
	model_->SetPos(pos);

	if (elapsedTime_ == total_time) {
		manager_.PopFrontScene();
	}
}

//フェードアウト
void SelectChapterScene::FadeOutUpdate()
{
	fadeValue_ = static_cast <int>(255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_)));
	if (++fadeTimer_ == fadeInterval_) {
		ChangeChapter();
		manager_.ChangeScene(std::shared_ptr<SceneBase>(std::make_shared<GameMain>(manager_)));
		return;
	}
}
