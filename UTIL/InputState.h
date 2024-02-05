#pragma once
#include "KeyData.h"
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
	dush,			//����
	sit,			//����
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

class Tutorial;
class KeyConfigScene;
class KeyConfigSceneForSceneTitle;

class InputState
{
	friend Tutorial;
	friend KeyConfigScene;
	friend KeyConfigSceneForSceneTitle;
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

	/// <summary>
	/// �ύX�����L�[��߂�
	/// </summary>
	/// <param name="type">�L�[�̃^�C�v</param>
	/// <param name="cat">���͑��u�̃J�e�S��</param>
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

	/// <summary>
	/// ���͑��u�̔ԍ����擾����
	/// </summary>
	/// <param name="type">�{�^���̃^�C�v</param>
	/// <param name="cat">���͑��u�̃J�e�S���[</param>
	/// <returns>���͑��u�̔ԍ�</returns>
	int GetInputNum(int num,InputCategory cat);

	/// <summary>
	/// key�摜�̕`��
	/// </summary>
	/// <param name="type">�`�悵����key</param>
	/// <param name="posX">�`����WX</param>
	/// <param name="posY">�`����WY</param>
	/// <param name="size">�`��T�C�YY</param>
	void DrawKeyGraph(InputType type,float posX, float posY, float size);

	/// <summary>
	/// pad�̃{�^���̕`��
	/// </summary>
	/// <param name="type">�`�悵�����{�^��</param>
	/// <param name="posX">�`����WX</param>
	/// <param name="posY">�`����WY</param>
	void DrawPadGraph(XboxBotton type, float posX, float posY);

	/// <summary>
	/// ���O�̕`��
	/// </summary>
	/// <param name="type">�`�悵����key</param>
	/// <param name="posX">�`����WX</param>
	/// <param name="posY">�`����WY</param>
	/// <param name="color">�F</param>
	/// <param name="fontHandle">�t�H���g</param>
	
	/// <summary>
	/// ���O�̕`��
	/// </summary>
	/// <param name="type">�`�悵����key</param>
	/// <param name="posX">�`����WX</param>
	/// <param name="posY">�`����WY</param>
	/// <param name="color">�F</param>
	/// <param name="fontHandle">�t�H���g</param>
	/// <param name="editName">���O��ҏW���邩</param>
	/// <param name="before">�O���ォ�@true : �O�@false�F���</param>
	/// <param name="sign">�L��</param>
	void DrawName(InputType type, float posX, float posY, int color, int fontHandle,bool editName,bool before,std::string sign = "");
private:

	InputState();

	InputState(const InputState&) = delete;
	void operator = (const InputState&) = delete;


	bool currentInputDevice_ = false;			//true:�L�[�{�[�h�@false:�p�b�h

	using InputMap_t = std::map<InputType, std::vector<InputInfo>>;
	InputMap_t inputMapTable_;

	InputMap_t tempMapTable_;

	InputMap_t defaultMapTable_;

	std::map<int, Key> keyNum_;							//�L�[�ԍ�
	std::map<InputCategory, int> UIHandle_;					//�摜
	std::map<InputType, std::string> inputNameTable_;

	std::vector<bool> currentInput_;
	std::vector<bool> lastInput_;

	std::unordered_map<int, int> rewriteKeyInfo_;
};
