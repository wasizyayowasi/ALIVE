#pragma once
#include <DxLib.h>
#include <string>

namespace MathUtil 
{
	//二点間の距離のサイズを取得する
	float GetSizeOfDistanceTwoPoints(VECTOR pos, VECTOR targetPos);

	//VECTOR型、度数法を弧度法に変換する
	VECTOR VECTORDegreeToRadian(VECTOR rot);

	//float型、度数法を弧度法に変換する
	float DegreeToRadian(float angle);
}

namespace StrUtil 
{
	//文字列から数値を取得する
	int GetNumberFromString(std::string name, std::string sign);

	//文字列と数値を結合した文字列を取得する
	std::string GetConcatenateNumAndStrings(std::string name, std::string sign, int num);

	//文字列からsign以降の文字列を削除した文字列を取得する
	std::string GetStringWithPartsAfterTheSymbolDeleted(std::string name, std::string sign);

	//文字列から記号以降の文字列を取得する
	std::string GetStringAfterSign(std::string name, std::string sign);

	//文字列から記号以前の文字列を取得する
	std::string GetStringBeforeSign(std::string name, std::string sign);
}