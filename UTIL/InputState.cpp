#include "InputState.h"
#include "DxLib.h"
#include <cassert>

#include<iostream>
#include <nlohmann/json.hpp>
#include <fstream>
#include <cassert>

using json = nlohmann::json;
using namespace std;

InputState::InputState()
{
	//����
	defaultMapTable_[InputType::next] = { {InputCategory::keybd,KEY_INPUT_RETURN},
										{InputCategory::pad,PAD_INPUT_1}};
	//�߂�
	defaultMapTable_[InputType::prev] = { {InputCategory::keybd,KEY_INPUT_BACK},
										  {InputCategory::pad,PAD_INPUT_2} };
	//�|�[�Y
	defaultMapTable_[InputType::pause] = { {InputCategory::keybd,KEY_INPUT_TAB},
										 {InputCategory::pad,PAD_INPUT_8} };
	//��
	defaultMapTable_[InputType::up] = { {InputCategory::keybd,KEY_INPUT_W},
									  {InputCategory::pad,PAD_INPUT_UP} };
	//��
	defaultMapTable_[InputType::down] = { {InputCategory::keybd,KEY_INPUT_S},
										{InputCategory::pad,PAD_INPUT_DOWN} };
	//��
	defaultMapTable_[InputType::left] = { {InputCategory::keybd,KEY_INPUT_A},
										{InputCategory::pad,PAD_INPUT_LEFT} };
	//�E
	defaultMapTable_[InputType::right] = { {InputCategory::keybd,KEY_INPUT_D},
									  {InputCategory::pad,PAD_INPUT_RIGHT} };
	//Space
	defaultMapTable_[InputType::space] = { {InputCategory::keybd,KEY_INPUT_SPACE},
									  {InputCategory::pad,PAD_INPUT_1} };
	//���S
	defaultMapTable_[InputType::death] = { {InputCategory::keybd,KEY_INPUT_Z},
									  {InputCategory::pad,PAD_INPUT_2} };
	//shift
	defaultMapTable_[InputType::shift] = { {InputCategory::keybd,KEY_INPUT_LSHIFT},
									  {InputCategory::pad,PAD_INPUT_3} };
	//ctrl
	defaultMapTable_[InputType::ctrl] = { {InputCategory::keybd,KEY_INPUT_LCONTROL},
									  {InputCategory::pad,PAD_INPUT_4} };

	inputMapTable_ = defaultMapTable_;

	//loadKeyInfo();
	loadKeyInfo2("keyInfo");

	//�ꎞ�}�b�v�e�[�u���ɃR�s�[
	tempMapTable_ = inputMapTable_;

	//���̓^�C�v�̖��O�̃e�[�u�������
	inputNameTable_[InputType::next] = "����";
	inputNameTable_[InputType::prev] = "�߂�";
	inputNameTable_[InputType::pause] = "�|�[�Y";
	inputNameTable_[InputType::up] = "��";
	inputNameTable_[InputType::down] = "��";
	inputNameTable_[InputType::left] = "��";
	inputNameTable_[InputType::right] = "�E";
	inputNameTable_[InputType::space] = "space";
	inputNameTable_[InputType::death] = "���S";
	inputNameTable_[InputType::shift] = "shift";
	inputNameTable_[InputType::ctrl] = "ctrl";


	currentInput_.resize(static_cast<int>(InputType::max));
	lastInput_.resize(static_cast<int>(InputType::max));

}

bool InputState::isTriggered(InputType type) const
{
	return isPressed(type) && !lastInput_[static_cast<int>(type)];
}

bool InputState::isPressed(InputType type) const
{
	return currentInput_[static_cast<int>(type)];
}

