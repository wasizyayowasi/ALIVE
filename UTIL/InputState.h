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
class Tutorial;

class InputState
{
	friend KeyConfigScene;
	friend Tutorial;
public:
	~InputState();

	static InputState& GetInstance() {
		static InputState instance;
		return instance;
	}

	/// <summary>
	/// �������s�\���͊֐�
	/// </summary>
	/// <param name="type">�L�[�̃^�C�v</param>
	/// <returns>�����Ă��邩�ǂ���</returns>
	bool IsTriggered(InputType type) const;

	/// <summary>
	/// �������\���͊֐�
	/// </summary>
	/// <param name="type">�L�[�̃^�C�v</param>
	/// <returns>�����Ă��邩�ǂ���</returns>
	bool IsPressed(InputType type) const;

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �ύX���̃L�[�R���t�B�O��tempMapTable�ɏ�������
	/// </summary>
	/// <param name="type">�L�[�^�C�v</param>
	/// <param name="cat">���͑��u�̃J�e�S��</param>
	/// <param name="id">�L�[�̔ԍ�</param>
	void RewriteInputInfo(InputType type, InputCategory cat, int id);

	/// <summary>
	/// �L�[�̕ύX�����s����
	/// </summary>
	void CommitChangedInputInfo();

	/// <summary>
	/// �ύX�O�̃L�[�R���t�B�O�ɖ߂�
	/// </summary>
	void RollbackChangedInputInfo();

	/// <summary>
	/// �L�[�R���t�B�O������������
	/// </summary>
	void ResetInputInfo();


	void UndoSelectKey(InputType type, InputCategory cat);

	/// <summary>
	/// �L�[�R���t�B�O���O���t�@�C���Ƃ��ĕۑ�����
	/// </summary>
	void SavekeyInfo()const;

	/// <summary>
	/// �L�[�R���t�B�O��ǂݍ���
	/// </summary>
	/// <param name="filename">�O���t�@�C���̃p�X</param>
	void LoadKeyInfo(const char* filename);

	/// <summary>
	/// �Ō�ɓ��͂��ꂽ���͑��u�𔻕ʂ���
	/// </summary>
	/// <returns>true:�L�[�{�[�h�@false�F�p�b�h</returns>
	bool LastInputDevice() const;

private:

	InputState();

	InputState(const InputState&) = delete;
	void operator = (const InputState&) = delete;

	bool currentInputDevice_ = false;			//true:�L�[�{�[�h�@false:�p�b�h

	using InputMap_t = std::map<InputType, std::vector<InputInfo>>;
	InputMap_t inputMapTable_;

	InputMap_t tempMapTable_;

	InputMap_t defaultMapTable_;

	std::map<InputType, std::string> inputNameTable_;

	std::vector<bool> currentInput_;
	std::vector<bool> lastInput_;

	std::unordered_map<int, int> rewriteKeyInfo_;
};
