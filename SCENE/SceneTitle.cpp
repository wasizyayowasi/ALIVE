#include "SceneTitle.h"
#include "GameMain.h"
#include "ScenePause.h"
#include "SceneManager.h"
#include "SelectChapterScene.h"
#include "SettingSceneForSceneTitle.h"

#include "../object/Camera.h"
#include "../object/LightBulb.h"
#include "../object/ObjectManager.h"

#include "../util/game.h"
#include "../util/Model.h"
#include "../util/InputState.h"
#include "../util/FontsManager.h"
#include "../util/SoundManager.h"
#include "../util/ExternalFile.h"
#include "../util/ModelManager.h"
#include "../util/UIItemManager.h"

#include <algorithm>

namespace {
	const char* const enemy_model_Filename = "data/player/mv1/player.mv1";
}

SceneTitle::SceneTitle(SceneManager& manager): SceneBase(manager)
{
	//短縮化
	auto& file  = ExternalFile::GetInstance();
	auto& model = ModelManager::GetInstance();

	//インスタンス化
	UI_					= std::make_shared<UIItemManager>();
	objManager_			= std::make_shared<ObjectManager>();
	subPlayerModel_		= std::make_shared<Model>(model.GetModelHandle(ObjectType::Player),Material::Other);
	mainPlayerModel_	= std::make_shared<Model>(model.GetModelHandle(ObjectType::Player),Material::Other);
	camera_				= std::make_shared<Camera>(file.GetCameraTargetPos("start"),file.GetCameraTargetPos("startTargetPos"));
	lightBulb_			= std::make_shared<LightBulb>(model.GetModelHandle(ObjectType::LightBulb), file.GetSpecifiedInfo("title", "LightBulb"));

	//メインプレイヤーモデルの配置データをセットする
	auto mainPlayerInfo = file.GetSpecifiedInfo("title", "Player");
	mainPlayerModel_->SetScale(mainPlayerInfo.scale);
	mainPlayerModel_->SetPos(mainPlayerInfo.pos);
	mainPlayerModel_->SetRot(mainPlayerInfo.rot);
	mainPlayerModel_->SetAnimation(static_cast<int>(PlayerAnimType::WakeUp), false, true);

	//サブプレイヤーモデルの配置データをセットする
	auto subPlayerInfo = file.GetSpecifiedInfo("title", "SubPlayer");
	subPlayerModel_->SetScale(subPlayerInfo.scale);
	subPlayerModel_->SetPos(subPlayerInfo.pos);
	subPlayerModel_->SetRot(subPlayerInfo.rot);
	subPlayerModel_->SetAnimation(static_cast<int>(PlayerAnimType::Jump), false, true);

	//UIを表示する座標を取得
	menuDrawPos_["タイトル"]			= file.GetUIPos("titleDrawPos");
	menuDrawPos_["ニューゲーム"]		= file.GetUIPos("startDrawPos");
	menuDrawPos_["再開する"]			= file.GetUIPos("continueDrawPos");
	menuDrawPos_["設定"]				= file.GetUIPos("settingDrawPos");
	menuDrawPos_["終了"]				= file.GetUIPos("endDrawPos");
	menuDrawPos_["モード"]				= file.GetUIPos("windowModeUIPos");
	menuDrawPos_["BGM"]					= file.GetUIPos("BGMUIPos");
	menuDrawPos_["SE"]					= file.GetUIPos("SEUIPos");
	menuDrawPos_["操作設定"]			= file.GetUIPos("advancedSettingUIPos");
	menuDrawPos_["戻る"]				= file.GetUIPos("backUIPos");

	SetUseLighting(true);

	//オブジェクトの生成
	objManager_->OpeningStageObjectGenerator();

	//カメラの初期化
	camera_->Init(VGet(0, 140, 0));

	//カメラのポジションと見る位置の設定
	camera_->SetCameraTargetPosAndView(file.GetCameraTargetPos("start"), file.GetCameraTargetPos("startTargetPos"),{0,1,0});

	//スポットライトに変更する。ライトの設定
	outAngle_ = 90.0f * DX_PI_F / 180.0f;
	inAngle_ = 30.0f * DX_PI_F / 180.0f;
	ChangeLightTypeSpot(lightBulb_->GetFramePos(), lightBulb_->GetFrontVec(), outAngle_, inAngle_, 600.0f, 0.0f, 0.003f, 0.0f);

	//ライトのディフューズカラーの設定
	SetLightDifColor(GetColorF(0.87f, 0.72f, 0.52f, 1.0f));

	//ディレクションライト角度取得
	VECTOR dir = {};
	dir.x = -33 / DX_PI_F * 180.0f;
	dir.y = -33 / DX_PI_F * 180.0f;
	dir.z = -33 / DX_PI_F * 180.0f;

	//ディレクションライトの作成
	lightHandle_.push_back(CreateDirLightHandle(dir));
	lightDir_.push_back(dir);
	SetLightDifColorHandle(lightHandle_[0], GetColorF(0.41f, 0.41f, 0.41f, 1.0f));

	//ディレクションライト角度取得
	dir.x = 33 / DX_PI_F * 180.0f;
	dir.y = -33 / DX_PI_F * 180.0f;
	dir.z = 33 / DX_PI_F * 180.0f;

	//ディレクションライトの作成
	lightHandle_.push_back(CreateDirLightHandle(dir));
	lightDir_.push_back(dir);
	SetLightDifColorHandle(lightHandle_[1], GetColorF(0.41f, 0.41f, 0.41f, 1.0f));

	//カメラ情報の設定
	CameraInfo info = {};

	//設定オブジェクトを見るカメラ
	info.targetPos = file.GetCameraTargetPos("setting");
	info.targetView = file.GetCameraTargetPos("settingTargetPos");
	info.upVec = VGet(0, 1, 0);

	cameraInfo_.push_back(info);

	//ベッドを見るカメラ
	info.targetPos = file.GetCameraTargetPos("start");
	info.targetView = file.GetCameraTargetPos("startTargetPos");
	info.upVec = VGet(0, 1, 0);

	cameraInfo_.push_back(info);

	//本を見るカメラ
	info.targetPos = file.GetCameraTargetPos("continue");
	info.targetView = file.GetCameraTargetPos("continueTargetPos");
	info.upVec = VGet(0, 0, 1);

	cameraInfo_.push_back(info);

	//扉を見るカメラ
	info.targetPos = file.GetCameraTargetPos("end");
	info.targetView = file.GetCameraTargetPos("endTargetPos");
	info.upVec = VGet(0, 1, 0);

	cameraInfo_.push_back(info);

	//タイトル画像の読み込み
	titleHandle_ = LoadGraph("data/graph/title.png");

	//UI文字列の作成
	menuName_.push_back("ニューゲーム");
	menuName_.push_back("再開する");
	menuName_.push_back("設定");
	menuName_.push_back("終了");

	//UI画像の作成
	fontHandle_ = FontsManager::GetInstance().GetFontHandle("ピグモ 0042");
	float y = 120.0f;
	for (auto& menu : menuName_) {
		UI_->AddMenu(static_cast<float>(Game::screen_width / 2), static_cast<float>(Game::screen_height / 2) + y, 320, 100, menu.c_str(), fontHandle_);
		y += 40.0f;
	}
}

