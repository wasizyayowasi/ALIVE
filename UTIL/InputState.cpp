#include "InputState.h"
#include <DxLib.h>
#include <cassert>

#include<iostream>
#include <nlohmann/json.hpp>
#include <fstream>
#include <cassert>

using json = nlohmann::json;
using namespace std;

InputState::InputState()
{
	//�|�[�Y
	defaultMapTable_[InputType::pause] =		{ {InputCategory::keybd,KEY_INPUT_TAB},
												{InputCategory::pad,PAD_INPUT_8} };
	//��
	defaultMapTable_[InputType::up] =			{ {InputCategory::keybd,KEY_INPUT_W},
												{InputCategory::pad,PAD_INPUT_UP} };
	//��
	defaultMapTable_[InputType::down] =			{ {InputCategory::keybd,KEY_INPUT_S},
												{InputCategory::pad,PAD_INPUT_DOWN} };
	//��
	defaultMapTable_[InputType::left] =			{ {InputCategory::keybd,KEY_INPUT_A},
												{InputCategory::pad,PAD_INPUT_LEFT} };
	//�E
	defaultMapTable_[InputType::right] =		{ {InputCategory::keybd,KEY_INPUT_D},
												{InputCategory::pad,PAD_INPUT_RIGHT} };
	//����
	defaultMapTable_[InputType::upArrow] =		{ {InputCategory::keybd,KEY_INPUT_UP},
												/*{InputCategory::pad,PAD_INPUT_UP}*/ };
	//�����
	defaultMapTable_[InputType::downArrow] =	{ {InputCategory::keybd,KEY_INPUT_DOWN},
												/*{InputCategory::pad,PAD_INPUT_DOWN}*/ };
	//�����
	defaultMapTable_[InputType::leftArrow] =	{ {InputCategory::keybd,KEY_INPUT_LEFT},
												/*{InputCategory::pad,PAD_INPUT_LEFT}*/ };
	//�E���
	defaultMapTable_[InputType::rightArrow] =	{ {InputCategory::keybd,KEY_INPUT_RIGHT},
												 /*{InputCategory::pad,PAD_INPUT_RIGHT}*/ };
	//Space
	defaultMapTable_[InputType::space] =		{ {InputCategory::keybd,KEY_INPUT_SPACE},
												{InputCategory::pad,PAD_INPUT_1} };
	//���S
	defaultMapTable_[InputType::death] =		{ {InputCategory::keybd,KEY_INPUT_Z},
												{InputCategory::pad,PAD_INPUT_4} };
	//shift
	defaultMapTable_[InputType::shift] =		{ {InputCategory::keybd,KEY_INPUT_LSHIFT},
												{InputCategory::pad,PAD_INPUT_3} };
	//activate
	defaultMapTable_[InputType::activate] =		{ {InputCategory::keybd,KEY_INPUT_F},
												{InputCategory::pad,PAD_INPUT_2} };

	defaultMapTable_[InputType::creative] =		{ {InputCategory::keybd,KEY_INPUT_Q},
												{InputCategory::pad,PAD_INPUT_5} };

	inputMapTable_ = defaultMapTable_;

	//loadKeyInfo();
	LoadKeyInfo2("keyInfo");

	//�ꎞ�}�b�v�e�[�u���ɃR�s�[
	tempMapTable_ = inputMapTable_;

	//���̓^�C�v�̖��O�̃e�[�u�������
	inputNameTable_[InputType::pause] = "�|�[�Y";
	inputNameTable_[InputType::up] = "��ֈړ�";
	inputNameTable_[InputType::down] = "���ֈړ�";
	inputNameTable_[InputType::left] = "���ֈړ�";
	inputNameTable_[InputType::right] = "�E�ֈړ�";
	inputNameTable_[InputType::upArrow] = "�������";
	inputNameTable_[InputType::downArrow] = "��������";
	inputNameTable_[InputType::leftArrow] = "��������";
	inputNameTable_[InputType::rightArrow] = "�E������";
	inputNameTable_[InputType::space] = "����/�W�����v";
	inputNameTable_[InputType::death] = "���S";
	inputNameTable_[InputType::shift] = "����";
	inputNameTable_[InputType::activate] = "�A�N�V����";


	currentInput_.resize(static_cast<int>(InputType::max));
	lastInput_.resize(static_cast<int>(InputType::max));

}

bool InputState::IsTriggered(InputType type) const
{
	return IsPressed(type) && !lastInput_[static_cast<int>(type)];
}

bool InputState::IsPressed(InputType type) const
{
	
	return currentInput_[static_cast<int>(type)];
}

