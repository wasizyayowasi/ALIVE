#include "KeyConfigScene.h"
#include "SceneManager.h"
#include "ScenePause.h"
#include "PopUpTextScene.h"
#include"../util/InputState.h"
#include"../util/game.h"
#include"../util/DrawFunctions.h"
#include"../util/FontsManager.h"
#include "../util/UIItemManager.h"
#include "DxLib.h"
#include <algorithm>

namespace {
	//key�摜�̈ꖇ������̃T�C�Y
	constexpr int graph_chip_size = 59;
	//�摜�̌��ԃT�C�Y
	constexpr int graph_gap_size = 10;
}

//�R���X�g���N�^
KeyConfigScene::KeyConfigScene(SceneManager& manager, const InputState& input):SceneBase(manager),updateFunc_(&KeyConfigScene::SelectChangeKeyUpdate),drawFunc_(&KeyConfigScene::KeyStateDraw), inputState_(input)
{
}

//�f�X�g���N�^
KeyConfigScene::~KeyConfigScene()
{
}

void KeyConfigScene::Init()
{
	//�t�H���g�̍쐬
	fontHandleSize16_ = FontsManager::getInstance().GetFontHandle("�s�O�� 0021");
	fontHandleSize32_ = FontsManager::getInstance().GetFontHandle("�s�O�� 0042");
	//�C���X�^���X��
	UI_ = std::make_shared<UIItemManager>();

	//�L�[�̖����̈�ڂ̍��W
	int namePosX = Game::screen_width / 4;
	int namePosY = Game::screen_height / 2 - (graph_chip_size * 4 + 45);

	int nameNo = 0;
	for (auto& table : inputState_.inputNameTable_) {
		//���j���[�̒ǉ�
		UI_->addMenu(namePosX, namePosY, 320, 100, table.second.c_str(), fontHandleSize16_);

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
	UI_->addMenu(Game::screen_width / 2, Game::screen_height / 5 * 4, 320, 100, "�ύX", fontHandleSize32_);
	UI_->addMenu(Game::screen_width / 2, Game::screen_height / 5 * 4 + 32, 320, 100, "�L�����Z��", fontHandleSize32_);

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
}

//�����o�֐��|�C���^�̍X�V
void KeyConfigScene::Update(const InputState& input)
{
	(this->*updateFunc_)();
}

//�`��
void KeyConfigScene::Draw()
{
	//�w�i�ɍ��̓���
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
	DrawBox(0, 0, Game::screen_width, Game::screen_height, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//makeScreen�ō쐬�����n���h����`��
	DrawGraph(0, 0, makeScreenHandle_, true);

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
	UI_->draw(selectNum_);

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
		keyNum = GetKeyName(key.second.begin()->id);
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

void KeyConfigScene::ChangeKeyPopUpText()
{
	//�w�i�̍��`��
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
	DrawBox(Game::screen_width / 6, Game::screen_height / 5, Game::screen_width / 6 * 5, Game::screen_height / 5 * 4, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//������
	const char* text = inputState_.inputNameTable_.find(static_cast<InputType>(selectNum_))->second.c_str();
	//������̉���
	int strWidth = GetDrawStringWidthToHandle(text, strlen(text), fontHandleSize16_);
	//�I�������L�[�̖��O���o��
	DrawStringToHandle(Game::screen_width / 6 + strWidth / 2 , Game::screen_height / 5 - graph_chip_size / 2 - 8, text, 0xffffff, fontHandleSize16_);

	//�I�������L�[��id���擾���ĉ摜�𕪊�����
	int num = GetKeyName(inputState_.tempMapTable_.find(static_cast<InputType>(selectNum_))->second.begin()->id);
	int x = num % 9;
	int y = num / 9;
	//�I�������L�[�̉摜���o��
	Graph::DrawRectRotaGraph(Game::screen_width / 6 + strWidth *2.5f, Game::screen_height / 5 - graph_chip_size / 2, x * graph_chip_size, y * graph_chip_size, graph_chip_size, graph_chip_size, 1.0f, 0.0f, keyTypeHandle_, true, false);

	//������
	text = "�ύX�������L�[����͂��Ă�������";
	//������̉���
	strWidth = GetDrawStringWidthToHandle(text, strlen(text), fontHandleSize32_);
	//������̕`��
	DrawStringToHandle(Game::screen_width / 2 - strWidth / 2, Game::screen_height / 2, text, 0xffffff, fontHandleSize32_);

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
			selectNum_ = ((selectNum_ - 1) + keyNum) % keyNum;
		}
		if (inputState_.IsTriggered(InputType::down)) {
			selectNum_ = (selectNum_ + 1)% keyNum;
		}
		if (inputState_.IsTriggered(InputType::left) || inputState_.IsTriggered(InputType::right)) {
			selectNum_ = (selectNum_ + (keyNum / 2 - 1)) % (keyNum - 2);
		}
	}

	//�L�[�̕ύX��ۑ�����
	if (selectNum_ == inputState_.inputNameTable_.size()) {
		if (inputState_.IsTriggered(InputType::space)) {
			configInput.CommitChangedInputInfo();		//���̃L�[�������ۂɎQ�Ƃ��Ă���L�[���ɑ������
			manager_.PushScene(std::shared_ptr<SceneBase>(std::make_shared<PopUpTextScene>(manager_)));		//�V�[�����|�b�v�A�b�v�e�L�X�g��`�悷��V�[���ɕύX����
			return;
		}
	}

	//���L�[�����������ă|�[�Y�V�[���ɖ߂�
	if (selectNum_ == inputState_.inputNameTable_.size() + 1) {
		if (inputState_.IsTriggered(InputType::space)) {
			configInput.ResetInputInfo();
			manager_.SwapScene(std::shared_ptr<SceneBase>(std::make_shared<ScenePause>(manager_)));
			return;
		}
	}
	
	//�ǂ̃L�[��ύX���邩��������
	if (inputState_.IsTriggered(InputType::space)) {
		isEditing_ = !isEditing_;
		drawFunc_ = &KeyConfigScene::ChangeKeyPopUpText;
		updateFunc_ = &KeyConfigScene::ChangeKeyUpdate;
		GraphFilter(makeScreenHandle_, DX_GRAPH_FILTER_GAUSS, 32, 800);
		return;
	}

	//�ЂƂO�̃V�[���ɖ߂�
	if (inputState_.IsTriggered(InputType::pause)) {
		manager_.SwapScene(std::shared_ptr<SceneBase>(std::make_shared<ScenePause>(manager_)));
		configInput.RollbackChangedInputInfo();
		return;
	}

}

//�ύX�������L�[���ǂ̃L�[�ɕύX����̂�
void KeyConfigScene::ChangeKeyUpdate()
{
	//��t���[���O�̓��͏�񂪎c���Ă����烊�^�[������
	for (const auto& key : inputState_.lastInput_) {
		if (key == true) return;
	}

	//�Z�k��
	auto& configInput = const_cast<InputState&>(inputState_);

	//�����o�֐��|�C���^��ύX����L�[��I������֐��ɕύX����
	if (inputState_.IsTriggered(InputType::pause)) {
		updateFunc_ = &KeyConfigScene::SelectChangeKeyUpdate;
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
			updateFunc_ = &KeyConfigScene::SelectChangeKeyUpdate;
			drawFunc_ = &KeyConfigScene::KeyStateDraw;
			break;
		}
	}
	//�p�b�h�̓��͂�ύX����
	if (padState != 0) {
		configInput.RewriteInputInfo(currentType, InputCategory::pad, padState);
		isEditing_ = !isEditing_;
		updateFunc_ = &KeyConfigScene::SelectChangeKeyUpdate;
		drawFunc_ = &KeyConfigScene::KeyStateDraw;
	}

}

int KeyConfigScene::GetKeyName(int num)
{
	if (num < 12 && num > 0) {
		return num - 1;
	}
	
	if (num >= 14 && num <= 39) {
		return num - 2;
	}

	if (num >= 42 && num <= 83) {
		return num - 4;
	}

	switch (num) {
	case 87:
		return 80;
	case 88:
		return 81;
	case 112:
		return 82;
	case 121:
		return 83;
	case 123:
		return 84;
	case 125:
		return 85;
	case 144:
		return 86;
	case 145:
		return 87;
	case 146:
		return 88;
	case 148:
		return 89;
	case 156:
		return 90;
	case 157:
		return 91;
	case 181:
		return 92;
	case 183:
		return 93;
	case 184:
		return 94;
	case 197:
		return 95;
	case 199:
		return 96;
	case 200:
		return 97;
	case 201:
		return 98;
	case 203:
		return 99;
	case 205:
		return 100;
	case 207:
		return 101;
	case 208:
		return 102;
	case 209:
		return 103;
	case 210:
		return 104;
	case 211:
		return 105;
	case 219:
		return 106;
	case 220:
		return 107;
	case 221:
		return 108;
	}

	return num;
}
