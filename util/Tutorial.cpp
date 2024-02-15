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
	//�L�[�{�[�h�摜�̃`�b�v�T�C�Y
	constexpr int keybord_graph_chip_size = 59;
	//�p�b�h�摜�̃`�b�v�T�C�Y
	constexpr int controller_graph_chip_size = 71;
}

Tutorial::Tutorial():drawFunc_(&Tutorial::NoneDraw)
{
	//�Z�k��
	auto& input = InputState::GetInstance();

	//�L�[�{�[�h�̉摜��`�悷��ʒu
	UIPos_[UIGraph::KeyBord].first = Game::screen_width / 2 - keybord_graph_chip_size;
	UIPos_[UIGraph::KeyBord].second = Game::screen_height - keybord_graph_chip_size * 1.3f;

	//pad�̃{�^����`�悷��ʒu
	UIPos_[UIGraph::XboxBotton].first = Game::screen_width / 2 - controller_graph_chip_size;
	UIPos_[UIGraph::XboxBotton].second = Game::screen_height - controller_graph_chip_size;

	//�z��̏���
	pushBottan_[static_cast<int>(InputType::Activate)]	= false;
	pushBottan_[static_cast<int>(InputType::Space)]		= false;
	pushBottan_[static_cast<int>(InputType::Death)]		= false;
	pushBottan_[static_cast<int>(InputType::Dush)]		= false;

	nextDisplayBottanType_ = static_cast<int>(XboxBotton::Y);
	nextDisplayKeyType_ = static_cast<int>(InputType::Death);

	fontPigumo42_ = FontsManager::GetInstance().GetFontHandle("�s�O�� 0042");
}

Tutorial::~Tutorial()
{
}

void Tutorial::Update(VECTOR pos)
{
	float distanceSize = 0.0f;

	auto tutorialInfo = ExternalFile::GetInstance().GetTutorialObjInfo(pos);

	distanceSize = MathUtil::GetSizeOfDistanceTwoPoints(tutorialInfo.pos, pos);

	float range = VSize(tutorialInfo.scale) / 2;

	tutorialInfo.name = StrUtil::GetStringBeforeSign(tutorialInfo.name, ".");

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

	(this->*drawFunc_)(input);
}

void Tutorial::NoneDraw(InputState& input)
{
	for (auto& bottan : pushBottan_) {
		bottan.second = false;
	}
}

void Tutorial::SwitchTutorialDraw(InputState& input)
{

	static std::string str = "���̂�����";

	if (input.currentInputDevice_) {
		input.DrawKeyGraph(nextDisplayKeyType_, UIPos_[UIGraph::KeyBord].first, UIPos_[UIGraph::KeyBord].second,1.2f);
	}
	else {
		//�摜�`��
		input.DrawPadGraph(nextDisplayBottanType_, UIPos_[UIGraph::XboxBotton].first, UIPos_[UIGraph::XboxBotton].second,1.0f);
	}

	if (input.IsTriggered(InputType::Death)) {
		nextDisplayKeyType_ = static_cast<int>(InputType::Activate);
		nextDisplayBottanType_ = static_cast<int>(XboxBotton::B);
		str = "���̂�����";
	}

	if (input.IsTriggered(InputType::Activate)) {
		str = "���̂�u��";
	}

	//�L�[�ɑΉ�����������̕`��(�A�N�V�����L�[�̕�����)
	if (nextDisplayKeyType_ == static_cast<int>(InputType::Death) || nextDisplayBottanType_ == static_cast<int>(XboxBotton::Y)) {
		input.DrawName(nextDisplayKeyType_, static_cast<float>(Game::screen_width / 2), Game::screen_height - keybord_graph_chip_size * 1.6f, 0xffffff, fontPigumo42_, false, false);
	}
	else {
		DrawStringToHandle(Game::screen_width / 2, static_cast<int>(Game::screen_height - keybord_graph_chip_size * 1.6f), str.c_str(), 0xffffff, fontPigumo42_);
	}
}

