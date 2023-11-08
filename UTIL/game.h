#pragma once
#include "DxLib.h"

//ゲーム全般の設定
namespace Game {
	//windowモード
	constexpr bool window_mode = true;
	//window名
	const char* const title_text = "ALIVE";
	//windowサイズ
	constexpr int screen_width = 1920;
	constexpr int screen_height = 1080;
	//カラーモード
	constexpr int screen_depth = 32;     //32 or 16
}