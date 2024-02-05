#include "Tutorial.h"
#include "InputState.h"
#include "ExternalFile.h"
#include "FontsManager.h"
#include "DrawFunctions.h"
#include "Util.h"
#include "game.h"

#include <algorithm>

namespace {
	//xboxのボタン画像のファイルパス
	const char* const xbox_Botton_filepath = "data/graph/ControllerBotton.png";
	const char* const key_filepath = "data/graph/key2.png";

	//キーボード画像のチップサイズ
	constexpr int keybord_graph_chip_size = 59;
	//パッド画像のチップサイズ
	constexpr int controller_graph_chip_size = 71;
}

Tutorial::Tutorial():drawFunc_(&Tutorial::NoneDraw)
{
	//UI画像の読み込み
	UIHandle_[UIGraph::XboxBotton] = Graph::LoadGraph(xbox_Botton_filepath);
	UIHandle_[UIGraph::KeyBord] = Graph::LoadGraph(key_filepath);

	//キーボードの画像を描画する位置
	UIPos_[UIGraph::KeyBord].first = Game::screen_width / 2 - keybord_graph_chip_size;
	UIPos_[UIGraph::KeyBord].second = Game::screen_height - keybord_graph_chip_size * 1.3f;

	//padのボタンを描画する位置
	UIPos_[UIGraph::XboxBotton].first = Game::screen_width / 2 - controller_graph_chip_size;
	UIPos_[UIGraph::XboxBotton].second = Game::screen_height - controller_graph_chip_size;

	fotnPigumo42_ = FontsManager::GetInstance().GetFontHandle("ピグモ 0042");
}

Tutorial::~Tutorial()
{
	//画像の削除
	for (auto& handle : UIHandle_) {
		DeleteGraph(handle.second);
	}
}

void Tutorial::Update(VECTOR pos)
{
	float distanceSize = 0.0f;

	auto tutorialInfo = ExternalFile::GetInstance().GetTutorialObjInfo(pos);

	distanceSize = MathUtil::GetSizeOfDistanceTwoPoints(tutorialInfo.pos, pos);

	float range = VSize(tutorialInfo.scale) / 3;

	if (range > distanceSize) {
		if (tutorialInfo.name == "CrankTutorial") {
			drawFunc_ = &Tutorial::CranckTutorialDraw;
		}
		else if (tutorialInfo.name == "SwitchTutorial") {
			drawFunc_ = &Tutorial::SwitchTutorialDraw;
		}
		else if (tutorialInfo.name == "RunTutorial") {
			drawFunc_ = &Tutorial::RunTutorialDraw;
		}
		else if (tutorialInfo.name == "JumpTutorial") {
			drawFunc_ = &Tutorial::JumpTutorialDraw;
		}
		else if (tutorialInfo.name == "ElevatorTutorial") {
			drawFunc_ = &Tutorial::ElevatorTutorialDraw;
		}
		tutorialDrawPos_ = tutorialInfo.pos;
	}
	else {
		drawFunc_ = &Tutorial::NoneDraw;
	}
}

void Tutorial::Draw()
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeValue_);
	(this->*drawFunc_)();
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void Tutorial::KeyGraphDraw(int keyNum)
{
	//短縮化
	auto& input = InputState::GetInstance();

	//番号
	int num = 0;

	//入力装置の番号を取得する
	num = input.GetInputNum(keyNum, InputCategory::keybd);

	//入力装置の番号を自作の画像の番号に変換する
	//num = static_cast<int>(keyNum_[num]);

	//画像の任意のキーが何列目の何行目にあるか取得する
	int GraphX = num % 9;
	int GraphY = num / 9;

	//キー画像の描画
	Graph::DrawRectRotaGraph(UIPos_[UIGraph::KeyBord].first, UIPos_[UIGraph::KeyBord].second, GraphX * keybord_graph_chip_size, GraphY * keybord_graph_chip_size, keybord_graph_chip_size, keybord_graph_chip_size, 1.2f, 0.0f, UIHandle_[UIGraph::KeyBord], true, false);
}

void Tutorial::NoneDraw()
{
	fadeTimer_ = (std::max)(fadeTimer_ - 1,0);

	fadeValue_ = (std::max)(static_cast <int>(255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_))), 0);
}

void Tutorial::SwitchTutorialDraw()
{
	//短縮化
	auto& input = InputState::GetInstance();

	static InputType type = InputType::death;
	static XboxBotton bottanType = XboxBotton::Y;

	fadeTimer_ = (std::min)(fadeTimer_ + 1,255);
	fadeValue_ = (std::min)(static_cast <int>(255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_))), 255);

	if (input.currentInputDevice_) {
		input.DrawKeyGraph(type, UIPos_[UIGraph::KeyBord].first, UIPos_[UIGraph::KeyBord].second,1.2f);
	}
	else {
		//画像描画
		input.DrawPadGraph(bottanType, UIPos_[UIGraph::XboxBotton].first, UIPos_[UIGraph::XboxBotton].second);
	}

	if (input.IsTriggered(InputType::death)) {
		type = InputType::activate;
		bottanType = XboxBotton::B;
	}

	//キーに対応した文字列の描画(アクションキーの文字列)
	input.DrawName(type, Game::screen_width / 2, Game::screen_height - keybord_graph_chip_size * 1.6f, 0xffffff, FontsManager::GetInstance().GetFontHandle("ピグモ 0042"),false,false);
}

