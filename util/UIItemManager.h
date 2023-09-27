#pragma once
#include <vector>
#include <memory>

class UIItem;

class UIItemManager
{
public:

	UIItemManager();
	virtual ~UIItemManager();

	void draw(int selectNum);

	void addMenu(float centerPosX, float centerPosY, int width, int height, const char* str, int fontHandle);

private:

	std::vector<std::shared_ptr<UIItem>> UIMenu_;

};

