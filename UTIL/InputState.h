#pragma once

#include <vector>
#include <map>
#include <unordered_map>
#include <string>

enum class InputType
{
	next,			/// 次へ行くボタン
	prev,			/// 前に戻るボタン
	pause,			/// ポーズボタン
	up,				///上
	down,			///下
	left,			///左
	right,			///右
	upArrow,		///上
	downArrow,		///下
	leftArrow,		///左
	rightArrow,		///右
	space,			///space
	death,			//死亡入力
	shift,			//shift
	ctrl,			///ctrl
	max,			///最大入力インデックス
};

enum class InputCategory {
	keybd,		//キーボード
	pad,		//ゲームパッド
};

struct InputInfo {
	InputCategory cat;
	int id;
};

class KeyConfigScene;

class InputState
{
	friend KeyConfigScene;
public:
	InputState();

	bool isTriggered(InputType type) const;


	bool isPressed(InputType type) const;


	void update();


	void rewriteInputInfo(InputType type, InputCategory cat, int id);


	void commitChangedInputInfo();


	void rollbackChangedInputInfo();


	void resetInputInfo();


	void savekeyInfo()const;

	void loadKeyInfo();

	void savekeyInfo2()const;

	void loadKeyInfo2(const char* filename);

private:

	using InputMap_t = std::map<InputType, std::vector<InputInfo> >;
	InputMap_t inputMapTable_;
	InputMap_t inputMapTable2_;

	InputMap_t tempMapTable_;

	InputMap_t defaultMapTable_;

	std::map<InputType, std::string> inputNameTable_;

	std::vector<bool> currentInput_;
	std::vector<bool> lastInput_;

	std::unordered_map<int, int> rewriteKeyInfo_;
	std::vector<InputInfo> temp2;
};