void Tutorial::CranckTutorialDraw()
{
	//短縮化
	auto& input = InputState::GetInstance();

	//ボタンが押されたか
	static bool pressedBottan = false;

	if (input.IsTriggered(InputType::activate)) {
		pressedBottan = true;
	}

	if (pressedBottan) {
		return;
	}

	fadeTimer_ = (std::min)(fadeTimer_ + 1,255);
	fadeValue_ = (std::min)(static_cast <int>(255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_))), 255);

	if (input.currentInputDevice_) {
		input.DrawKeyGraph(InputType::activate, UIPos_[UIGraph::KeyBord].first, UIPos_[UIGraph::KeyBord].second,1.2f);
	}
	else {
		//画像描画
		input.DrawPadGraph(XboxBotton::B, UIPos_[UIGraph::XboxBotton].first, UIPos_[UIGraph::XboxBotton].second);
	}

	//キーに対応した文字列の描画(アクションキーの文字列)
	input.DrawName(InputType::activate, Game::screen_width / 2, Game::screen_height - keybord_graph_chip_size * 1.6f, 0xffffff, FontsManager::GetInstance().GetFontHandle("ピグモ 0042"),false,false);
}

void Tutorial::RunTutorialDraw()
{
	//短縮化
	auto& input = InputState::GetInstance();

	//ボタンが押されたか
	static bool pressedBottan = false;

	if (input.IsTriggered(InputType::dush)) {
		pressedBottan = true;
	}

	if (pressedBottan) {
		return;
	}

	fadeTimer_ = (std::min)(fadeTimer_ + 1,255);
	fadeValue_ = (std::min)(static_cast <int>(255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_))), 255);

	if (input.currentInputDevice_) {
		input.DrawKeyGraph(InputType::dush, UIPos_[UIGraph::KeyBord].first, UIPos_[UIGraph::KeyBord].second, 1.2f);
	}
	else {
		//画像描画
		input.DrawPadGraph(XboxBotton::X, UIPos_[UIGraph::XboxBotton].first, UIPos_[UIGraph::XboxBotton].second);
	}

	//キーに対応した文字列の描画(アクションキーの文字列)
	input.DrawName(InputType::dush, Game::screen_width / 2, Game::screen_height - keybord_graph_chip_size * 1.6f, 0xffffff, FontsManager::GetInstance().GetFontHandle("ピグモ 0042"),false,false);
}

void Tutorial::JumpTutorialDraw()
{
	//短縮化
	auto& input = InputState::GetInstance();

	//ボタンが押されたか
	static bool pressedBottan = false;

	if (input.IsTriggered(InputType::space)) {
		pressedBottan = true;
	}

	if (pressedBottan) {
		return;
	}

	fadeTimer_ = (std::min)(fadeTimer_ + 1,255);
	fadeValue_ = (std::min)(static_cast <int>(255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_))), 255);

	if (input.currentInputDevice_) {
		input.DrawKeyGraph(InputType::space, UIPos_[UIGraph::KeyBord].first, UIPos_[UIGraph::KeyBord].second, 1.2f);
	}
	else {
		//画像描画
		input.DrawPadGraph(XboxBotton::A, UIPos_[UIGraph::XboxBotton].first, UIPos_[UIGraph::XboxBotton].second);
	}

	//キーに対応した文字列の描画(アクションキーの文字列)
	input.DrawName(InputType::space, Game::screen_width / 2, Game::screen_height - keybord_graph_chip_size * 1.6f, 0xffffff, FontsManager::GetInstance().GetFontHandle("ピグモ 0042"),true,false,"/");
}

void Tutorial::ElevatorTutorialDraw()
{
	//短縮化
	auto& input = InputState::GetInstance();

	//ボタンが押されたか
	static bool pressedBottan = false;

	if (input.IsTriggered(InputType::activate)) {
		pressedBottan = true;
	}

	if (pressedBottan) {
		return;
	}

	fadeTimer_ = (std::min)(fadeTimer_ + 1,255);
	fadeValue_ = (std::min)(static_cast <int>(255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_))), 255);

	if (input.currentInputDevice_) {
		input.DrawKeyGraph(InputType::activate, UIPos_[UIGraph::KeyBord].first, UIPos_[UIGraph::KeyBord].second, 1.2f);
	}
	else {
		//画像描画
		input.DrawPadGraph(XboxBotton::B, UIPos_[UIGraph::XboxBotton].first, UIPos_[UIGraph::XboxBotton].second);
	}

	//キーに対応した文字列の描画(アクションキーの文字列)
	input.DrawName(InputType::activate, Game::screen_width / 2, Game::screen_height - keybord_graph_chip_size * 1.6f, 0xffffff, FontsManager::GetInstance().GetFontHandle("ピグモ 0042"),false,false);
}