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
		//radianの取得
		radian = (time + (UIMenu_.size() - i) * 36) * DX_PI_F / 180.0f;
		float sin = std::sin(radian);

		//文字が波のように動くようにした
		UIMenu_[i]->ChangePosDraw(centerPosX + i * 20, centerPosY + sin * 10);
	}

}

void UIItemManager::DrawBillBoard(std::map<std::string, VECTOR> drawPos,int alpha, float size)
{
	for (int i = 0; i < UIMenu_.size(); i++) {
		//画像に書かれた文字を取得
		std::string name = UIMenu_[i]->GetString();

		//画像を3D空間に描画する
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

		//画像に書かれた文字を取得
		std::string name = UIMenu_[i]->GetString();

		//画像を3D空間に描画する
		UIMenu_[i]->DrawBillboard(drawPos[name], static_cast<int>(alpha), size);
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
	int num = static_cast<int>(str.size());
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
