#include "UIItemManager.h"
#include "Util.h"
#include "UIItem.h"
#include "FontsManager.h"
#include "game.h"

UIItemManager::UIItemManager()
{
}

UIItemManager::~UIItemManager()
{
}

void UIItemManager::AlphaChangeDraw(int selectNum, int fadeAlphaValue)
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

void UIItemManager::ChangePosDraw(float centerPosX, float centerPosY)
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

void UIItemManager::DrawBillBoard(std::map<std::string, VECTOR> drawPos,int alpha, float size)
{
	for (int i = 0; i < UIMenu_.size(); i++) {
		//�摜�ɏ����ꂽ�������擾
		std::string name = UIMenu_[i]->GetString();

		//�摜��3D��Ԃɕ`�悷��
		UIMenu_[i]->DrawBillboard(drawPos[name],alpha,size);
	}
}

void UIItemManager::AlphaChangeDrawBillBoard(std::map<std::string, VECTOR> drawPos, int selectNum,int fadeValue, float size)
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

void UIItemManager::AddMenu(float centerPosX, float centerPosY, int width, int height, std::string str, int fontHandle)
{
	//�C���X�^���X��
	UIMenu_.push_back(std::make_shared<UIItem>());

	//���j���[�̒ǉ�
	UIMenu_.back()->CreateUIGraphSetUp(centerPosX,centerPosY, width, height, str, fontHandle);
}

void UIItemManager::AddingMenuWithSplitStr(float centerPosX, float centerPosY, int width, int height, std::string str, int fontHandle)
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
