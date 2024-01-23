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
		UIMenu_[i]->AlphaChangeDraw(scale,alpha);
	}
}

void UIItemManager::ChangePosDraw(float centerPosX, float centerPosY)
{
	static int time = 0;
	float radian = 0.0f;

	time++;

	for (int i = 0; i < UIMenu_.size(); i++) {
		//radianの取得
		radian = (time + (UIMenu_.size() - i) * 36) * DX_PI_F / 180.0f;
		float sin = std::sin(radian);

		//文字が波のように動くようにした
		UIMenu_[i]->ChangePosDraw(centerPosX + i * 20, centerPosY + sin * 10);
	}

}

void UIItemManager::AddMenu(float centerPosX, float centerPosY, int width, int height, std::string str, int fontHandle)
{
	//インスタンス化
	UIMenu_.push_back(std::make_shared<UIItem>());

	//メニューの追加
	UIMenu_.back()->CreateUIGraphSetUp(centerPosX,centerPosY, width, height, str, fontHandle);
}

void UIItemManager::AddingMenuWithSplitStr(float centerPosX, float centerPosY, int width, int height, std::string str, int fontHandle)
{
	//短縮化
	auto& font = FontsManager::GetInstance();

	//文字の数を取得
	int num = str.size();
	float size = 0.0f;

	for (int i = 0; i < num; i++) {
		//一文字を取得
		std::string letter = str.substr(i, 1);

		//インスタンス化
		UIMenu_.push_back(std::make_shared<UIItem>());

		//画像の作成
		UIMenu_.back()->CreateUIGraphSetUp(centerPosX, centerPosY, width, height, letter, fontHandle);
	}
}
