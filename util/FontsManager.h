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
	/// �t�H���g��������
	/// </summary>
	/// <param name="filename">�t�@�C���̖��O</param>
	/// <param name="fontSize">�t�H���g�̃T�C�Y�w��</param>
	void AddFonts(const char* filename,int fontSize);

	/// <summary>
	/// �t�H���g��K�p�����e�L�X�g�̉������擾����
	/// </summary>
	/// <param name="string">�t�H���g��K�p���镶����</param>
	/// <param name="filename">�t�H���g�̖��O</param>
	/// <returns>�t�H���g��G�����e�L�X�g�̉���</returns>
	int GetStringSize(const char* string, const char* filename);

	/// <summary>
	/// �t�H���g���擾����
	/// </summary>
	/// <param name="filename">�t�H���g�̖��O</param>
	/// <returns>�쐬�����t�H���g</returns>
	int GetFontHandle(const char* filename);

private:

	int titleWidth_ = 0;

	FontsManager();

	FontsManager(const FontsManager&) = delete;
	void operator = (const FontsManager&) = delete;

	std::unordered_map<std::string,int> fontHandle_;

};

