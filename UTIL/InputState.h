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
	activate,		//�^��
	creative,		//�f�o�b�O�p
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

	bool IsTriggered(InputType type) const;

	bool IsPressed(InputType type) const;

	void Update();

	void RewriteInputInfo(InputType type, InputCategory cat, int id);

	void CommitChangedInputInfo();

	void RollbackChangedInputInfo();

	void ResetInputInfo();

	void UndoSelectKey(InputType type, InputCategory cat);

	void SavekeyInfo()const;

	void LoadKeyInfo();

	void SavekeyInfo2()const;

	void LoadKeyInfo2(const char* filename);

	bool LastInputDevice() const;

private:

	bool currentInputDevice_ = false;			//true:�L�[�{�[�h�@false:�p�b�h

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
