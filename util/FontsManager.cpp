#include "FontsManager.h"
#include "DxLib.h"
#include <cassert>

int FontsManager::getFontHandle(const char* filename)
{
	return fontHandle_[filename];
}

FontsManager::FontsManager()
{
	addFonts("High Tower Text",32);
}

FontsManager::~FontsManager()
{
	for (auto& font : fontHandle_) {
		DeleteFontToHandle(font.second);
	}
}

void FontsManager::addFonts(const char* filename, int fontSize)
{
	std::string path = "data/fonts/";
	path += filename;
	path += ".ttf";

	AddFontResourceEx(path.c_str(), FR_PRIVATE, NULL);

	fontHandle_[filename] = CreateFontToHandle(filename, fontSize,-1,-1);
	assert(fontHandle_[filename] != -1);

}

int FontsManager::getStringSize(const char* string, const char* filename)
{
	titleWidth_ = GetDrawStringWidthToHandle(string, strlen(string), fontHandle_[filename]);
	return titleWidth_;
}
