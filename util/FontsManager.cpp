#include "FontsManager.h"
#include <DxLib.h>
#include <cassert>

FontsManager::FontsManager()
{
	AddFonts("�s�O�� 00",".otf", 42);
	AddFonts("�s�O�� 00",".otf", 21);
}

FontsManager::~FontsManager()
{
	for (auto& font : fontHandle_) {
		DeleteFontToHandle(font.second);
	}
}

// �t�H���g��������
void FontsManager::AddFonts(const char* const filename, const char* const extension, int fontSize)
{
	//�t�H���g�p�X
	std::string path = "data/fonts/";
	path += filename;
	path += extension;

	//�t�H���g�̍쐬
	AddFontResourceEx(path.c_str(), FR_PRIVATE, NULL);

	//�t�H���g���͓����ł��T�C�Y�ō��ʉ����邽�߂�
	//�t�H���g�T�C�Y�𖼑O�̍Ō�ɉ�����
	std::string name = filename + std::to_string(fontSize);

	//�쐬�������O���L�[�Ƀn���h����z��ɉ�����
	fontHandle_[name] = CreateFontToHandle(filename, fontSize,-1,-1);
	assert(fontHandle_[filename] != -1);

}

// �t�H���g��K�p�����e�L�X�g�̉������擾����
int FontsManager::GetStringSize(const char* const string, const char* const filename)
{
	int strWidth = GetDrawStringWidthToHandle(string, static_cast<int>(strlen(string)), fontHandle_[filename]);
	return strWidth;
}

// �t�H���g���擾����
int FontsManager::GetFontHandle(const char* const filename)
{
	return fontHandle_[filename];
}

// �����̒l�����Ƀt�H���g�̖��O��Ԃ�
std::string FontsManager::GetFontName(int fontHandle)
{
	std::string fontname;

	//�����Ɠ����l�̃t�H���g�����z���T��
	for (auto& font : fontHandle_) {
		if (fontHandle == font.second) {
			fontname = font.first;
		}
	}

	return fontname;
}
