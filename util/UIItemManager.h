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
	/// 引数で送られてきた番号だけ
	/// 拡縮率を変更するときの描画
	/// </summary>
	/// <param name="selectNum"></param>
	void AlphaChangeDraw(int selectNum);

	/// <summary>
	/// UIメニューの追加
	/// </summary>
	/// <param name="centerPosX">ポジションのX</param>
	/// <param name="centerPosY">ポジションのY</param>
	/// <param name="width"></param>
	/// <param name="height"></param>
	/// <param name="str">文字列</param>
	/// <param name="fontHandle">フォントハンドル</param>
	void AddMenu(float centerPosX, float centerPosY, int width, int height, const char* str, int fontHandle);

private:

	std::vector<std::shared_ptr<UIItem>> UIMenu_;

};

