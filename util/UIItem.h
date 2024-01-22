#pragma once
#include <DxLib.h>

class UIItem
{
public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	UIItem();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~UIItem();

	/// <summary>
	/// �T�C�Y�����A�A���t�@�l�̕ύX�������UI�`��
	/// </summary>
	/// <param name="scale">�T�C�Y</param>
	/// <param name="alpha">�A���t�@�l</param>
	void AlphaChangeDraw(float scale, int alpha);

	/// <summary>
	/// UI�摜���쐬���邽�߂̈ʒu��񓙂�ݒ肷��
	/// </summary>
	/// <param name="centerPosX">�|�W�V������X</param>
	/// <param name="centerPosY">�|�W�V������Y</param>
	/// <param name="width"></param>
	/// <param name="height"></param>
	/// <param name="str">������</param>
	/// <param name="fontHandle">�t�H���g�n���h��</param>
	void CreateUIGraphSetUp(float centerPosX, float centerPosY, int width, int height, const char* str, int fontHandle);

	/// <summary>
	/// UI�摜�̍쐬
	/// </summary>
	/// <param name="fontHandle">�t�H���g�n���h��</param>
	void CreateUIGraph(int fontHandle);

private:
	
	int makeScreenHandle_ = 0;
	int makeScreenWidth_ = 0;
	int makeScreenHeight_ = 0;

	float centerPosX_ = 0;
	float centerPosY_ = 0;
	float scale_ = 1.0f;

	const char* UIString = nullptr;

};

