#pragma once
#include <string>
#include <unordered_map>

class FontsManager
{
public:

	~FontsManager();

	static FontsManager& getInstance() {
		static FontsManager instance;
		return instance;
	}

	/// <summary>
	/// フォントを加える
	/// </summary>
	/// <param name="filename">ファイルの名前</param>
	/// <param name="fontSize">フォントのサイズ指定</param>
	void AddFonts(const char* filename,int fontSize);

	/// <summary>
	/// フォントを適用したテキストの横幅を取得する
	/// </summary>
	/// <param name="string">フォントを適用する文字列</param>
	/// <param name="filename">フォントの名前</param>
	/// <returns>フォントを敵将下テキストの横幅</returns>
	int GetStringSize(const char* string, const char* filename);

	/// <summary>
	/// フォントを取得する
	/// </summary>
	/// <param name="filename">フォントの名前</param>
	/// <returns>作成したフォント</returns>
	int GetFontHandle(const char* filename);

private:

	int titleWidth_ = 0;

	FontsManager();

	FontsManager(const FontsManager&) = delete;
	void operator = (const FontsManager&) = delete;

	std::unordered_map<std::string,int> fontHandle_;

};