void InputState::Update()
{

	lastInput_ = currentInput_;//���O�̓��͏����L�����Ă���

	char keystate[256];
	GetHitKeyStateAll(keystate);//�S�L�[���擾

	int padState = GetJoypadInputState(DX_INPUT_PAD1);//�p�b�h1�R���̏����擾����

	for (const auto& keymap : inputMapTable_) {			//�}�b�v�̑S�������[�v����
		for (const auto& input : keymap.second) {		//���͏��z������[�v����
			//���̒��g�́Akeybd,KEY_INPUT_RETURN�Ȃǂ̃Z�b�g(InputInfo)�������Ă�
			//keymap.second�ɂ́A���̓��͏��Z�b�gInputInfo�������Ă���B
			//keymap.first�ɂ́A�Ή�����Q�[�����͖���"InputType"�Ȃǂ������Ă���

			if (input.cat == InputCategory::keybd) {
				currentInput_[static_cast<int>(keymap.first)] = keystate[input.id];
				if (currentInput_[static_cast<int>(keymap.first)]) {
					currentInputDevice_ = true;
				}
			}
			else if (input.cat == InputCategory::pad) {
				currentInput_[static_cast<int>(keymap.first)] = padState & input.id;
				if (currentInput_[static_cast<int>(keymap.first)]) {
					currentInputDevice_ = false;
				}
			}
			//2�̓��͂̂����ǂꂩ��true������������u���͂���Ă�v
			//�Ƃ݂Ȃ��āAbreak����B
			if (currentInput_[static_cast<int>(keymap.first)]) {
				break;
			}
		}
	}
}

void InputState::RewriteInputInfo(InputType type, InputCategory cat, int id)
{
	//���͎��(���蓖�Đ�)���Ȃ���΁A�����Ȃ̂Ŗ������܂��B
	if (tempMapTable_.count(type) == 0) {
		return;
	}

	bool isRewrite = false;

	for (auto& info : tempMapTable_[type]) {
		if (info.cat == cat)//�J�e�S�����q�b�g������
		{
			info.id = id;//ID���㏑������
			isRewrite = true;
			break;
		}
	}
	if (!isRewrite) {//�����J�e�S�������݂��Ȃ�������A�����Œǉ����Ă���
		tempMapTable_[type].push_back({ cat,id });
	}
}

//�ύX����������
void InputState::CommitChangedInputInfo()
{
	inputMapTable_ = tempMapTable_;
}

//�O�̏�Ԃɖ߂�
void InputState::RollbackChangedInputInfo()
{
	tempMapTable_ = inputMapTable_;
}

//���Z�b�g
void InputState::ResetInputInfo()
{
	inputMapTable_ = defaultMapTable_;
	tempMapTable_ = defaultMapTable_;
}

void InputState::UndoSelectKey(InputType type, InputCategory cat)
{
	for (auto& info : tempMapTable_[type]) {
		if (info.cat == cat) {
			info.id = inputMapTable_[type].begin()->id;
			break;
		}
	}
}

//TODO�F����
//�����o��
//�����g��Ȃ��Ǝv��
void InputState::SavekeyInfo() const
{
	FILE* fp = nullptr;

	auto err = fopen_s(&fp, "key.info", "wb");
	assert(fp != nullptr);

	//���z�L�[�^�C�v�̐�����������
	int keyTypeNum = inputMapTable_.size();
	fwrite(&keyTypeNum, sizeof(keyTypeNum), 1, fp);
	//���z�L�[�^�C�v(next�Aprev�Ȃ�)�̃��[�v
	for (const auto& key : inputMapTable_) {
		int keytype = static_cast<int>(key.first);
		//���z�L�[�ԍ�
		fwrite(&keytype, sizeof(keytype), 1, fp);
		int dataSize = key.second.size();
		//�������̓f�[�^������̂�
		fwrite(&dataSize, sizeof(dataSize), 1, fp);

		//Vector�^��data()�͐擪�̃A�h���X��Ԃ�
		//��C�ɂ��̓��̓f�[�^����������
		fwrite(key.second.data(), dataSize * sizeof(InputInfo), 1, fp);
	}

	fclose(fp);
}

//TODO�F����
//�ǂݍ���
//�����g��Ȃ��Ǝv��
void InputState::LoadKeyInfo()
{
	int handle = FileRead_open("key.info");
	assert(handle != -1);

	int keyTypeNum = 0;
	FileRead_read(&keyTypeNum, sizeof(keyTypeNum), handle);

	//�������������g������
	inputMapTable_.clear();

	//�O���f�[�^�̓ǂݍ���
	for (int i = 0; i < keyTypeNum; i++) {
		int  inputType;
		FileRead_read(&inputType, sizeof(inputType), handle);

		int dataSize = 0;
		FileRead_read(&dataSize, sizeof(dataSize), handle);
		std::vector<InputInfo>inputInfoes(dataSize);
		FileRead_read(inputInfoes.data(), sizeof(InputInfo) * dataSize, handle);
		inputMapTable_[static_cast<InputType>(inputType)] = inputInfoes;
	}
	tempMapTable_ = inputMapTable_;
	FileRead_close(handle);
}