SceneTitle::~SceneTitle()
{
	for (auto& light : lightHandle_) {
		DeleteLightHandle(light);
	}

	DeleteGraph(titleHandle_);
}

void SceneTitle::Init()
{
	updateFunc_ = &SceneTitle::FadeInUpdate;

	fadeTimer_ = 0;
	fadeValue_ = 0;
}

void SceneTitle::End()
{
}

void SceneTitle::Update()
{
	//カメラの更新
	camera_->EasingMoveCamera();

	//電球の更新
	lightBulb_->Update();

	//ライトの更新
	SetLightDirection(lightBulb_->GetFrontVec());
	SetLightPosition(lightBulb_->GetFramePos());

	//ライトの方向設定
	SetLightDirectionHandle(lightHandle_[0], lightDir_[0]);
	SetLightDirectionHandle(lightHandle_[1], lightDir_[1]);

	(this->*updateFunc_)();
}

void SceneTitle::Draw()
{
	//短縮化
	auto& input = InputState::GetInstance();
	auto& file = ExternalFile::GetInstance();

	//カメラの初期化
	camera_->Init(VGet(0, 140, 0));

	//オブジェクトの描画
	objManager_->Draw({ 0,0,0 });

	//電球の描画
	lightBulb_->Draw();

	//モデルの描画
	mainPlayerModel_->Draw();
	subPlayerModel_->Draw();

	//UIの描画
	UI_->DrawBillBoard(menuDrawPos_,static_cast<float>(UIfadeValue_),200.0f);

	//キー画像描画
	input.DrawKeyGraph(InputType::left, Game::screen_width / 7 * 5, Game::screen_height - 40, 0.8f);
	input.DrawKeyGraph(InputType::right, Game::screen_width / 8 * 6, Game::screen_height - 40, 0.8f);
	input.DrawKeyGraph(InputType::space, Game::screen_width / 8 * 7, Game::screen_height - 40, 0.8f);

	//文字列の描画
	DrawStringToHandle(Game::screen_width / 10 * 8 - 25, Game::screen_height - 60, "移動", 0xffffff, fontHandle_);
	input.DrawName(InputType::space, Game::screen_width / 20 * 18, Game::screen_height - 60, 0xffffff, fontHandle_, true, true, "/");

	//fadeValue_の値によって透過具合が変化するタイトルの描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, UIfadeValue_);
	DrawBillboard3D(menuDrawPos_["タイトル"], 0.5f, 0.5f, 300.0f, 0.0f, titleHandle_, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//黒いフェード用boxの描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeValue_);
	DrawBox(0, 0, Game::screen_width, Game::screen_height, fadeColor_, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void SceneTitle::FadeInUpdate()
{
	//フェードイン
	float timer = (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_));
	UIfadeValue_ = static_cast <int>(255 * timer);
	if (++fadeTimer_ == fadeInterval_) {
		updateFunc_ = &SceneTitle::UIUpdate;
		UIfadeValue_ = 255;
		return;
	}
}

void SceneTitle::UIUpdate()
{
	//短縮化
	auto& file = ExternalFile::GetInstance();
	auto& input = InputState::GetInstance();

	//1フレーム前の選択番号を記録する
	int oldSelectNum = selectNum_;

	if (camera_->GetMoving()) {
		return;
	}

	//選択
	if (input.IsTriggered(InputType::left)) {
		selectNum_ = (std::max)(selectNum_ - 1, 0);
	}
	if (input.IsTriggered(InputType::right)) {
		selectNum_ = (std::min)(selectNum_ + 1, static_cast<int>(menuName_.size() - 1));
	}

	//現在のフレームと前のフレームで番号が違ったら
	//カメラの設定を行う
	if (selectNum_ != oldSelectNum) {
		camera_->SetCameraTargetPosAndView(cameraInfo_[selectNum_].targetPos, cameraInfo_[selectNum_].targetView, cameraInfo_[selectNum_].upVec);
	}

	//決定
	if (input.IsTriggered(InputType::space)) {
		updateFunc_ = &SceneTitle::UIFadeOutUpdate;
	}
}

void SceneTitle::OpeningSoundUpdate()
{
	//短縮化
	auto& sound = SoundManager::GetInstance();
	int notPlaying = static_cast<int>(SoundPlay::notPlaying);

	//サウンドが鳴り終わったら
	if (sound.CheckSoundFile("alarm") == notPlaying) {
		if (sound.CheckSoundFile("stopAlarm") == notPlaying) {
			sound.PlaySE("stopAlarm");
			updateFunc_ = &SceneTitle::OpeningUpdate;
		}
	}
}

void SceneTitle::OpeningUpdate()
{
	//モデルの描画
	mainPlayerModel_->Update();

	//アニメーションが終了次第
	if (mainPlayerModel_->IsAnimEnd()) {
		updateFunc_ = &SceneTitle::SceneTitleFadeOutUpdate;
	}
}

void SceneTitle::UIFadeOutUpdate()
{
	//UIのフェードアウト
	UIfadeValue_ = static_cast <int>(255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_)));
	if (--fadeTimer_ == 0) {
		UIfadeValue_ = 0;
		SceneChange();
		return;
	}
}

void SceneTitle::SceneTitleFadeOutUpdate()
{
	//フェードアウト
	fadeValue_ = static_cast <int>(255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_)));
	if (++fadeTimer_ == fadeInterval_) {
		manager_.ChangeScene(std::shared_ptr<SceneBase>(std::make_shared<GameMain>(manager_)));
		return;
	}
}

void SceneTitle::SceneChange()
{
	switch (selectNum_) {
	case 0:
		Init();
		manager_.PushFrontScene(std::shared_ptr<SceneBase>(std::make_shared<SettingSceneForSceneTitle>(manager_)));
		break;
	case 1:
		ExternalFile::GetInstance().ClearSaveData();
		updateFunc_ = &SceneTitle::OpeningSoundUpdate;
		SoundManager::GetInstance().PlaySE("alarm");
		break;
	case 2:
		Init();
		manager_.PushFrontScene(std::shared_ptr<SceneBase>(std::make_shared<SelectChapterScene>(manager_)));
		break;
	case 3:
		manager_.SetEndFlag(true);
		break;
	}
}
