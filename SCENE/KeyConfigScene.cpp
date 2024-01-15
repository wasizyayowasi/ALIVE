#include "KeyConfigScene.h"
#include "SceneManager.h"
#include "SettingScene.h"
#include "PopUpTextScene.h"

#include"../util/InputState.h"
#include"../util/game.h"
#include"../util/DrawFunctions.h"
#include"../util/FontsManager.h"
#include "../util/UIItemManager.h"

#include <DxLib.h>
#include <algorithm>

namespace {
	//key�摜�̈ꖇ������̃T�C�Y
	constexpr int graph_chip_size = 59;
	//�摜�̌��ԃT�C�Y
	constexpr int graph_gap_size = 10;
}

//�R���X�g���N�^
KeyConfigScene::KeyConfigScene(SceneManager& manager, const InputState& input):SceneBase(manager),
updateFunc_(&KeyConfigScene::FadeInUpdate),
changeKeyUpdateFunc_(&KeyConfigScene::SelectChangeKeyUpdate),
drawFunc_(&KeyConfigScene::KeyStateDraw), 
inputState_(input)
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
	keyNum_[112] = Key::�J�i;
	keyNum_[121] = Key::�ϊ�;
	keyNum_[123] = Key::���ϊ�;
	keyNum_[125] = Key::YEN_mark;
	keyNum_[144] = Key::Caret;
	keyNum_[145] = Key::At_mark;
	keyNum_[146] = Key::Colon;
	keyNum_[148] = Key::����;
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

//�f�X�g���N�^
KeyConfigScene::~KeyConfigScene()
{
}

void KeyConfigScene::Init()
{
	//�摜�̓ǂݍ���
	controllerHandle_ = LoadGraph("data/graph/controller.png");

	//�t�H���g�̍쐬
	fontHandleSize21_ = FontsManager::getInstance().GetFontHandle("�s�O�� 0021");
	fontHandleSize42_ = FontsManager::getInstance().GetFontHandle("�s�O�� 0042");
	//�C���X�^���X��
	UI_ = std::make_shared<UIItemManager>();

	//�L�[�̖����̈�ڂ̍��W
	int namePosX = Game::screen_width / 4;
	int namePosY = Game::screen_height / 2 - (graph_chip_size * 4 + 45);

	int nameNo = 0;
	for (auto& table : inputState_.inputNameTable_) {
		//���j���[�̒ǉ�
		UI_->AddMenu(namePosX, namePosY, 320, 100, table.second.c_str(), fontHandleSize21_);

		//�|�W�V��������
		namePosY += graph_chip_size + graph_gap_size;

		//inputstate.tempmaptable�̔����𒴂�����܂�Ԃ�
		if (nameNo == inputState_.tempMapTable_.size() / 2 - 1) {
			namePosY = Game::screen_height / 2 - (graph_chip_size * 4 + 30);
			namePosX += Game::screen_width / 4 * 1.5f;
		}

		nameNo++;
	}

	//���j���[�̒ǉ�
	UI_->AddMenu(Game::screen_width / 2, Game::screen_height / 5 * 4, 320, 100, "�ύX", fontHandleSize42_);
	UI_->AddMenu(Game::screen_width / 2, Game::screen_height / 5 * 4 + 32, 320, 100, "�L�����Z��", fontHandleSize42_);

	//�X�N���[���T�C�Y�̃n���h�����쐬
	makeScreenHandle_ = MakeScreen(Game::screen_width, Game::screen_height, true);
	//�L�[�O���t��ǂݍ���
	keyTypeHandle_ = Graph::LoadGraph("data/graph/key2.png");
}

void KeyConfigScene::End()
{
	//���݂̃L�[���͏����O���f�[�^�Ƃ��ď����o��
	//inputState_.savekeyInfo();
	inputState_.SavekeyInfo2();
	DeleteGraph(makeScreenHandle_);
	DeleteGraph(keyTypeHandle_);
	DeleteGraph(controllerHandle_);
}

//�����o�֐��|�C���^�̍X�V
void KeyConfigScene::Update(const InputState& input)
{
	(this->*updateFunc_)(input);
}

