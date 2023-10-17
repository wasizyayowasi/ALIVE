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
#include "../util/LoadExternalFile.h"

#include "../util/ObjectManager.h"
#include "../util/ObjectData.h"

using namespace std;

namespace {
	const char* const player_Filename = "data/player/player14.mv1";
}

GameMain::GameMain(SceneManager& manager) : SceneBase(manager),
updateFunc_(&GameMain::fadeInUpdate)
{
}

GameMain::~GameMain()
{
	//ゲームメインが終わるときにプレイ中に死んだ回数と
	//saveDataに記録されている死亡回数を足す
	totalDeathNum_ += player_->getDeathCount();
	LoadExternalFile::GetInstance().SaveDataRewriteInfo(checkPoint_, totalDeathNum_);
}

void GameMain::Init()
{
	makeScreenHandle_ = MakeScreen(Game::screen_width, Game::screen_height, true);

	//オブジェクトを生成
	ObjectGenerater();

	//カメラのインスタンス化
	camera_ = make_shared<Camera>();
	//プレイヤーのインスタンス化
	player_ = make_shared<Player>(player_Filename);
	//broom_ = make_shared<Broom>();
	//depthOfField_ = make_shared<DepthOfField>();

	//仮でライト処理を消している
	SetUseLighting(false);

	//1mの範囲を設定する
	Set3DSoundOneMetre(10.0f);

	//セーブデータの内容を読み取る
	auto data = LoadExternalFile::GetInstance();
	//死亡回数
	totalDeathNum_ = data.GetSaveData().totalDeathNum;
	//
	player_->SetSaveData(data.GetSaveData().checkPoint);

	//プレイヤーの初期化
	player_->Init();

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

	camera_->init();
	camera_->TrackingCameraUpdate(player_->GetStatus().pos);

	//broom_->writingScreenUpdate(player_->getPos());
	DrawString(0, 0, "GameMain", 0xffffff);
	DrawFormatString(0, 16, 0x448844, "%d", totalDeathNum_);

	player_->Draw();
	
	ObjectManager::GetInstance().Draw();

	//broom_->graphFilterUpdate();
	//broom_->draw();

	SetDrawScreen(DX_SCREEN_BACK);

	DrawGraph(0, 0, makeScreenHandle_, true);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeValue_);
	//画面全体を真っ黒に塗りつぶす
	DrawBox(0, 0, Game::screen_width, Game::screen_height, fadeColor_, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

//オブジェクトの生成
void GameMain::ObjectGenerater()
{
	auto& objManager = ObjectManager::GetInstance();

//	objManager.ObjectGenerator(ObjectBaseType::enemyBase, ObjectType::enemy);
//	objManager.ObjectGenerator(ObjectBaseType::ornamentBase, ObjectType::field);
//	objManager.ObjectGenerator(ObjectBaseType::carryBase, ObjectType::carry);
//	objManager.ObjectGenerator(ObjectBaseType::gimmickBase, ObjectType::gimmickSwitch);
//	objManager.ObjectGenerator(ObjectBaseType::gimmickBase, ObjectType::gimmickSwitch);
//	objManager.ObjectGenerator(ObjectBaseType::gimmickBase, ObjectType::gimmickSteelyard);

	for (auto& objInfo : LoadExternalFile::GetInstance().GetLoadObjectInfo()) {
		if (objInfo.first == "field") {
			for (auto& objSecond : objInfo.second) {
				objManager.ObjectGenerator(ObjectBaseType::ornamentBase, ObjectType::field, objSecond);
			}
		}
		else if (objInfo.first == "switch") {
			for (auto& objSecond : objInfo.second) {
				objManager.ObjectGenerator(ObjectBaseType::gimmickBase, ObjectType::gimmickSwitch, objSecond);
			}
		}
		else if (objInfo.first == "steelyard") {
			for (auto& objSecond : objInfo.second) {
				objManager.ObjectGenerator(ObjectBaseType::gimmickBase, ObjectType::gimmickSteelyard, objSecond);
			}
		}
		else if (objInfo.first == "box") {
			for (auto& objSecond : objInfo.second) {
				objManager.ObjectGenerator(ObjectBaseType::carryBase, ObjectType::carry, objSecond);
			}
		}
		else if (objInfo.first == "player") {
			for (auto& objSecond : objInfo.second) {
				objManager.ObjectGenerator(ObjectBaseType::enemyBase, ObjectType::enemy, objSecond);
			}
		}
	}

	/*for (auto& objInfo : LoadExternalFile::GetInstance(true).GetLoadObjectInfo()) {
		if (objInfo.first == "sofa") {
			for (auto& objSecond : objInfo.second) {
				objManager.ObjectGenerator(ObjectBaseType::ornamentBase, ObjectType::tempsofa, objSecond);
			}
			
		}
		else if(objInfo.first == "bed") {
			for (auto& objSecond : objInfo.second) {
				objManager.ObjectGenerator(ObjectBaseType::ornamentBase, ObjectType::tempbed, objSecond);
			}
		}
	}*/
	

}

//TODO：別のフェードインが出来次第消去
void GameMain::fadeInUpdate(const InputState& input)
{
	fadeValue_ = static_cast <int>(255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_)));
	if (--fadeTimer_ == 0) {
		updateFunc_ = &GameMain::normalUpdate;
		fadeValue_ = 0;
	}
}

//更新
void GameMain::normalUpdate(const InputState& input)
{
	auto& objManager = ObjectManager::GetInstance();

	objManager.Update(*player_);

	player_->Update(input);
	camera_->ChangeOfFocus(input);

	//camera_->fixedPointCamera(player_->getPos());

	SoundManager::GetInstance().Set3DSoundListenerInfo(camera_->GetPos(), camera_->GetTarget());

	if (player_->GetStatus().pos.x > 1900) {
		checkPoint_ = { 1900,0,0 };
	}

	if (input.IsTriggered(InputType::pause)) {
		//GraphFilter(makeScreenHandle_, DX_GRAPH_FILTER_GAUSS, 32, 20000);
		manager_.PushScene(std::shared_ptr<SceneBase>(std::make_shared<ScenePause>(manager_)));
	}
}

//TODO：別のフェードインが出来次第消去
void GameMain::fadeOutUpdate(const InputState& input)
{
	fadeValue_ = static_cast <int>(255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_)));
	if (++fadeTimer_ == fadeInterval_) {
		manager_.ChangeScene(std::shared_ptr<SceneBase>(std::make_shared<ScenePause>(manager_)));
		return;
	}
}
