#include "Util.h"
#include <algorithm>
#include <stdio.h>

namespace MathUtil 
{
	//二点間の距離のサイズを取得する
	float GetSizeOfDistanceTwoPoints(VECTOR pos, VECTOR targetPos)
	{
		VECTOR distance = {};
		distance = VSub(pos, targetPos);

		return VSize(distance);
	}

	//VECTOR型、度数法を弧度法に変換する
	VECTOR VECTORDegreeToRadian(VECTOR rot)
	{
		rot.x = rot.x * DX_PI_F / 180.0f;
		rot.y = rot.y * DX_PI_F / 180.0f;
		rot.z = rot.z * DX_PI_F / 180.0f;

		return rot;
	}

	//float型、度数法を弧度法に変換する
	float DegreeToRadian(float angle)
	{
		angle = angle * DX_PI_F / 180.0f;

		return angle;
	}

}

namespace StrUtil
{
	//文字列から数値を取得する
	int StrUtil::GetNumberFromString(std::string name, std::string sign)
	{
		//文字列の数を取得
		int strSize = name.size();

		//文字列の数が0以下だったら0を返す
		if (strSize <= 0) {
			return 0;
		}

		//文字列から第二引数がある番号を取得する
		int searchStrNum = name.find(sign);

		//上記で取得した番号から第一引数の文字列の末尾までを取得
		auto str = name.substr(searchStrNum + 1, strSize);

		//文字列を数値に変換する
		int num = atoi(str.c_str());

		return num;
	}

	std::string GetConcatenateNumAndStrings(std::string name, std::string sign, int num)
	{
		//数値を文字列にする
		std::string value = std::to_string(num);
		//nameに記号(sign)と文字列に変換したvalue(num)を連結
		name = name + sign + value;

		return name;
	}

	std::string GetStringWithPartsAfterTheSymbolDeleted(std::string name, std::string sign)
	{
		//文字列を保管
		std::string str = name;

		if (name.find(sign) != -1) {
			str = str.erase(name.find(sign));
		}

		return str;
	}

	std::string GetStringAfterSign(std::string name, std::string sign)
	{
		//文字列のサイズを取得
		int size = name.size();

		//「.」が文字列の何番目かを取得する
		int dotNum = name.find(sign);

		//「.」以降から最後までの文字列を取得する
		std::string str = name.substr(dotNum + 1, size);

		return str;
	}

}


