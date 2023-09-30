#pragma once

#include <vector>
#include <map>
#include <unordered_map>
#include <string>

enum class InputType
{
	pause,			// �|�[�Y�{�^��
	up,				//��
	down,			//��
	left,			//��
	right,			//�E
	upArrow,		//��
	downArrow,		//��
	leftArrow,		//��
	rightArrow,		//�E
	space,			//space
	death,			//���S����
	shift,			//shift
	ctrl,			//ctrl
	carry,			//�^��
	max,			//�ő���̓C���f�b�N�X
};

enum class InputCategory {
	keybd,		//�L�[�{�[�h
	pad,		//�Q�[���p�b�h
};

struct InputInfo {
	InputCategory cat;
	int id;
};

class KeyConfigScene;

class InputState
{
	friend KeyConfigScene;
public:
	InputState();

	bool isTriggered(InputType type) const;

	bool isPressed(InputType type) const;

	void update();

	void rewriteInputInfo(InputType type, InputCategory cat, int id);

	void commitChangedInputInfo();

	void rollbackChangedInputInfo();

	void resetInputInfo();

	void undoSelectKey(InputType type, InputCategory cat);

	void savekeyInfo()const;

	void loadKeyInfo();

	void savekeyInfo2()const;

	void loadKeyInfo2(const char* filename);

	bool lastInput();

private:

	using InputMap_t = std::map<InputType, std::vector<InputInfo>>;
	InputMap_t inputMapTable_;

	InputMap_t tempMapTable_;

	InputMap_t defaultMapTable_;

	std::map<InputType, std::string> inputNameTable_;

	std::vector<bool> currentInput_;
	std::vector<bool> lastInput_;

	std::unordered_map<int, int> rewriteKeyInfo_;
	std::vector<InputInfo> temp2;
};
