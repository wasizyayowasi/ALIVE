#include "UIItemManager.h"
#include "UIItem.h"

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
		UIMenu_[i]->AlphaChangeDraw(scale,alpha);
	}
}

void UIItemManager::AddMenu(float centerPosX, float centerPosY, int width, int height, const char* str, int fontHandle)
{
	UIMenu_.push_back(std::make_shared<UIItem>());
	UIMenu_.back()->CreateUIGraphSetUp(centerPosX,centerPosY, width, height, str, fontHandle);
}
