#pragma once
#include <vector>
#include <memory>

class UIItem;

class UIItemManager
{
public:

	UIItemManager();
	virtual ~UIItemManager();

	/// <summary>
	/// �����ő����Ă����ԍ�����
	/// �g�k����ύX����Ƃ��̕`��
	/// </summary>
	/// <param name="selectNum"></param>
	void AlphaChangeDraw(int selectNum);

	/// <summary>
	/// UI���j���[�̒ǉ�
	/// </summary>
	/// <param name="centerPosX">�|�W�V������X</param>
	/// <param name="centerPosY">�|�W�V������Y</param>
	/// <param name="width"></param>
	/// <param name="height"></param>
	/// <param name="str">������</param>
	/// <param name="fontHandle">�t�H���g�n���h��</param>
	void AddMenu(float centerPosX, float centerPosY, int width, int height, const char* str, int fontHandle);

private:

	std::vector<std::shared_ptr<UIItem>> UIMenu_;

};

