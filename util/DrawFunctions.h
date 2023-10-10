#pragma once

namespace Graph {

	///<summary>
	///�O���t�̓ǂݍ���
	///</summary>
	/// <param name="path">�ǂݍ��݂����O���t�̃t�@�C���p�X</param>
	int LoadGraph(const char* path);

    /// <summary>
	/// �ǂݍ��񂾃O���t�������̒l�ɂ���ĕ����`�悷��
	/// </summary>
	/// <param name="x,y">�\��������WXY</param>
	/// <param name="left,top">���蔲�������摜�̍�����WXY</param>
	/// <param name="width,height">�摜�P��������̉����A�����̃T�C�Y</param>
	/// <param name="scale">�摜�̊g�嗦</param>
	/// <param name="angle">�摜�̉�]��</param>
	/// <param name="handle">�ǂݍ��񂾉摜</param>
	/// <param name="transFlag">���߂��邩</param>
	/// <param name="tumFlag">���]�����邩</param>
	int DrawRectRotaGraph(float x, float y, int left, int top, int width, int height, float scale, float angle, int handle, bool transFlag, bool tumFlag);

}