void Tutorial::CranckTutorialDraw(InputState& input)
{
	if (input.IsTriggered(InputType::Activate)) {
		pushBottan_[static_cast<int>(InputType::Death)] = true;
	}

	if (pushBottan_[static_cast<int>(InputType::Death)]) {
		//������̕`��
		DrawStringToHandle(Game::screen_width / 2 , Game::screen_height - static_cast<int>(keybord_graph_chip_size * 1.6f) - 80, "����]", 0xffffff, fontPigumo42_);
		DrawStringToHandle(Game::screen_width / 2 , Game::screen_height - static_cast<int>(keybord_graph_chip_size * 1.6f), "�E��]", 0xffffff, fontPigumo42_);

		if (input.currentInputDevice_) {
			//�L�[�摜�̕`��
			input.DrawKeyGraph(static_cast<int>(InputType::Up), UIPos_[UIGraph::KeyBord].first, UIPos_[UIGraph::KeyBord].second - 80.0f, 1.2f);
			input.DrawKeyGraph(static_cast<int>(InputType::Down), UIPos_[UIGraph::KeyBord].first, UIPos_[UIGraph::KeyBord].second, 1.2f);
		}
		else {
			input.DrawPadGraph(static_cast<int>(XboxBotton::Up),UIPos_[UIGraph::XboxBotton].first, UIPos_[UIGraph::XboxBotton].second - 80.0f, 1.0f);
			input.DrawPadGraph(static_cast<int>(XboxBotton::Down),UIPos_[UIGraph::XboxBotton].first, UIPos_[UIGraph::XboxBotton].second, 1.0f);
		}

		if (input.IsTriggered(InputType::Up) || input.IsTriggered(InputType::Down)) {
			pushBottan_[static_cast<int>(InputType::Activate)] = true;
			pushBottan_[static_cast<int>(InputType::Death)] = false;
		}
		
		return;
	}

	if (input.currentInputDevice_) {
		input.DrawKeyGraph(static_cast<int>(InputType::Activate), UIPos_[UIGraph::KeyBord].first, UIPos_[UIGraph::KeyBord].second,1.2f);
	}
	else {
		//�摜�`��
		input.DrawPadGraph(static_cast<int>(XboxBotton::B), UIPos_[UIGraph::XboxBotton].first, UIPos_[UIGraph::XboxBotton].second, 1.0f);
	}

	if (pushBottan_[static_cast<int>(InputType::Activate)]) {
		//�L�[�ɑΉ�����������̕`��(�A�N�V�����L�[�̕�����)
		DrawStringToHandle(Game::screen_width / 2, Game::screen_height - static_cast<int>(keybord_graph_chip_size * 1.6f), "������", 0xffffff, fontPigumo42_);
	}
	else {
		//�L�[�ɑΉ�����������̕`��(�A�N�V�����L�[�̕�����)
		DrawStringToHandle(Game::screen_width / 2, Game::screen_height - static_cast<int>(keybord_graph_chip_size * 1.6f), "�N�����N����", 0xffffff, fontPigumo42_);
	}
}

void Tutorial::RunTutorialDraw(InputState& input)
{
	if (input.IsTriggered(InputType::Dush)) {
		pushBottan_[static_cast<int>(InputType::Dush)] = true;
	}

	if (pushBottan_[static_cast<int>(InputType::Dush)]) {
		return;
	}

	if (input.currentInputDevice_) {
		input.DrawKeyGraph(static_cast<int>(InputType::Dush), UIPos_[UIGraph::KeyBord].first, UIPos_[UIGraph::KeyBord].second, 1.2f);
	}
	else {
		//�摜�`��
		input.DrawPadGraph(static_cast<int>(XboxBotton::X), UIPos_[UIGraph::XboxBotton].first, UIPos_[UIGraph::XboxBotton].second, 1.0f);
	}

	//�L�[�ɑΉ�����������̕`��(�A�N�V�����L�[�̕�����)
	input.DrawName(static_cast<int>(InputType::Dush), static_cast<float>(Game::screen_width / 2), Game::screen_height - keybord_graph_chip_size * 1.6f, 0xffffff, fontPigumo42_,false,false);
}

