#include "UIItemManager.h"
#include "UIItem.h"

UIItemManager::UIItemManager()
{
}

UIItemManager::~UIItemManager()
{
}

void UIItemManager::draw(int selectNum)
{
	float scale = 1.0f;
	int alpha = 150;
	for (int i = 0; i < UIMenu_.size();i++) {
		if (selectNum == i) {
			scale = 1.0f;
			alpha = 255;
		}
		else {
			scale = 0.7f;
			alpha = 150;
		}
		UIMenu_[i]->draw(scale,alpha);
	}
}

void UIItemManager::addMenu(float centerPosX, float centerPosY, int width, int height, const char* str, int fontHandle)
{
	UIMenu_.push_back(std::make_shared<UIItem>());
	UIMenu_.back()->createUIGraphSetUp(centerPosX,centerPosY, width, height, str, fontHandle);
}
