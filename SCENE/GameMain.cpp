#include "GameMain.h"
#include "SceneManager.h"
#include "GameEnd.h"

#include "../gimmick/GimmickBase.h"
#include "../gimmick/Switch.h"
#include "../gimmick/Steelyard.h"

#include "../object/Player.h"
#include "../object/Camera.h"
#include "../object/CharacterBase.h"
#include "../object/ShotManager.h"

#include "../staging/Broom.h"
#include "../staging/DepthOfField.h"

#include "ScenePause.h" 

#include "../util/game.h"
#include "../util/SoundManager.h"
#include "../util/InputState.h"
#include "../util/model.h"
#include "../util/ExternalFile.h"
#include "../util/CheckCollisionModel.h"
#include "../util/Tutorial.h"

#include "../util/ObjectManager.h"
#include "../util/ObjectData.h"

using namespace std;

namespace {
	const char* const enemy_Filename = "data/player/player16.mv1";
}

GameMain::GameMain(SceneManager& manager) : SceneBase(manager),
updateFunc_(&GameMain::NormalUpdate)
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
	shotManager_ = std::make_shared<ShotManager>();
	tutorial_ = std::make_shared<Tutorial>();

	//オブジェクトを生成
	ObjectGenerater();

	//broom_ = make_shared<Broom>();
	//depthOfField_ = make_shared<DepthOfField>();

	//仮でライト処理を消している
	SetUseLighting(false);

	//1mの範囲を設定する
	Set3DSoundOneMetre(10.0f);

	//セーブデータの内容を読み取る
	//死亡回数
	totalDeathNum_ = ExternalFile::GetInstance().GetSaveData().totalDeathNum;

	skyHandle_ = MV1LoadModel("data/model/skyDorm/SkyDorm.mv1");
	float scale = 30.0f;
	MV1SetScale(skyHandle_, VGet(scale, scale, scale));
	MV1SetPosition(skyHandle_, VGet(0, 200, 0));

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
	camera_->Update(player_->GetStatus().pos, player_->GetStatus().height);

//	broom_->writingScreenUpdate(player_->GetStatus().pos);

	//オブジェクトの描画
	objManager_->Draw(player_->GetStatus().pos);

	MV1SetPosition(skyHandle_, player_->GetStatus().pos);
	MV1DrawModel(skyHandle_);

	//プレイヤーの描画
	player_->Draw();

	shotManager_->Draw();

	tutorial_->Draw();

//	broom_->graphFilterUpdate();
//	broom_->draw();

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

	//プレイヤーのインスタンス化
	player_ = make_shared<Player>(enemy_Filename);
	//プレイヤーの初期化
	player_->Init(loadData.GetSpecifiedInfo("Player").front());
	//ゲームオブジェクトの生成
	objManager_->ObjectGenerator();
	//カメラのインスタンス化
	camera_ = make_shared<Camera>();

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
	objManager_->Update(*player_,input,shotManager_);

	//プレイヤーとその他オブジェクトとの衝突判定
	checkCollisionModel_->CheckCollision(player_,objManager_);

	//カメラの注視点を変更する
	camera_->ChangeOfFocus(input);

	shotManager_->Update();
	shotManager_->Hit(*player_);

	tutorial_->Update(player_->GetStatus().pos);

	//リスナーの位置と方向を設定
	//今回は、プレイヤーではなくカメラの座標にしている
	SoundManager::GetInstance().Set3DSoundListenerInfo(camera_->GetPos(), camera_->GetTarget());

	//ポーズシーンを開く
	if (input.IsTriggered(InputType::pause)) {
		isFilterOn_ = true;
		manager_.PushFrontScene(std::shared_ptr<SceneBase>(std::make_shared<ScenePause>(manager_)));
	}

//	auto info = ExternalFile::GetInstance().GetSpecifiedInfo("GOAL");
//
//	auto result = MV1CollCheck_Capsule(player_->GetModelPointer()->GetModelHandle(), player_->GetModelPointer()->GetColFrameIndex(), VGet(info.front().pos.x, info.front().pos.y - info.front().scale.y, info.front().pos.z), VGet(info.front().pos.x, info.front().pos.y + info.front().scale.y, info.front().pos.z), 600);
//
//	if (result.HitNum > 0) {
//		updateFunc_ = &GameMain::FadeOutUpdate;
//	}
//
//	MV1CollResultPolyDimTerminate(result);

}

//TODO：別のフェードインが出来次第消去
void GameMain::FadeOutUpdate(const InputState& input)
{
	fadeValue_ = static_cast <int>(255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_)));
	if (++fadeTimer_ == fadeInterval_) {
		manager_.ChangeScene(std::shared_ptr<SceneBase>(std::make_shared<GameEnd>(manager_)));
		return;
	}
}
