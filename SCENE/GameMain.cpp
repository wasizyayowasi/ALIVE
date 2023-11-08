#include "GameMain.h"
#include "SceneManager.h"
#include "GameEnd.h"

#include "../gimmick/GimmickBase.h"
#include "../gimmick/Switch.h"
#include "../gimmick/Steelyard.h"

#include "../object/Player.h"
#include "../object/cube.h"
#include "../object/Camera.h"
#include "../object/CharacterBase.h"

#include "../staging/Broom.h"
#include "../staging/DepthOfField.h"

#include "ScenePause.h" 

#include "../util/game.h"
#include "../util/SoundManager.h"
#include "../util/InputState.h"
#include "../util/model.h"
#include "../util/ExternalFile.h"
#include "../util/CheckCollisionModel.h"

#include "../util/ObjectManager.h"
#include "../util/ObjectData.h"

using namespace std;

namespace {
	const char* const player_Filename = "data/player/player16.mv1";
}

GameMain::GameMain(SceneManager& manager) : SceneBase(manager),
updateFunc_(&GameMain::FadeInUpdate)
{
}

GameMain::~GameMain()
{
	//ゲームメインが終わるときにプレイ中に死んだ回数と
	//saveDataに記録されている死亡回数を足す
	totalDeathNum_ += player_->GetDeathCount();
	ExternalFile::GetInstance().SaveDataRewriteInfo(checkPoint_, totalDeathNum_);
}

void GameMain::Init()
{
	makeScreenHandle_ = MakeScreen(Game::screen_width, Game::screen_height, true);

	checkCollisionModel_ = std::make_shared<CheckCollisionModel>();
	objManager_ = std::make_shared<ObjectManager>();

	//オブジェクトを生成
	ObjectGenerater();

	//broom_ = make_shared<Broom>();
	//depthOfField_ = make_shared<DepthOfField>();

	//仮でライト処理を消している
	SetUseLighting(false);

	//1mの範囲を設定する
	Set3DSoundOneMetre(10.0f);

	//セーブデータの内容を読み取る
	auto data = ExternalFile::GetInstance();
	//死亡回数
	totalDeathNum_ = data.GetSaveData().totalDeathNum;

	//3Dリスナーの位置を設定する
	SoundManager::GetInstance().Set3DSoundListenerInfo(camera_->GetPos(), camera_->GetTarget());
	//3Dサウンドに関連する情報を設定する
	SoundManager::GetInstance().Set3DSoundInfo(VGet(575, 120, -60), 1000, "cafe");
	//仮でcafeという音楽を流している
	SoundManager::GetInstance().Play("cafe");
}

void GameMain::End()
{
	DeleteGraph(makeScreenHandle_);
}

//更新
void GameMain::Update(const InputState& input)
{
	(this->*updateFunc_)(input);
}

//描画
void GameMain::Draw()
{
	SetDrawScreen(makeScreenHandle_);
	ClearDrawScreen();

	//カメラの初期化
	//SetDrawScreenを行うとカメラの情報がリセットされるために
	camera_->Init();
	camera_->TrackingCameraUpdate(player_->GetStatus().pos);
//	camera_->DebugCamera(player_->GetStatus().pos);

	//broom_->writingScreenUpdate(player_->getPos());
	DrawString(0, 0, "GameMain", 0xffffff);

	//オブジェクトの描画
	objManager_->Draw();

	DrawFormatString(0, 16, 0x448844, "%d", totalDeathNum_);

	//プレイヤーの描画
	player_->Draw();

	//broom_->graphFilterUpdate();
	//broom_->draw();

	SetDrawScreen(DX_SCREEN_BACK);

	//フィルター処理を行うか
	if (isFilterOn_) {
		GraphFilter(makeScreenHandle_, DX_GRAPH_FILTER_GAUSS, 32, 800);
	}
	//makescreenHandleに書き込んだ内容を描画する
	DrawGraph(0, 0, makeScreenHandle_, true);

	//画面全体を真っ黒に塗りつぶす
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeValue_);
	DrawBox(0, 0, Game::screen_width, Game::screen_height, fadeColor_, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

//オブジェクトの生成
void GameMain::ObjectGenerater()
{

	auto& loadData = ExternalFile::GetInstance();

	//ゲームオブジェクトの生成
	objManager_->ObjectGenerator();
	//カメラのインスタンス化
	camera_ = make_shared<Camera>();
	//プレイヤーのインスタンス化
	player_ = make_shared<Player>(player_Filename);
	//プレイヤーの初期化
	player_->Init(loadData.GetSpecifiedInfo("player").front());

}

//TODO：別のフェードインが出来次第消去
void GameMain::FadeInUpdate(const InputState& input)
{
	fadeValue_ = static_cast <int>(255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_)));
	if (--fadeTimer_ == 0) {
		updateFunc_ = &GameMain::NormalUpdate;
		fadeValue_ = 0;
	}
}

//更新
void GameMain::NormalUpdate(const InputState& input)
{
	
	//フィルター処理を行わない用にする
	isFilterOn_ = false;

	//プレイヤーの更新
	player_->Update(input, objManager_);

	//オブジェクトの更新
	objManager_->Update(*player_,input);

	//プレイヤーとその他オブジェクトとの衝突判定
	checkCollisionModel_->CheckCollision(player_,objManager_);

	//カメラの注視点を変更する
	camera_->ChangeOfFocus(input);

	//camera_->fixedPointCamera(player_->getPos());

	//リスナーの位置と方向を設定
	//今回は、プレイヤーではなくカメラの座標にしている
	SoundManager::GetInstance().Set3DSoundListenerInfo(camera_->GetPos(), camera_->GetTarget());

	//いらないやつ
	if (player_->GetStatus().pos.x > 1900) {
		checkPoint_ = { 1900,0,0 };
	}

	//ポーズシーンを開く
	if (input.IsTriggered(InputType::pause)) {
		isFilterOn_ = true;
		manager_.PushFrontScene(std::shared_ptr<SceneBase>(std::make_shared<ScenePause>(manager_)));
	}
}

//TODO：別のフェードインが出来次第消去
void GameMain::FadeOutUpdate(const InputState& input)
{
	fadeValue_ = static_cast <int>(255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_)));
	if (++fadeTimer_ == fadeInterval_) {
		manager_.ChangeScene(std::shared_ptr<SceneBase>(std::make_shared<ScenePause>(manager_)));
		return;
	}
}