//�`��
void KeyConfigScene::Draw()
{
	//�w�i�ɍ��̓���
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
	DrawBox(0, 0, Game::screen_width, Game::screen_height, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	GraphFilter(makeScreenHandle_, DX_GRAPH_FILTER_GAUSS, 8, 255 - fadeValue_);

	//makeScreen�ō쐬�����n���h����`��
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeValue_);
	DrawGraph(0, 0, makeScreenHandle_, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	(this->*drawFunc_)();
	
	//----------------�ȍ~�����\��
	DrawString(0, 0, "keyConfig",0xffffff);
//	DrawLine(Game::screen_width / 2, 0, Game::screen_width / 2, Game::screen_height, 0xff0000);
//	DrawLine(Game::screen_width / 4, 0, Game::screen_width / 4, Game::screen_height, 0xff0000);
//	DrawLine(Game::screen_width / 4 * 3, 0, Game::screen_width / 4 * 3, Game::screen_height, 0xff0000);
//	DrawLine(Game::screen_width / 6, 0, Game::screen_width / 6, Game::screen_height, 0xff0000);
}

void KeyConfigScene::KeyStateDraw()
{
	//�������݃X�N���[���̕ύX
	SetDrawScreen(makeScreenHandle_);
	//�X�N���[���̃N���A
	ClearDrawScreen();

	//�L�[�̖�����`�悷��
	UI_->AlphaChangeDraw(selectNum_,fadeValue_);

	//�L�[�ɑΉ�����摜��`�悷��
	KeyGraphDraw();

	SetDrawScreen(DX_SCREEN_BACK);

}

void KeyConfigScene::KeyGraphDraw()
{
	//�|�W�V�����ݒ�
	float graphPosX = Game::screen_width / 4 * 1.5f;
	float graphPosY = Game::screen_height / 2 - (graph_chip_size * 4 + 30.0f);
	//dxlib���̊e�L�[�ԍ�����肷��
	int keyNum = 0;
	//for���ŉ��Ԗڂ����擾����
	int keyNo = 0;
	//���邳�������l
	int subBrightness = 180;
	//�摜�̊g�k��
	float graphScale = 1.0f;

	for (auto& key : inputState_.tempMapTable_) {
		//key�ԍ����擾����
		keyNum = static_cast<int>(keyNum_[key.second.begin()->id]);
		//�摜�𕪊����邽�߂̔z��ԍ����擾����
		int graphArrayX = keyNum % 9;
		int graphArrayY = keyNum / 9;

		//���݃J�[�\���������Ă���ꍇ
		//�T�C�Y�Ɩ��邳�������l��ύX����
		if (keyNo == selectNum_) {
			graphScale = 1.2f;
			subBrightness = 0;
		}
		else {
			graphScale = 1.0f;
			subBrightness = 180;
		}

		//�����摜�̕`��
		Graph::DrawRectRotaGraph(graphPosX, graphPosY, graphArrayX * graph_chip_size, graphArrayY * graph_chip_size, graph_chip_size, graph_chip_size, graphScale, 0.0f, keyTypeHandle_, true, false);

		//�Â������摜���摜�̏�ɏ悹��
		SetDrawBlendMode(DX_BLENDMODE_SUB, subBrightness);
		DrawBoxAA(graphPosX - 80.0f, graphPosY - graph_chip_size / 2 - graph_gap_size / 2, graphPosX - 240.0f + Game::screen_width / 4, graphPosY + graph_chip_size / 2 + graph_gap_size / 2, 0x000000, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		//�����𒴂�����܂�Ԃ�����
		graphPosY += graph_chip_size + 10;
		if (keyNo == inputState_.tempMapTable_.size() / 2 - 1) {
			graphPosY = Game::screen_height / 2 - (graph_chip_size * 4 + 30);
			graphPosX += Game::screen_width / 4 * 1.5f;
		}

		keyNo++;
	}
}

void KeyConfigScene::ControllerDraw()
{

	//�������݃X�N���[���̕ύX
	SetDrawScreen(makeScreenHandle_);
	//�X�N���[���̃N���A
	ClearDrawScreen();

	DrawGraph(0, 0, controllerHandle_, true);

	SetDrawScreen(DX_SCREEN_BACK);
}

void KeyConfigScene::ChangeKeyPopUpText()
{
	//�w�i�̍��`��
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
	DrawBox(Game::screen_width / 6, Game::screen_height / 5, Game::screen_width / 6 * 5, Game::screen_height / 5 * 4, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//������
	const char* text = inputState_.inputNameTable_.find(static_cast<InputType>(selectNum_))->second.c_str();
	//������̉���
	int strWidth = GetDrawStringWidthToHandle(text, strlen(text), fontHandleSize21_);
	//�I�������L�[�̖��O���o��
	DrawStringToHandle(Game::screen_width / 6 + strWidth / 2 , Game::screen_height / 5 - graph_chip_size / 2 - 8, text, 0xffffff, fontHandleSize21_);

	//�I�������L�[��id���擾���ĉ摜�𕪊�����
	int num = static_cast<int>(keyNum_[inputState_.tempMapTable_.find(static_cast<InputType>(selectNum_))->second.begin()->id]);
	int x = num % 9;
	int y = num / 9;
	//�I�������L�[�̉摜���o��
	Graph::DrawRectRotaGraph(Game::screen_width / 6 + strWidth *2.5f, Game::screen_height / 5 - graph_chip_size / 2, x * graph_chip_size, y * graph_chip_size, graph_chip_size, graph_chip_size, 1.0f, 0.0f, keyTypeHandle_, true, false);

	//������
	text = "�ύX�������L�[����͂��Ă�������";
	//������̉���
	strWidth = GetDrawStringWidthToHandle(text, strlen(text), fontHandleSize42_);
	//������̕`��
	DrawStringToHandle(Game::screen_width / 2 - strWidth / 2, Game::screen_height / 2, text, 0xffffff, fontHandleSize42_);

}

//�ύX�������L�[��I������
void KeyConfigScene::SelectChangeKeyUpdate()
{
	//�Z�k��
	auto& configInput = const_cast<InputState&>(inputState_);
	//���݂̃L�[�̐����擾����
	const int keyNum = static_cast<int>(inputState_.inputNameTable_.size() + 2);

	int lastSelectNum = selectNum_;

	//TODO:�܂Ƃ߂�
	//�I�𑀍�
	{
		if (inputState_.IsTriggered(InputType::up)) {
			if (selectNum_ == (keyNum - 1) / 2) {
				selectNum_ += keyNum / 2;
			}
			else {
				selectNum_ = ((selectNum_ - 1) + keyNum) % keyNum;
			}
		}
		if (inputState_.IsTriggered(InputType::down)) {
			if (selectNum_ + 1 == (keyNum - 1) / 2) {
				selectNum_ += keyNum / 2;
			}
			else {
				selectNum_ = (selectNum_ + 1) % keyNum;
			}
		}
		if (inputState_.IsTriggered(InputType::left) || inputState_.IsTriggered(InputType::right)) {
			selectNum_ = (selectNum_ + (keyNum / 2 - 1)) % (keyNum - 2);
		}
	}

	//�L�[�̕ύX��ۑ�����
	if (selectNum_ == inputState_.inputNameTable_.size()) {
		if (inputState_.IsTriggered(InputType::space)) {
			configInput.CommitChangedInputInfo();		//���̃L�[�������ۂɎQ�Ƃ��Ă���L�[���ɑ������
			manager_.PushFrontScene(std::shared_ptr<SceneBase>(std::make_shared<PopUpTextScene>(manager_)));		//�V�[�����|�b�v�A�b�v�e�L�X�g��`�悷��V�[���ɕύX����
			return;
		}
	}

	//���L�[�����������ă|�[�Y�V�[���ɖ߂�
	if (selectNum_ == inputState_.inputNameTable_.size() + 1) {
		if (inputState_.IsTriggered(InputType::space)) {
			configInput.ResetInputInfo();
			updateFunc_ = &KeyConfigScene::FadeOutUpdate;
			return;
		}
	}
	
	//�ǂ̃L�[��ύX���邩��������
	if (inputState_.IsTriggered(InputType::space)) {
		isEditing_ = !isEditing_;
		drawFunc_ = &KeyConfigScene::ChangeKeyPopUpText;
		changeKeyUpdateFunc_ = &KeyConfigScene::ChangeKeyborardUpdate;
		GraphFilter(makeScreenHandle_, DX_GRAPH_FILTER_GAUSS, 32, 800);
		return;
	}

	//�ЂƂO�̃V�[���ɖ߂�
	if (inputState_.IsTriggered(InputType::pause)) {
		updateFunc_ = &KeyConfigScene::FadeOutUpdate;
		configInput.RollbackChangedInputInfo();
		return;
	}

	//�R���g���[���[�̓��͂��������Ƃ���
	//�R���g���[���[�p�̕`��ƍX�V�p�N���X�ɕύX����
	if (!inputState_.LastInputDevice()) {
		changeKeyUpdateFunc_ = &KeyConfigScene::ControllerUpdate;
		drawFunc_ = &KeyConfigScene::ControllerDraw;
	}

}

//�ύX�������L�[���ǂ̃L�[�ɕύX����̂�
void KeyConfigScene::ChangeKeyborardUpdate()
{
	//��t���[���O�̓��͏�񂪎c���Ă����烊�^�[������
	for (const auto& key : inputState_.lastInput_) {
		if (key == true) return;
	}

	//�Z�k��
	auto& configInput = const_cast<InputState&>(inputState_);

	//�����o�֐��|�C���^��ύX����L�[��I������֐��ɕύX����
	if (inputState_.IsTriggered(InputType::pause)) {
		changeKeyUpdateFunc_ = &KeyConfigScene::SelectChangeKeyUpdate;
		drawFunc_ = &KeyConfigScene::KeyStateDraw;
		isEditing_ = !isEditing_;
		configInput.UndoSelectKey(static_cast<InputType>(selectNum_), InputCategory::keybd);
		return;
	}

	//�L�[�{�[�h�ƃp�b�h�̓��͂𓾂�
	char keyState[256];
	auto padState = GetJoypadInputState(DX_INPUT_PAD1);
	GetHitKeyStateAll(keyState);

	int idx = 0;
	InputType currentType = InputType::max;
	//���݂̑I��inputNameTable���擾����
	for (const auto& name : inputState_.inputNameTable_) {
		if (selectNum_ == idx) {
			currentType = name.first;
			break;
		}
		idx++;
	}

	//�L�[�{�[�h�̓��͂�ύX����
	for (int i = 0; i < 256; i++) {
		if (keyState[i]) {
			configInput.RewriteInputInfo(currentType, InputCategory::keybd, i);
			isEditing_ = !isEditing_;
			changeKeyUpdateFunc_ = &KeyConfigScene::SelectChangeKeyUpdate;
			drawFunc_ = &KeyConfigScene::KeyStateDraw;
			break;
		}
	}
	//�p�b�h�̓��͂�ύX����
	if (padState != 0) {
		configInput.RewriteInputInfo(currentType, InputCategory::pad, padState);
		isEditing_ = !isEditing_;
		changeKeyUpdateFunc_ = &KeyConfigScene::SelectChangeKeyUpdate;
		drawFunc_ = &KeyConfigScene::KeyStateDraw;
	}

}

void KeyConfigScene::ControllerUpdate()
{
	//�Z�k��
	auto& configInput = const_cast<InputState&>(inputState_);

	//���L�[�����������ă|�[�Y�V�[���ɖ߂�
	if (selectNum_ == 0) {
		if (inputState_.IsTriggered(InputType::space)) {
			configInput.ResetInputInfo();
			updateFunc_ = &KeyConfigScene::FadeOutUpdate;
			return;
		}
	}

	//�ЂƂO�̃V�[���ɖ߂�
	if (inputState_.IsTriggered(InputType::pause)) {
		updateFunc_ = &KeyConfigScene::FadeOutUpdate;
		configInput.RollbackChangedInputInfo();
		return;
	}

	//�L�[�{�[�h�̓��͂��������Ƃ���
	//�L�[�{�[�h�p�̕`��ƍX�V�p�N���X�ɕύX����
	if (inputState_.LastInputDevice()) {
		changeKeyUpdateFunc_ = &KeyConfigScene::SelectChangeKeyUpdate;
		drawFunc_ = &KeyConfigScene::KeyStateDraw;
	}

}

void KeyConfigScene::FadeInUpdate(const InputState& input)
{
	float timer = static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_);
	fadeValue_ = static_cast <int>(255 * timer);
	if (++fadeTimer_ == fadeInterval_) {
		updateFunc_ = &KeyConfigScene::NormalUpdate;
		fadeValue_ = 255;
		if (!input.LastInputDevice()) {
			changeKeyUpdateFunc_ = &KeyConfigScene::ControllerUpdate;
			drawFunc_ = &KeyConfigScene::ControllerDraw;
		}
	}
}

void KeyConfigScene::NormalUpdate(const InputState& input)
{
	(this->*changeKeyUpdateFunc_)();
}

void KeyConfigScene::FadeOutUpdate(const InputState& input)
{
	fadeValue_ = static_cast <int>(255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_)));
	if (--fadeTimer_ == 0) {
		manager_.SwapScene(std::shared_ptr<SceneBase>(std::make_shared<SettingScene>(manager_)));
		fadeValue_ = 0;
		return;
	}
}
