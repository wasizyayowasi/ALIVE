#include "Tutorial.h"
#include "InputState.h"
#include "ExternalFile.h"
#include "FontsManager.h"
#include "DrawFunctions.h"
#include "Util.h"
#include "game.h"

#include <string>
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

	fontPigumo42_ = FontsManager::GetInstance().GetFontHandle("ピグモ 0042");
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
		else if (tutorialInfo.name == "CorpseScaffoldTutorial") {
			drawFunc_ = &Tutorial::CorpseScaffoldDraw;
		}
		tutorialDrawPos_ = tutorialInfo.pos;
	}
	else {
		drawFunc_ = &Tutorial::NoneDraw;
	}
}

void Tutorial::Draw()
{
	auto& input = InputState::GetInstance();

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeValue_);
	(this->*drawFunc_)(input);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void Tutorial::NoneDraw(InputState& input)
{
	fadeTimer_ = (std::max)(fadeTimer_ - 1,0);

	fadeValue_ = (std::max)(static_cast <int>(255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_))), 0);
}

void Tutorial::SwitchTutorialDraw(InputState& input)
{
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

void Tutorial::CranckTutorialDraw(InputState& input)
{
	//ボタンが押されたか
	static bool pressedBottan = false;

	if (input.IsTriggered(InputType::activate)) {
		pressedBottan = true;
	}

	if (pressedBottan) {
		//文字列の描画
		DrawStringToHandle(Game::screen_width / 2 , Game::screen_height - keybord_graph_chip_size * 1.6f - 80.0f, "右回転", 0xffffff, fontPigumo42_);
		DrawStringToHandle(Game::screen_width / 2 , Game::screen_height - keybord_graph_chip_size * 1.6f, "左回転", 0xffffff, fontPigumo42_);

		//キー画像の描画
		input.DrawKeyGraph(InputType::up, UIPos_[UIGraph::KeyBord].first, UIPos_[UIGraph::KeyBord].second - 80.0f, 1.2f);
		input.DrawKeyGraph(InputType::down, UIPos_[UIGraph::KeyBord].first, UIPos_[UIGraph::KeyBord].second, 1.2f);

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

void Tutorial::RunTutorialDraw(InputState& input)
{
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

void Tutorial::JumpTutorialDraw(InputState& input)
{
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

void Tutorial::ElevatorTutorialDraw(InputState& input)
{
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

void Tutorial::CorpseScaffoldDraw(InputState& input)
{
	//ボタンが押されたか
	static bool pressedBottan = false;

	if (input.IsTriggered(InputType::death)) {
		pressedBottan = true;
	}

	if (pressedBottan) {
		std::string str = "死体は足場として使える";
		int width = GetDrawStringWidthToHandle(str.c_str(), str.size(), fontPigumo42_);
		DrawStringToHandle(Game::screen_width / 2 - width / 2, Game::screen_height - keybord_graph_chip_size * 1.6f, str.c_str(), 0xffffff, fontPigumo42_);
		return;
	}

	fadeTimer_ = (std::min)(fadeTimer_ + 1, 255);
	fadeValue_ = (std::min)(static_cast <int>(255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_))), 255);

	if (input.currentInputDevice_) {
		input.DrawKeyGraph(InputType::death, UIPos_[UIGraph::KeyBord].first, UIPos_[UIGraph::KeyBord].second, 1.2f);
	}
	else {
		//画像描画
		input.DrawPadGraph(XboxBotton::Y, UIPos_[UIGraph::XboxBotton].first, UIPos_[UIGraph::XboxBotton].second);
	}

	//キーに対応した文字列の描画(アクションキーの文字列)
	input.DrawName(InputType::death, Game::screen_width / 2, Game::screen_height - keybord_graph_chip_size * 1.6f, 0xffffff, FontsManager::GetInstance().GetFontHandle("ピグモ 0042"), false, false);
}