void InputState::SavekeyInfo2() const
{
	//���ߑł������o���Ȃ��̂��l�b�N
	json keyInfo_[static_cast<int>(InputType::max)];
	//�z��ԍ���i�߂邽�߂̕ϐ�(�o����Ȃ珑�������Ȃ�)
	int i = 0;
	for (auto& input : inputMapTable_) {
		//inputMapTable_(map�^)�̃L�[�����Ƃɒl���擾����
		//vector�̃T�C�Y��l�̃T�C�Y���p�ӂ���
		std::vector<InputInfo> rewrite(input.second.size());
		rewrite = input.second;

		//���������̓��͑��u���������ꍇ�͎蓮�Ńv���O�����𑝂₳�Ȃ��ƍs���Ȃ�������
		//�e���͑��u���ƑΉ�����id���擾
		//�L�[�{�[�h
		std::unordered_map<int, int> keybord;
		keybord[static_cast<int>(rewrite.front().cat)] = static_cast<int>(rewrite.front().id);
		//�p�b�h
		std::unordered_map<int, int> pad;
		pad[static_cast<int>(rewrite.back().cat)] = static_cast<int>(rewrite.back().id);

		//�e���͑��u��id�A�T�C�Y�AinputType���f�[�^�ɂ܂Ƃ߂�
		keyInfo_[i] = {
			{"inputType",static_cast<int>(input.first)},
			{"inputSize",static_cast<int>(input.second.size())},
			{"keybord",keybord},
			{"pad",pad},
		};
		i++;
	}

	//�o�͂����f�[�^���܂Ƃ߂�
	json keySize = {
		{"name","keyInfo"},
		{"keyTypeNum",inputMapTable_.size()},
		{"temp",keyInfo_}
	};

	//�����o���t�@�C������"name"�ɂ���(�ʂɖ����Ă�����)
	string filename = keySize["name"];
	string extension = ".json";
	filename += extension;
	filename = "data/jsonFile/" + filename;

	//�o��
	ofstream writeingFile;
	//�t�@�C�����J��
	writeingFile.open(filename, ios::out);
	//�܂Ƃ߂��f�[�^�����ɂ܂Ƃ߂�
	writeingFile << keySize.dump() << std::endl;
	//����
	writeingFile.close();

}

void InputState::LoadKeyInfo2(const char* filename)
{

	//�������������g������
	inputMapTable_.clear();

	//�ǂݍ��ރt�@�C���̃p�X�𐶐�
	string path = "data/jsonFile/";
	path += filename;
	path += ".json";

	//�ǂݍ��ނ���
	ifstream ifs(path.c_str());
	assert(ifs);

	//�r�b�g���V�t�g����H
	//�悭�킩���Ă��Ȃ�
	//�K�������Ǌo���ĂȂ�
	json json_;
	ifs >> json_;

	//���O���擾���Ă���(��������Ȃ����ǎc���Ă�(��))
	string name = json_["name"];
	//inputState�̑���
	int keyTypeNum = json_["keyTypeNum"];

	//���ߑł������o���Ȃ��̂��l�b�N(�m���A����s��)
	json inputInfo[static_cast<int>(InputType::max)];
	//���������ꌙ��
	int i = 0;

	//�e���͑��u�Aid���擾
	for (auto& aiu : json_["temp"]) {
		inputInfo[i] = aiu;
		i++;
	}

	//inputMapTable_�ɓǂݍ��񂾃f�[�^����
	for (auto& const info : inputInfo) {
		//���͑��u�ԍ��ƃL�[id�����ꂼ��ێ����邽�߂̂��(�ЂƂ܂Ƃ߂ɂł���Ȃ玀��)
		std::unordered_map<int, int> keybord;
		std::unordered_map<int, int> pad;

		//�ǂݍ��񂾏����ꎞ�ۊǂ��邽�߂̕ϐ�
		InputInfo tempInfo;

		//�f�[�^���擾
		int type = info["inputType"];
		int inputSize = info["inputSize"];
		keybord = info["keybord"];
		pad = info["pad"];

		//�e���͑��u�ƃL�[id���ЂƂ܂Ƃ߂Ɏ����߂̕ϐ�
		std::vector<InputInfo> inputInfo(inputSize);

		//�e���͑��u�ƃL�[id���擾
		tempInfo.cat = static_cast<InputCategory>(keybord.begin()->first);
		tempInfo.id = keybord.begin()->second;
		inputInfo.front() = tempInfo;
		tempInfo.cat = static_cast<InputCategory>(pad.begin()->first);
		tempInfo.id = pad.begin()->second;
		inputInfo.back() = tempInfo;

		//�ǂݍ��񂾏���������
		inputMapTable_[static_cast<InputType>(type)] = inputInfo;
	}

	//�ꉞ����
	ifs.close();
}

bool InputState::LastInputDevice() const
{
	return currentInputDevice_;
}
