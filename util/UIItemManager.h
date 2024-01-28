#pragma once
#include <DxLib.h>
#include <map>
#include <vector>
#include <memory>
#include <string>

class UIItem;

class UIItemManager
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	UIItemManager();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~UIItemManager();

	/// <summary>
	/// �����ő����Ă����ԍ�����
	/// �g�k����ύX����Ƃ��̕`��
	/// </summary>
	/// <param name="selectNum">�ԍ�</param>
	void AlphaChangeDraw(int selectNum,int fadeAlphaValue);

	/// <summary>
	/// �|�W�V��������ύX����UI�̕`��
	/// </summary>
	/// <param name="centerPosX">�|�W�V����X</param>
	/// <param name="centerPosY">�|�W�V����Y</param>
	void ChangePosDraw(float centerPosX, float centerPosY);

	/// <summary>
	/// �摜��3D��Ԃɕ`�悷��
	/// </summary>
	/// <param name="drawPos">�`����W</param>
	/// <param name="alpha">�A���t�@�l</param>
	/// /// <param name="size">�T�C�Y</param>
	void DrawBillBoard(std::map<std::string,VECTOR> drawPos, float alpha,float size);

	/// <summary>
	/// �摜��3D��Ԃɕ`�悷��
	/// </summary>
	/// <param name="drawPos">�`����W</param>
	/// <param name="selectNum">�I��ԍ�</param>
	/// <param name="fadeValue">�A���t�@�l</param>
	/// /// <param name="size">�T�C�Y</param>
	void AlphaChangeDrawBillBoard(std::map<std::string, VECTOR> drawPos, int selectNum, int fadeValue, float size);

	/// <summary>
	/// UI���j���[�̒ǉ�
	/// </summary>
	/// <param name="centerPosX">�|�W�V������X</param>
	/// <param name="centerPosY">�|�W�V������Y</param>
	/// <param name="width">�X�N���[���̉��̃T�C�Y</param>
	/// <param name="height">�X�N���[���̏c�̃T�C�Y</param>
	/// <param name="str">������</param>
	/// <param name="fontHandle">�t�H���g�n���h��</param>
	void AddMenu(float centerPosX, float centerPosY, int width, int height, std::string str, int fontHandle);

	/// <summary>
	/// ������𕪊��������j���[�̒ǉ�
	/// </summary>
	/// <param name="centerPosX">�|�W�V������X</param>
	/// <param name="centerPosY">�|�W�V������Y</param>
	/// <param name="width">�X�N���[���̉��̃T�C�Y</param>
	/// <param name="height">�X�N���[���̏c�̃T�C�Y</param>
	/// <param name="str">������</param>
	/// <param name="fontHandle">�t�H���g�n���h��</param>
	void AddingMenuWithSplitStr(float centerPosX, float centerPosY, int width, int height, std::string str, int fontHandle);

private:

	std::vector<std::shared_ptr<UIItem>> UIMenu_;			//UIItem�̃X�}�[�g�|�C���^��vector�z��
	std::map<std::string, int>	fontSizeWithAppliedFont_;	//�t�H���g��K�p���������̑傫��

};