void Tutorial::JumpTutorialDraw(InputState& input)
{

	if (input.IsTriggered(InputType::Space)) {
		pushBottan_[static_cast<int>(InputType::Space)] = true;
	}

	if (pushBottan_[static_cast<int>(InputType::Space)]) {
		return;
	}

	if (input.currentInputDevice_) {
		input.DrawKeyGraph(static_cast<int>(InputType::Space), UIPos_[UIGraph::KeyBord].first, UIPos_[UIGraph::KeyBord].second, 1.2f);
	}
	else {
		//�摜�`��
		input.DrawPadGraph(static_cast<int>(XboxBotton::A), UIPos_[UIGraph::XboxBotton].first, UIPos_[UIGraph::XboxBotton].second, 1.0f);
	}

	//�L�[�ɑΉ�����������̕`��(�A�N�V�����L�[�̕�����)
	input.DrawName(static_cast<int>(InputType::Space), static_cast<float>(Game::screen_width / 2), Game::screen_height - keybord_graph_chip_size * 1.6f, 0xffffff, fontPigumo42_,true,false,"/");
}

void Tutorial::ElevatorTutorialDraw(InputState& input)
{
	if (input.IsTriggered(InputType::Activate)) {
		pushBottan_[static_cast<int>(InputType::Activate)] = true;
	}

	if (pushBottan_[static_cast<int>(InputType::Activate)]) {
		return;
	}

	if (input.currentInputDevice_) {
		input.DrawKeyGraph(static_cast<int>(InputType::Activate), UIPos_[UIGraph::KeyBord].first, UIPos_[UIGraph::KeyBord].second, 1.2f);
	}
	else {
		//�摜�`��
		input.DrawPadGraph(static_cast<int>(XboxBotton::B), UIPos_[UIGraph::XboxBotton].first, UIPos_[UIGraph::XboxBotton].second, 1.0f);
	}

	//�L�[�ɑΉ�����������̕`��(�A�N�V�����L�[�̕�����)
	DrawStringToHandle(Game::screen_width / 2, static_cast<int>(Game::screen_height - keybord_graph_chip_size * 1.6f), "���o�[������", 0xffffff, fontPigumo42_);
}

void Tutorial::CorpseScaffoldDraw(InputState& input)
{
	if (input.IsTriggered(InputType::Death)) {
		pushBottan_[static_cast<int>(InputType::Death)] = true;
	}

	if (pushBottan_[static_cast<int>(InputType::Death)]) {
		std::string str = "���̂͑���Ƃ��Ďg����";
		int width = GetDrawStringWidthToHandle(str.c_str(), static_cast<int>(str.size()), fontPigumo42_);
		DrawStringToHandle(Game::screen_width / 2 - width / 2, Game::screen_height - static_cast<int>(keybord_graph_chip_size * 1.6f), str.c_str(), 0xffffff, fontPigumo42_);
		return;
	}

	if (input.currentInputDevice_) {
		input.DrawKeyGraph(static_cast<int>(InputType::Death), UIPos_[UIGraph::KeyBord].first, UIPos_[UIGraph::KeyBord].second, 1.2f);
	}
	else {
		//�摜�`��
		input.DrawPadGraph(static_cast<int>(XboxBotton::Y), UIPos_[UIGraph::XboxBotton].first, UIPos_[UIGraph::XboxBotton].second, 1.0f);
	}

	//�L�[�ɑΉ�����������̕`��(�A�N�V�����L�[�̕�����)
	input.DrawName(static_cast<int>(InputType::Death), static_cast<float>(Game::screen_width / 2), Game::screen_height - keybord_graph_chip_size * 1.6f, 0xffffff, fontPigumo42_, false, false);
}
