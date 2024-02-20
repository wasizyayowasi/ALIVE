#include "UIItemManager.h"
#include "Util.h"
#include "UIItem.h"
#include "FontsManager.h"
#include "game.h"

//�R���X�g���N�^
UIItemManager::UIItemManager()
{
}

//�f�X�g���N�^
UIItemManager::~UIItemManager()
{
}

//�����ő����Ă����ԍ�����
//�g�k����ύX����Ƃ��̕`��
void UIItemManager::AlphaChangeDraw(const int selectNum, const int fadeAlphaValue)
{
	float scale = 1.0f;
	float alpha = 150;

	float currentNum = 250.0f / 255.0f;
	float notCurrentNum = 150.0f / 255.0f;

	for (int i = 0; i < UIMenu_.size();i++) {
		if (selectNum == i) {
			scale = 1.0f;
			alpha = currentNum * fadeAlphaValue;
		}
		else {
			scale = 0.7f;
			alpha = notCurrentNum * fadeAlphaValue;
		}
		UIMenu_[i]->AlphaChangeDraw(scale, static_cast<int>(alpha));
	}
}

//�|�W�V��������ύX����UI�̕`��
void UIItemManager::ChangePosDraw(const float centerPosX, const float centerPosY)
{
	static int time = 0;
	float radian = 0.0f;

	time++;

	for (int i = 0; i < UIMenu_.size(); i++) {
		//radian�̎擾
		radian = (time + (UIMenu_.size() - i) * 36) * DX_PI_F / 180.0f;
		float sin = std::sin(radian);

		//�������g�̂悤�ɓ����悤�ɂ���
		UIMenu_[i]->ChangePosDraw(centerPosX + i * 20, centerPosY + sin * 10);
	}

}

//�摜��3D��Ԃɕ`�悷��
void UIItemManager::DrawBillBoard(std::map<std::string, VECTOR> drawPos, const int alpha, const float size)
{
	for (int i = 0; i < UIMenu_.size(); i++) {
		//�摜�ɏ����ꂽ�������擾
		std::string name = UIMenu_[i]->GetString();

		//�摜��3D��Ԃɕ`�悷��
		UIMenu_[i]->DrawBillboard(drawPos[name],alpha,size);
	}
}

//�A���t�@�l��ύX�����摜��3D��Ԃɕ`�悷��
void UIItemManager::AlphaChangeDrawBillBoard(std::map<std::string, VECTOR> drawPos, const int selectNum, const int fadeValue, const float size)
{
	float alpha = 100;

	float currentNum = 250.0f / 255.0f;
	float notCurrentNum = 100.0f / 255.0f;

	for (int i = 0; i < UIMenu_.size(); i++) {

		if (selectNum == i) {
			alpha = currentNum * fadeValue;
		}
		else {
			alpha = notCurrentNum * fadeValue;
		}

		//�摜�ɏ����ꂽ�������擾
		std::string name = UIMenu_[i]->GetString();

		//�摜��3D��Ԃɕ`�悷��
		UIMenu_[i]->DrawBillboard(drawPos[name], static_cast<int>(alpha), size);
	}
}

//UI���j���[�̒ǉ�
void UIItemManager::AddMenu(const float centerPosX, const float centerPosY, const int width, const int height, std::string str, const int fontHandle)
{
	//�C���X�^���X��
	UIMenu_.push_back(std::make_shared<UIItem>());

	//���j���[�̒ǉ�
	UIMenu_.back()->CreateUIGraphSetUp(centerPosX,centerPosY, width, height, str, fontHandle);
}

//������𕪊��������j���[�̒ǉ�
void UIItemManager::AddingMenuWithSplitStr(const float centerPosX, const float centerPosY, const int width, const int height, std::string str, const int fontHandle)
{
	//�Z�k��
	auto& font = FontsManager::GetInstance();

	//�����̐����擾
	int num = static_cast<int>(str.size());
	float size = 0.0f;

	for (int i = 0; i < num; i++) {
		//�ꕶ�����擾
		std::string letter = str.substr(i, 1);

		//�C���X�^���X��
		UIMenu_.push_back(std::make_shared<UIItem>());

		//�摜�̍쐬
		UIMenu_.back()->CreateUIGraphSetUp(centerPosX, centerPosY, width, height, letter, fontHandle);
	}
}