void InputState::update()
{
	lastInput_ = currentInput_;//���O�̓��͏����L�����Ă���

	char keystate[256];
	GetHitKeyStateAll(keystate);//�S�L�[���擾

	int padState = GetJoypadInputState(DX_INPUT_PAD1);//�p�b�h1�R���̏����擾����
	int mouseState = GetMouseInput();

	for (const auto& keymap : inputMapTable_) {			//�}�b�v�̑S�������[�v����
		for (const auto& input : keymap.second) {		//���͏��z������[�v����
			//���̒��g�́Akeybd,KEY_INPUT_RETURN�Ȃǂ̃Z�b�g(InputInfo)�������Ă�
			//keymap.second�ɂ́A���̓��͏��Z�b�gInputInfo�������Ă���B
			//keymap.first�ɂ́A�Ή�����Q�[�����͖���"InputType"�Ȃǂ������Ă���
			if (input.cat == InputCategory::keybd) {
				currentInput_[static_cast<int>(keymap.first)] = keystate[input.id];
			}
			else if (input.cat == InputCategory::pad) {
				currentInput_[static_cast<int>(keymap.first)] = padState & input.id;
			}
			//2�̓��͂̂����ǂꂩ��true������������u���͂���Ă�v
			//�Ƃ݂Ȃ��āAbreak����B
			if (currentInput_[static_cast<int>(keymap.first)]) {
				break;
			}
		}
	}
}

void InputState::rewriteInputInfo(InputType type, InputCategory cat, int id)
{
	//���͎��(���蓖�Đ�)���Ȃ���΁A�����Ȃ̂Ŗ������܂��B
	if (tempMapTable_.count(type) == 0) {
		return;
	}

	bool isRewrite = false;

	for (auto& InputInfo : tempMapTable_[type]) {
		if (InputInfo.cat == cat)//�J�e�S�����q�b�g������
		{
			InputInfo.id = id;//ID���㏑������
			isRewrite = true;
			break;
		}
	}
	if (!isRewrite) {//�����J�e�S�������݂��Ȃ�������A�����Œǉ����Ă���
		tempMapTable_[type].push_back({ cat,id });
	}
}

//�ύX����������
void InputState::commitChangedInputInfo()
{
	inputMapTable_ = tempMapTable_;
}

//�O�̏�Ԃɖ߂�
void InputState::rollbackChangedInputInfo()
{
	tempMapTable_ = inputMapTable_;
}

//���Z�b�g
void InputState::resetInputInfo()
{
	inputMapTable_ = defaultMapTable_;
	tempMapTable_ = defaultMapTable_;
}

//�����o��
void InputState::savekeyInfo() const
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
//�ǂݍ���
void InputState::loadKeyInfo()
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

void InputState::savekeyInfo2() const
{
	json keyInfo_[11];
	int i = 0;
	for (auto& input : inputMapTable_) {
		std::vector<InputInfo> rewrite(input.second.size());
		rewrite = input.second;

		std::unordered_map<int, int> keybord;
		keybord[static_cast<int>(rewrite.front().cat)] = static_cast<int>(rewrite.front().id);
		std::unordered_map<int, int> pad;
		pad[static_cast<int>(rewrite.back().cat)] = static_cast<int>(rewrite.back().id);

		keyInfo_[i] = {
			{"inputType",static_cast<int>(input.first)},
			{"inputSize",static_cast<int>(input.second.size())},
			{"keybord",keybord},
			{"pad",pad},
		};
		i++;
	}

	json keySize = {
		{"name","keyInfo"},
		{"keyTypeNum",inputMapTable_.size()},
	};

	string filename = keySize["name"];
	string extension = ".json";
	filename += extension;

	ofstream writeingFile;
	writeingFile.open(filename, ios::out);
	writeingFile << keySize.dump() << std::endl;
	for (const auto& temp : keyInfo_) {
		writeingFile << temp.dump() << std::endl;
	}
	writeingFile.close();

}

void InputState::loadKeyInfo2(const char* filename)
{
	string path = "data/jsonFile/";
	path += filename;
	path += ".json";

	ifstream ifs(path.c_str());
	assert(ifs);

	json json_;
	ifs >> json_;

	string name = json_["name"];
	int num = json_["keyTypeNum"];

	auto type = json_["inputType"];
	auto size = json_["inputSize"];
	auto keybord = json_["keybord"];
	auto pad = json_["pad"];

	size = static_cast<int>(size);

}
