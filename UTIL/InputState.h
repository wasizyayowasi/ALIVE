#pragma once

#include <vector>
#include <map>
#include <unordered_map>
#include <string>

enum class InputType
{
	pause,			// ポーズボタン
	up,				//上
	down,			//下
	left,			//左
	right,			//右
	upArrow,		//上
	downArrow,		//下
	leftArrow,		//左
	rightArrow,		//右
	space,			//space
	death,			//死亡入力
	shift,			//shift
	activate,		//運ぶ
	creative,		//デバッグ用
	max,			//最大入力インデックス
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

	bool IsTriggered(InputType type) const;

	bool IsPressed(InputType type) const;

	void Update();

	void RewriteInputInfo(InputType type, InputCategory cat, int id);

	void CommitChangedInputInfo();

	void RollbackChangedInputInfo();

	void ResetInputInfo();

	void UndoSelectKey(InputType type, InputCategory cat);

	void SavekeyInfo()const;

	void LoadKeyInfo();

	void SavekeyInfo2()const;

	void LoadKeyInfo2(const char* filename);

	bool LastInputDevice() const;

private:

	bool currentInputDevice_ = false;			//true:キーボード　false:パッド

	using InputMap_t = std::map<InputType, std::vector<InputInfo>>;
	InputMap_t inputMapTable_;

	InputMap_t tempMapTable_;

	InputMap_t defaultMapTable_;

	std::map<InputType, std::string> inputNameTable_;

	std::vector<bool> currentInput_;
	std::vector<bool> lastInput_;

	std::unordered_map<int, int> rewriteKeyInfo_;
	std::vector<InputInfo> temp2;
};
