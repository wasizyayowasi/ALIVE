#include "KeyConfigScene.h"
#include "SceneManager.h"
#include "ScenePause.h"
#include "PopUpTextScene.h"
#include"../util/InputState.h"
#include"../util/game.h"
#include"../util/DrawFunctions.h"
#include "DxLib.h"
#include <algorithm>

namespace {
	//key�摜�̈ꖇ������̃T�C�Y
	constexpr int graph_chip_size = 59;
}

//�R���X�g���N�^
KeyConfigScene::KeyConfigScene(SceneManager& manager, const InputState& input):SceneBase(manager),updateFunc_(&KeyConfigScene::selectChangeKeyUpdate), inputState_(input)
{
	keyTypeHandle_ = Graph::loadGraph("data/graph/key2.png");
}

//�f�X�g���N�^
KeyConfigScene::~KeyConfigScene()
{
	//���݂̃L�[���͏����O���f�[�^�Ƃ��ď����o��
	//inputState_.savekeyInfo();
	inputState_.savekeyInfo2();
	DeleteGraph(keyTypeHandle_);
}

//�����o�֐��|�C���^�̍X�V
void KeyConfigScene::update(const InputState& input)
{
	(this->*updateFunc_)();
}

//�`��
void KeyConfigScene::draw()
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	float x = Game::kScreenWidth / 4;
	float y = Game::kScreenHeight / 2 - (graph_chip_size * 4 + 30.0f);
	int num = 0;
	int i = 0;
	for (auto& key : inputState_.inputNameTable_) {
		DrawFormatString(x, y, 0xffffff, "%s", key.second.c_str());
		y += graph_chip_size + 10.0f;
		if (i == inputState_.tempMapTable_.size() / 2) {
			y = Game::kScreenHeight / 2 - (graph_chip_size * 4 + 30.0f);
			x += Game::kScreenWidth / 4 * 1.2f;
		}
		i++;
	}

	x = Game::kScreenWidth / 4 * 1.5f;
	y = Game::kScreenHeight / 2 - (graph_chip_size * 4 + 30.0f);
	i = 0;
	float graphScale = 1.0f;
	for (auto& key : inputState_.tempMapTable_) {
		num = getKeyName(key.second.begin()->id);
		int x2 = num % 9;
		int y2 = num / 9;

		if (i == selectNum_) {
			graphScale = 1.2f;
		}
		else {
			graphScale = 1.0f;
		}

		Graph::drawRectRotaGraph(x, y, x2 * graph_chip_size, y2 * graph_chip_size, graph_chip_size, graph_chip_size, graphScale, 0.0f, keyTypeHandle_, true, false);
		y += graph_chip_size + 10;
		if (i == inputState_.tempMapTable_.size() / 2) {
			y = Game::kScreenHeight / 2 - (graph_chip_size * 4 + 30.0f);
			x += Game::kScreenWidth / 4 * 1.2f;
		}
		i++;
	}
	
	//TODO:�����I�ɂ͎g��Ȃ��������
	//�ꎞ�I�Ɍ��₷�����邽��
	int color = 0xffffff;
	int color2 = 0xffffff;
	if (selectNum_ == 15) {
		color = 0xff0000;
	}
	else if(selectNum_ == 16){
		color2 = 0xff0000;
	}
	y += 50;
	DrawString(Game::kScreenWidth / 2 - 16, y, "�ύX", color);
	y += 16;
	DrawString(Game::kScreenWidth / 2 - 32, y, "�L�����Z��", color2);

	
	//----------------�ȍ~�����\��
	DrawString(0, 0, "keyConfig",0xffffff);
	DrawFormatString(0, 16, 0xffffff, "%d", selectNum_);
	DrawFormatString(0, y + 16, 0xffffff, "%d", aiu);
}

//�ύX�������L�[��I������
void KeyConfigScene::selectChangeKeyUpdate()
{
	//�Z�k��
	auto& configInput = const_cast<InputState&>(inputState_);
	//���݂̃L�[�̐����擾����
	const int keyNum = static_cast<int>(inputState_.inputNameTable_.size() + 2);

	//�I�𑀍�
	{
		if (inputState_.isTriggered(InputType::up)) {
			selectNum_ = (std::max)(selectNum_ - 1, 0);
		}
		if (inputState_.isTriggered(InputType::down)) {
			selectNum_ = (std::min)(selectNum_ + 1, keyNum - 1);
		}
	}

	//�L�[�̕ύX��ۑ�����
	if (selectNum_ == inputState_.inputNameTable_.size()) {
		if (inputState_.isTriggered(InputType::next)) {
			configInput.commitChangedInputInfo();		//���̃L�[�������ۂɎQ�Ƃ��Ă���L�[���ɑ������
			manager_.pushScene(std::shared_ptr<SceneBase>(std::make_shared<PopUpTextScene>(manager_)));		//�V�[�����|�b�v�A�b�v�e�L�X�g��`�悷��V�[���ɕύX����
			return;
		}
	}

	//���L�[�����������ă|�[�Y�V�[���ɖ߂�
	if (selectNum_ == inputState_.inputNameTable_.size() + 1) {
		if (inputState_.isTriggered(InputType::next)) {
			configInput.resetInputInfo();
			manager_.swapScene(std::shared_ptr<SceneBase>(std::make_shared<ScenePause>(manager_)));
			return;
		}
	}
	
	//�ǂ̃L�[��ύX���邩��������
	if (inputState_.isTriggered(InputType::next)) {
		updateFunc_ = &KeyConfigScene::changeKeyUpdate;
		return;
	}

	//�ЂƂO�̃V�[���ɖ߂�
	if (inputState_.isTriggered(InputType::prev)) {
		manager_.swapScene(std::shared_ptr<SceneBase>(std::make_shared<ScenePause>(manager_)));
		configInput.rollbackChangedInputInfo();
		return;
	}

}

//�ύX�������L�[���ǂ̃L�[�ɕύX����̂�
void KeyConfigScene::changeKeyUpdate()
{
	//�Z�k��
	auto& configInput = const_cast<InputState&>(inputState_);

	//�L�[�{�[�h�ƃp�b�h�̓��͂𓾂�
	char keyState[256];
	GetHitKeyStateAll(keyState);
	auto padState = GetJoypadInputState(DX_INPUT_PAD1);

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
			configInput.rewriteInputInfo(currentType, InputCategory::keybd, i);
			break;
		}
	}
	//�p�b�h�̓��͂�ύX����
	if (padState != 0) {
		configInput.rewriteInputInfo(currentType, InputCategory::pad, padState);
	}
	
	//�����o�֐��|�C���^��ύX����L�[��I������֐��ɕύX����
	if (inputState_.isTriggered(InputType::prev)) {
		updateFunc_ = &KeyConfigScene::selectChangeKeyUpdate;
		isEditing_ = !isEditing_;
		return;
	}

}

int KeyConfigScene::getKeyName(int num)
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
