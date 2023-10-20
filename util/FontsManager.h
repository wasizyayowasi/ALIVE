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
	/// <param name="extension">�g���q</param>
	/// <param name="fontSize">�t�H���g�̃T�C�Y�w��</param>
	void AddFonts(const char* const filename,const char* const extension,int fontSize);

	/// <summary>
	/// �t�H���g��K�p�����e�L�X�g�̉������擾����
	/// </summary>
	/// <param name="string">�t�H���g��K�p���镶����</param>
	/// <param name="filename">�t�H���g�̖��O</param>
	/// <returns>�t�H���g��G�����e�L�X�g�̉���</returns>
	int GetStringSize(const char* const string, const char* const filename);

	/// <summary>
	/// �t�H���g���擾����
	/// </summary>
	/// <param name="filename">�t�H���g�̖��O</param>
	/// <returns>�쐬�����t�H���g</returns>
	int GetFontHandle(const char* const filename);

	/// <summary>
	/// �����̒l�����Ƀt�H���g�̖��O��Ԃ�
	/// </summary>
	/// <param name="fontHandle">�t�H���g�n���h��</param>
	/// <returns>�t�H���g��</returns>
	std::string GetFontName(int fontHandle);

private:

	FontsManager();

	FontsManager(const FontsManager&) = delete;
	void operator = (const FontsManager&) = delete;

	std::unordered_map<std::string,int> fontHandle_;		//��������L�[�Ƃ����t�H���g��ۊǂ��邽�߂̔z��

};
