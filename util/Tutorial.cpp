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

	{
		keyNum_[0] = Key::ESC;
		keyNum_[1] = Key::Key_1;
		keyNum_[2] = Key::Key_2;
		keyNum_[3] = Key::Key_3;
		keyNum_[4] = Key::Key_4;
		keyNum_[5] = Key::Key_5;
		keyNum_[6] = Key::Key_6;
		keyNum_[7] = Key::Key_7;
		keyNum_[8] = Key::Key_8;
		keyNum_[9] = Key::Key_9;
		keyNum_[10] = Key::Key_0;
		keyNum_[11] = Key::Key_equal;

		keyNum_[14] = Key::Back;
		keyNum_[15] = Key::Tab;
		keyNum_[16] = Key::Q;
		keyNum_[17] = Key::W;
		keyNum_[18] = Key::E;
		keyNum_[19] = Key::R;
		keyNum_[20] = Key::T;
		keyNum_[21] = Key::Y;
		keyNum_[22] = Key::U;
		keyNum_[23] = Key::I;
		keyNum_[24] = Key::O;
		keyNum_[25] = Key::P;
		keyNum_[26] = Key::Left_parenthesis_key;
		keyNum_[27] = Key::Right_parenthesis_key;
		keyNum_[28] = Key::Big_enter;
		keyNum_[29] = Key::L_ctrl;
		keyNum_[30] = Key::A;
		keyNum_[31] = Key::S;
		keyNum_[32] = Key::D;
		keyNum_[33] = Key::F;
		keyNum_[34] = Key::G;
		keyNum_[35] = Key::H;
		keyNum_[36] = Key::J;
		keyNum_[37] = Key::K;
		keyNum_[38] = Key::L;
		keyNum_[39] = Key::Semicolon;

		keyNum_[42] = Key::L_shift;
		keyNum_[43] = Key::Backslash;
		keyNum_[44] = Key::Z;
		keyNum_[45] = Key::X;
		keyNum_[46] = Key::C;
		keyNum_[47] = Key::V;
		keyNum_[48] = Key::B;
		keyNum_[49] = Key::N;
		keyNum_[50] = Key::M;
		keyNum_[51] = Key::Comma;
		keyNum_[52] = Key::Period;
		keyNum_[53] = Key::Slash;
		keyNum_[54] = Key::R_shift;
		keyNum_[55] = Key::Asterisk;
		keyNum_[56] = Key::L_alt;
		keyNum_[57] = Key::Space;
		keyNum_[58] = Key::CaspLock;
		keyNum_[59] = Key::F1;
		keyNum_[60] = Key::F2;
		keyNum_[61] = Key::F3;
		keyNum_[62] = Key::F4;
		keyNum_[63] = Key::F5;
		keyNum_[64] = Key::F6;
		keyNum_[65] = Key::F7;
		keyNum_[66] = Key::F8;
		keyNum_[67] = Key::F9;
		keyNum_[68] = Key::F10;
		keyNum_[69] = Key::NumLock;
		keyNum_[70] = Key::ScrollLock;
		keyNum_[71] = Key::Key10_7;
		keyNum_[72] = Key::Key10_8;
		keyNum_[73] = Key::Key10_9;
		keyNum_[74] = Key::Key10_minus;
		keyNum_[75] = Key::Key10_4;
		keyNum_[76] = Key::Key10_5;
		keyNum_[77] = Key::Key10_6;
		keyNum_[78] = Key::Key10_plus;
		keyNum_[79] = Key::Key10_1;
		keyNum_[80] = Key::Key10_2;
		keyNum_[81] = Key::Key10_3;
		keyNum_[82] = Key::Key10_0;
		keyNum_[83] = Key::Key10_period;

		keyNum_[87] = Key::F11;
		keyNum_[88] = Key::F12;
		keyNum_[112] = Key::KANA;
		keyNum_[121] = Key::Conversion;
		keyNum_[123] = Key::NoConversion;
		keyNum_[125] = Key::YEN_mark;
		keyNum_[144] = Key::Caret;
		keyNum_[145] = Key::At_mark;
		keyNum_[146] = Key::Colon;
		keyNum_[148] = Key::ChineseCharacters;
		keyNum_[156] = Key::Key10_enter;
		keyNum_[157] = Key::R_ctrl;
		keyNum_[181] = Key::Key10_slash;
		keyNum_[183] = Key::Print;
		keyNum_[184] = Key::Ralt;
		keyNum_[197] = Key::Pause;
		keyNum_[199] = Key::Home;
		keyNum_[200] = Key::Up_Arrow;
		keyNum_[201] = Key::Page_up;
		keyNum_[203] = Key::Left_Arrow;
		keyNum_[205] = Key::Right_Arrow;
		keyNum_[207] = Key::END;
		keyNum_[208] = Key::Down_Arrow;
		keyNum_[209] = Key::Page_Down;
		keyNum_[210] = Key::Insert;
		keyNum_[211] = Key::Delete;
		keyNum_[219] = Key::L_win;
		keyNum_[220] = Key::Rwin;
		keyNum_[221] = Key::App;
	}
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
	num = static_cast<int>(keyNum_[num]);

	//画像の任意のキーが何列目の何行目にあるか取得する
	int GraphX = num % 9;
	int GraphY = num / 9;

	//キー画像の描画
	Graph::DrawRectRotaGraph(UIPos_[UIGraph::KeyBord].first, UIPos_[UIGraph::KeyBord].second, GraphX * keybord_graph_chip_size, GraphY * keybord_graph_chip_size, keybord_graph_chip_size, keybord_graph_chip_size, 1.2f, 0.0f, UIHandle_[UIGraph::KeyBord], true, false);
}

