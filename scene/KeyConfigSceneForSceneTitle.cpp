#include "KeyConfigSceneForSceneTitle.h"
#include "SceneManager.h"
#include "SettingScene.h"
#include "PopUpTextScene.h"
#include "SettingSceneForSceneTitle.h"

#include"../util/game.h"
#include"../util/InputState.h"
#include"../util/FontsManager.h"
#include"../util/DrawFunctions.h"
#include "../util/UIItemManager.h"
#include "../util/ExternalFile.h"

#include <DxLib.h>
#include <algorithm>

namespace {
	//key�摜�̈ꖇ������̃T�C�Y
	constexpr int graph_chip_size = 59;
	//�摜�̌��ԃT�C�Y
	constexpr int graph_gap_size = 10;
}

//�R���X�g���N�^
KeyConfigSceneForSceneTitle::KeyConfigSceneForSceneTitle(SceneManager& manager) :SceneBase(manager),
updateFunc_(&KeyConfigSceneForSceneTitle::FadeInUpdate)
{
	//�Z�k��
	auto& input = InputState::GetInstance();

	if (!input.LastInputDevice()) {
		changeKeyUpdateFunc_ = &KeyConfigSceneForSceneTitle::ControllerUpdate;
		drawFunc_ = &KeyConfigSceneForSceneTitle::ControllerDraw;
	}
	else {
		changeKeyUpdateFunc_ = &KeyConfigSceneForSceneTitle::SelectChangeKeyUpdate;
		drawFunc_ = &KeyConfigSceneForSceneTitle::KeyStateDraw;
	}

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

//�f�X�g���N�^
KeyConfigSceneForSceneTitle::~KeyConfigSceneForSceneTitle()
{
}

void KeyConfigSceneForSceneTitle::Init()
{
	//�摜�̓ǂݍ���
	controllerHandle_ = LoadGraph("data/graph/controller.png");

	//�L�[�O���t�𕪊����ēǂݍ���
	LoadDivGraph("data/graph/key2.png",117,9,13,graph_chip_size, graph_chip_size,keyTypeHandle1_);

	//�t�H���g�̍쐬
	fontHandleSize21_ = FontsManager::GetInstance().GetFontHandle("�s�O�� 0021");
	fontHandleSize42_ = FontsManager::GetInstance().GetFontHandle("�s�O�� 0042");

	//�C���X�^���X��
	KeyUI_ = std::make_shared<UIItemManager>();
	PadUI_ = std::make_shared<UIItemManager>();

	//�Z�k��
	auto& input = InputState::GetInstance();
	auto& file = ExternalFile::GetInstance();

	//�L�[�̖����̈�ڂ̍��W
	float namePosX = static_cast<float>(Game::screen_width / 4);
	float namePosY = static_cast<float>(Game::screen_height / 2) - (graph_chip_size * 4.0f + 45.0f);

	VECTOR pos = file.GetUIPos("keyConfPos");
	int inputSize = static_cast<int>(input.inputNameTable_.size());

	int no = 0;
	for (auto& input : input.inputNameTable_) {
		keyDrawPos_[input.second] = pos;
		pos.y -= 5.5f;
		if (no == inputSize / 2 - 1) {
			pos = file.GetUIPos("keyConfTurnBackPos");
		}
		no++;
	}

	keyDrawPos_["�ύX"] = file.GetUIPos("advancedSettingUIPos");
	keyDrawPos_["�L�����Z��"] = file.GetUIPos("backUIPos");

	padDrawPos_["�߂�"] = file.GetUIPos("backUIPos");

	int nameNo = 0;
	for (auto& table : input.inputNameTable_) {
		//���j���[�̒ǉ�
		KeyUI_->AddMenu(namePosX, namePosY, 320, 100, table.second.c_str(), fontHandleSize21_);
	
		//�|�W�V��������
		namePosY += graph_chip_size + graph_gap_size;
	
		//inputstate.tempmaptable�̔����𒴂�����܂�Ԃ�
		if (nameNo == input.tempMapTable_.size() / 2 - 1) {
			namePosY =  static_cast<float>(Game::screen_height / 2) - (graph_chip_size * 4.0f + 30.0f);
			namePosX += static_cast<float>(Game::screen_width / 4) * 1.5f;
		}
		nameNo++;
	}

	//���j���[�̒ǉ�
	KeyUI_->AddMenu(static_cast<float>(Game::screen_width / 2), static_cast<float>(Game::screen_height / 5 * 4), 320, 100, "�ύX", fontHandleSize21_);
	KeyUI_->AddMenu(static_cast<float>(Game::screen_width / 2), static_cast<float>(Game::screen_height / 5 * 4)+ 32.0f, 320, 100, "�L�����Z��", fontHandleSize21_);
	PadUI_->AddMenu(static_cast<float>(Game::screen_width / 2), static_cast<float>(Game::screen_height / 5 * 4)+ 32.0f, 320, 100, "�߂�", fontHandleSize21_);
}

void KeyConfigSceneForSceneTitle::End()
{
	//�Z�k��
	auto& input = InputState::GetInstance();

	//���݂̃L�[���͏����O���f�[�^�Ƃ��ď����o��
	input.SavekeyInfo();
	DeleteGraph(controllerHandle_);

	for (auto& graph : keyTypeHandle1_) {
		DeleteGraph(graph);
	}
}

//�����o�֐��|�C���^�̍X�V
void KeyConfigSceneForSceneTitle::Update()
{
	(this->*updateFunc_)();
}

//�`��
void KeyConfigSceneForSceneTitle::Draw()
{
	(this->*drawFunc_)();
}

void KeyConfigSceneForSceneTitle::KeyStateDraw()
{
	KeyUI_->AlphaChangeDrawBillBoard(keyDrawPos_, selectNum_, fadeValue_,50.0f);

	//�L�[�ɑΉ�����摜��`�悷��
	KeyGraphDraw();
}

void KeyConfigSceneForSceneTitle::KeyGraphDraw()
{
	//�Z�k��
	auto& input = InputState::GetInstance();

	//dxlib���̊e�L�[�ԍ�����肷��
	int keyNum = 0;

	//�A���t�@�l
	float alphaValue = 0;

	//�摜�̊g�k��
	float graphScale = 5.0f;

	float choosingValue = 250.0f / 255.0f;
	float notChoosingValue = 150.0f / 255.0f;

	VECTOR pos = ExternalFile::GetInstance().GetUIPos("keyGraphPos");

	int no = 0;
	for (auto& key : input.tempMapTable_) {
		//key�ԍ����擾����
		keyNum = static_cast<int>(keyNum_[key.second.begin()->id]);

		//���݃J�[�\���������Ă���ꍇ
		//�T�C�Y�Ɩ��邳�������l��ύX����
		if (no == selectNum_) {
			graphScale = 5.2f;
			alphaValue = choosingValue * fadeValue_;
		}
		else {
			graphScale = 5.0f;
			alphaValue = notChoosingValue * fadeValue_;
		}

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(alphaValue));
		DrawBillboard3D(pos, 0.5f, 0.5f, graphScale, 0.0f, keyTypeHandle1_[keyNum], true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		pos.y -= 5.5f;

		if (no == input.tempMapTable_.size() / 2 - 1) {
			pos = ExternalFile::GetInstance().GetUIPos("keyGraphTurnBackPos");
		}
		no++;
	}
}

void KeyConfigSceneForSceneTitle::ControllerDraw()
{
	//UI�z�u�|�W�V����
	VECTOR pos = ExternalFile::GetInstance().GetUIPos("controllerUIPos");

	//�R���g���[���[�摜�̕`��
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeValue_);
	DrawBillboard3D(pos, 0.5f, 0.5f, 90.0f, 0.0f, controllerHandle_, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//UI�̕`��
	PadUI_->AlphaChangeDrawBillBoard(padDrawPos_, selectNum_, fadeValue_, 50.0f);
}

void KeyConfigSceneForSceneTitle::ChangeKeyPopUpText()
{
	//�Z�k��
	auto& input = InputState::GetInstance();

	//�w�i�̍��`��
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
	DrawBox(Game::screen_width / 6, Game::screen_height / 5, Game::screen_width / 6 * 5, Game::screen_height / 5 * 4, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//������
	const char* text = input.inputNameTable_.find(static_cast<InputType>(selectNum_))->second.c_str();

	//������̉���
	int strWidth = GetDrawStringWidthToHandle(text, static_cast<int>(strlen(text)), fontHandleSize21_);


	//�I�������L�[�̖��O���o��
	DrawStringToHandle(Game::screen_width / 6 + strWidth / 2, Game::screen_height / 5 - graph_chip_size / 2 - 8, text, 0xffffff, fontHandleSize21_);

	//�I�������L�[��id���擾���ĉ摜�𕪊�����
	int num = static_cast<int>(keyNum_[input.tempMapTable_.find(static_cast<InputType>(selectNum_))->second.begin()->id]);
	
	//�I�������L�[�̉摜���o��
	DrawRotaGraphF(Game::screen_width / 6 + strWidth * 2.5f, static_cast<float>(Game::screen_height / 5 - graph_chip_size / 2), 1.0f, 0.0f, keyTypeHandle1_[num], true);


	//������
	text = "�ύX�������L�[����͂��Ă�������";
	//������̉���
	strWidth = GetDrawStringWidthToHandle(text, static_cast<int>(strlen(text)), fontHandleSize42_);
	//������̕`��
	DrawStringToHandle(Game::screen_width / 2 - strWidth / 2, Game::screen_height / 2, text, 0xffffff, fontHandleSize42_);

}

//�ύX�������L�[��I������
void KeyConfigSceneForSceneTitle::SelectChangeKeyUpdate()
{
	//�Z�k��
	auto& input = InputState::GetInstance();

	//���݂̃L�[�̐����擾����
	const int keyNum = static_cast<int>(input.inputNameTable_.size() + 2);

	int lastSelectNum = selectNum_;

	if (input.IsTriggered(InputType::up)) {
		if (selectNum_ == (keyNum - 1) / 2) {
			selectNum_ += keyNum / 2;
		}
		else {
			selectNum_ = ((selectNum_ - 1) + keyNum) % keyNum;
		}
	}
	else if (input.IsTriggered(InputType::down)) {
		if (selectNum_ + 1 == (keyNum - 1) / 2) {
			selectNum_ += keyNum / 2;
		}
		else {
			selectNum_ = (selectNum_ + 1) % keyNum;
		}
	}
	else if (input.IsTriggered(InputType::left) || input.IsTriggered(InputType::right)) {
		if (selectNum_ < keyNum - 2) {
			selectNum_ = (selectNum_ + ((keyNum - 2) / 2)) % (keyNum - 2);
		}
	}

	//�L�[�̕ύX��ۑ�����
	if (selectNum_ == input.inputNameTable_.size()) {
		if (input.IsTriggered(InputType::space)) {

			//���̃L�[�������ۂɎQ�Ƃ��Ă���L�[���ɑ������
			input.CommitChangedInputInfo();

			//�V�[�����|�b�v�A�b�v�e�L�X�g��`�悷��V�[���ɕύX����
			manager_.PushFrontScene(std::shared_ptr<SceneBase>(std::make_shared<PopUpTextScene>(manager_)));
			return;
		}
	}

	//���L�[�����������ă|�[�Y�V�[���ɖ߂�
	if (selectNum_ == input.inputNameTable_.size() + 1) {
		if (input.IsTriggered(InputType::space)) {
			input.ResetInputInfo();
			updateFunc_ = &KeyConfigSceneForSceneTitle::FadeOutUpdate;
			return;
		}
	}

	//�ǂ̃L�[��ύX���邩��������
	if (input.IsTriggered(InputType::space)) {
		isEditing_ = !isEditing_;
		drawFunc_ = &KeyConfigSceneForSceneTitle::ChangeKeyPopUpText;
		changeKeyUpdateFunc_ = &KeyConfigSceneForSceneTitle::ChangeKeyborardUpdate;
		return;
	}

	//�ЂƂO�̃V�[���ɖ߂�
	if (input.IsTriggered(InputType::pause)) {
		updateFunc_ = &KeyConfigSceneForSceneTitle::FadeOutUpdate;
		input.RollbackChangedInputInfo();
		return;
	}

	//�R���g���[���[�̓��͂��������Ƃ���
	//�R���g���[���[�p�̕`��ƍX�V�p�N���X�ɕύX����
	if (!input.LastInputDevice()) {
		changeKeyUpdateFunc_ = &KeyConfigSceneForSceneTitle::ControllerUpdate;
		drawFunc_ = &KeyConfigSceneForSceneTitle::ControllerDraw;
	}

}

//�ύX�������L�[���ǂ̃L�[�ɕύX����̂�
void KeyConfigSceneForSceneTitle::ChangeKeyborardUpdate()
{
	//�Z�k��
	auto& input = InputState::GetInstance();

	//��t���[���O�̓��͏�񂪎c���Ă����烊�^�[������
	for (const auto& key : input.lastInput_) {
		if (key == true) return;
	}

	//�����o�֐��|�C���^��ύX����L�[��I������֐��ɕύX����
	if (input.IsTriggered(InputType::pause)) {
		changeKeyUpdateFunc_ = &KeyConfigSceneForSceneTitle::SelectChangeKeyUpdate;
		drawFunc_ = &KeyConfigSceneForSceneTitle::KeyStateDraw;
		isEditing_ = !isEditing_;
		input.UndoSelectKey(static_cast<InputType>(selectNum_), InputCategory::keybd);
		return;
	}

	//�L�[�{�[�h�ƃp�b�h�̓��͂𓾂�
	char keyState[256];
	auto padState = GetJoypadInputState(DX_INPUT_PAD1);
	GetHitKeyStateAll(keyState);

	int idx = 0;
	InputType currentType = InputType::max;
	//���݂̑I��inputNameTable���擾����
	for (const auto& name : input.inputNameTable_) {
		if (selectNum_ == idx) {
			currentType = name.first;
			break;
		}
		idx++;
	}

	//�L�[�{�[�h�̓��͂�ύX����
	for (int i = 0; i < 256; i++) {
		if (keyState[i]) {
			input.RewriteInputInfo(currentType, InputCategory::keybd, i);
			isEditing_ = !isEditing_;
			changeKeyUpdateFunc_ = &KeyConfigSceneForSceneTitle::SelectChangeKeyUpdate;
			drawFunc_ = &KeyConfigSceneForSceneTitle::KeyStateDraw;
			break;
		}
	}
	//�p�b�h�̓��͂�ύX����
	if (padState != 0) {
		input.RewriteInputInfo(currentType, InputCategory::pad, padState);
		isEditing_ = !isEditing_;
		changeKeyUpdateFunc_ = &KeyConfigSceneForSceneTitle::SelectChangeKeyUpdate;
		drawFunc_ = &KeyConfigSceneForSceneTitle::KeyStateDraw;
	}

}

void KeyConfigSceneForSceneTitle::ControllerUpdate()
{
	//�Z�k��
	auto& input = InputState::GetInstance();

	selectNum_ = 0;

	//���L�[�����������ă|�[�Y�V�[���ɖ߂�
	if (selectNum_ == 0) {
		if (input.IsTriggered(InputType::space)) {
			input.ResetInputInfo();
			updateFunc_ = &KeyConfigSceneForSceneTitle::FadeOutUpdate;
			return;
		}
	}

	//�ЂƂO�̃V�[���ɖ߂�
	if (input.IsTriggered(InputType::pause)) {
		updateFunc_ = &KeyConfigSceneForSceneTitle::FadeOutUpdate;
		input.RollbackChangedInputInfo();
		return;
	}

	//�L�[�{�[�h�̓��͂��������Ƃ���
	//�L�[�{�[�h�p�̕`��ƍX�V�p�N���X�ɕύX����
	if (input.LastInputDevice()) {
		changeKeyUpdateFunc_ = &KeyConfigSceneForSceneTitle::SelectChangeKeyUpdate;
		drawFunc_ = &KeyConfigSceneForSceneTitle::KeyStateDraw;
	}

}

void KeyConfigSceneForSceneTitle::FadeInUpdate()
{
	float timer = static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_);
	fadeValue_ = static_cast <int>(255 * timer);
	if (++fadeTimer_ == fadeInterval_) {
		updateFunc_ = &KeyConfigSceneForSceneTitle::NormalUpdate;
		fadeValue_ = 255;
	}
}

void KeyConfigSceneForSceneTitle::NormalUpdate()
{
	(this->*changeKeyUpdateFunc_)();
}

void KeyConfigSceneForSceneTitle::FadeOutUpdate()
{
	fadeValue_ = static_cast <int>(255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_)));
	if (--fadeTimer_ == 0) {
		manager_.SwapScene(std::shared_ptr<SceneBase>(std::make_shared<SettingSceneForSceneTitle>(manager_)));
		fadeValue_ = 0;
		return;
	}
}