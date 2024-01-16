#pragma once
#include <string>
#include <unordered_map>

class FontsManager
{
public:

	~FontsManager();

	static FontsManager& GetInstance() {
		static FontsManager instance;
		return instance;
	}

	/// <summary>
	/// フォントを加える
	/// </summary>
	/// <param name="filename">ファイルの名前</param>
	/// <param name="extension">拡張子</param>
	/// <param name="fontSize">フォントのサイズ指定</param>
	void AddFonts(const char* const filename,const char* const extension,int fontSize);

	/// <summary>
	/// フォントを適用したテキストの横幅を取得する
	/// </summary>
	/// <param name="string">フォントを適用する文字列</param>
	/// <param name="filename">フォントの名前</param>
	/// <returns>フォントを敵将下テキストの横幅</returns>
	int GetStringSize(const char* const string, const char* const filename);

	/// <summary>
	/// フォントを取得する
	/// </summary>
	/// <param name="filename">フォントの名前</param>
	/// <returns>作成したフォント</returns>
	int GetFontHandle(const char* const filename);

	/// <summary>
	/// 引数の値を元にフォントの名前を返す
	/// </summary>
	/// <param name="fontHandle">フォントハンドル</param>
	/// <returns>フォント名</returns>
	std::string GetFontName(int fontHandle);

private:

	FontsManager();

	FontsManager(const FontsManager&) = delete;
	void operator = (const FontsManager&) = delete;

	std::unordered_map<std::string,int> fontHandle_;		//文字列をキーとしたフォントを保管するための配列

};