void Tutorial::NoneDraw()
{
	fadeTimer_--;

	fadeValue_ = (std::max)(static_cast <int>(255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_))), 0);
}

void Tutorial::SwitchTutorialDraw()
{
	//短縮化
	auto& input = InputState::GetInstance();

	fadeTimer_++;
	fadeValue_ = (std::min)(static_cast <int>(255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_))), 255);

	if (input.currentInputDevice_) {
		KeyGraphDraw(static_cast<int>(InputType::death));
	}
	else {
		//画像描画
		Graph::DrawRectRotaGraph(UIPos_[UIGraph::XboxBotton].first, UIPos_[UIGraph::XboxBotton].second, controller_graph_chip_size * static_cast<int>(XboxBotton::Y), 0, controller_graph_chip_size, controller_graph_chip_size, 1.0f, 0.0f, UIHandle_[UIGraph::XboxBotton], true, false);
	}

	//キーに対応した文字列の描画(アクションキーの文字列)
	DrawStringFToHandle(Game::screen_width / 2, Game::screen_height - keybord_graph_chip_size * 1.6f, input.inputNameTable_[InputType::activate].c_str(), 0xffffff, FontsManager::GetInstance().GetFontHandle("ピグモ 0042"));
}

void Tutorial::CranckTutorialDraw()
{
	//短縮化
	auto& input = InputState::GetInstance();

	fadeTimer_++;
	fadeValue_ = (std::min)(static_cast <int>(255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_))), 255);

	if (input.currentInputDevice_) {
		KeyGraphDraw(static_cast<int>(InputType::activate));
	}
	else {
		//画像描画
		Graph::DrawRectRotaGraph(UIPos_[UIGraph::XboxBotton].first, UIPos_[UIGraph::XboxBotton].second, controller_graph_chip_size * static_cast<int>(XboxBotton::B), 0, controller_graph_chip_size, controller_graph_chip_size, 1.0f, 0.0f, UIHandle_[UIGraph::XboxBotton], true, false);
	}

	//キーに対応した文字列の描画(アクションキーの文字列)
	DrawStringFToHandle(Game::screen_width / 2, Game::screen_height - keybord_graph_chip_size*1.6f, input.inputNameTable_[InputType::activate].c_str(), 0xffffff, FontsManager::GetInstance().GetFontHandle("ピグモ 0042"));
}

void Tutorial::RunTutorialDraw()
{
	//短縮化
	auto& input = InputState::GetInstance();

	fadeTimer_++;
	fadeValue_ = (std::min)(static_cast <int>(255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_))), 255);

	if (input.currentInputDevice_) {
		KeyGraphDraw(static_cast<int>(InputType::dush));
	}
	else {
		//画像描画
		Graph::DrawRectRotaGraph(UIPos_[UIGraph::XboxBotton].first, UIPos_[UIGraph::XboxBotton].second, controller_graph_chip_size * static_cast<int>(XboxBotton::X), 0, controller_graph_chip_size, controller_graph_chip_size, 1.0f, 0.0f, UIHandle_[UIGraph::XboxBotton], true, false);
	}

	//キーに対応した文字列の描画(アクションキーの文字列)
	DrawStringFToHandle(Game::screen_width / 2, Game::screen_height - keybord_graph_chip_size * 1.6f, input.inputNameTable_[InputType::dush].c_str(), 0xffffff, FontsManager::GetInstance().GetFontHandle("ピグモ 0042"));
}

void Tutorial::JumpTutorialDraw()
{
	//短縮化
	auto& input = InputState::GetInstance();

	fadeTimer_++;
	fadeValue_ = (std::min)(static_cast <int>(255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_))), 255);

	if (input.currentInputDevice_) {
		KeyGraphDraw(static_cast<int>(InputType::space));
	}
	else {
		//画像描画
		Graph::DrawRectRotaGraph(UIPos_[UIGraph::XboxBotton].first, UIPos_[UIGraph::XboxBotton].second, controller_graph_chip_size * static_cast<int>(XboxBotton::Y), 0, controller_graph_chip_size, controller_graph_chip_size, 1.0f, 0.0f, UIHandle_[UIGraph::XboxBotton], true, false);
	}

	//決定/ジャンプという文字列の「決定/」の部分を取り除いた文字列を取得する
	std::string name = StrUtil::GetStringAfterSign(input.inputNameTable_[InputType::space], "/");

	//キーに対応した文字列の描画(アクションキーの文字列)
	DrawStringFToHandle(Game::screen_width / 2, Game::screen_height - keybord_graph_chip_size * 1.6f, name.c_str(), 0xffffff, FontsManager::GetInstance().GetFontHandle("ピグモ 0042"));
}

void Tutorial::ElevatorTutorialDraw()
{
	//短縮化
	auto& input = InputState::GetInstance();

	fadeTimer_++;
	fadeValue_ = (std::min)(static_cast <int>(255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_))), 255);

	if (input.currentInputDevice_) {
		KeyGraphDraw(static_cast<int>(InputType::activate));
	}
	else {
		//画像描画
		Graph::DrawRectRotaGraph(UIPos_[UIGraph::XboxBotton].first, UIPos_[UIGraph::XboxBotton].second, controller_graph_chip_size * static_cast<int>(XboxBotton::B), 0, controller_graph_chip_size, controller_graph_chip_size, 1.0f, 0.0f, UIHandle_[UIGraph::XboxBotton], true, false);
	}

	//キーに対応した文字列の描画(アクションキーの文字列)
	DrawStringFToHandle(Game::screen_width / 2, Game::screen_height - keybord_graph_chip_size * 1.6f, input.inputNameTable_[InputType::activate].c_str(), 0xffffff, FontsManager::GetInstance().GetFontHandle("ピグモ 0042"));
}