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
class Tutorial;

class InputState
{
	friend KeyConfigScene;
	friend Tutorial;
public:
	~InputState();

	static InputState& GetInstance() {
		static InputState instance;
		return instance;
	}

	/// <summary>
	/// 長押し不可能入力関数
	/// </summary>
	/// <param name="type">キーのタイプ</param>
	/// <returns>押しているかどうか</returns>
	bool IsTriggered(InputType type) const;

	/// <summary>
	/// 長押し可能入力関数
	/// </summary>
	/// <param name="type">キーのタイプ</param>
	/// <returns>押しているかどうか</returns>
	bool IsPressed(InputType type) const;

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 変更中のキーコンフィグをtempMapTableに書き込む
	/// </summary>
	/// <param name="type">キータイプ</param>
	/// <param name="cat">入力装置のカテゴリ</param>
	/// <param name="id">キーの番号</param>
	void RewriteInputInfo(InputType type, InputCategory cat, int id);

	/// <summary>
	/// キーの変更を実行する
	/// </summary>
	void CommitChangedInputInfo();

	/// <summary>
	/// 変更前のキーコンフィグに戻す
	/// </summary>
	void RollbackChangedInputInfo();

	/// <summary>
	/// キーコンフィグを初期化する
	/// </summary>
	void ResetInputInfo();


	void UndoSelectKey(InputType type, InputCategory cat);

	/// <summary>
	/// キーコンフィグを外部ファイルとして保存する
	/// </summary>
	void SavekeyInfo()const;

	/// <summary>
	/// キーコンフィグを読み込む
	/// </summary>
	/// <param name="filename">外部ファイルのパス</param>
	void LoadKeyInfo(const char* filename);

	/// <summary>
	/// 最後に入力された入力装置を判別する
	/// </summary>
	/// <returns>true:キーボード　false：パッド</returns>
	bool LastInputDevice() const;

private:

	InputState();

	InputState(const InputState&) = delete;
	void operator = (const InputState&) = delete;

	bool currentInputDevice_ = false;			//true:キーボード　false:パッド

	using InputMap_t = std::map<InputType, std::vector<InputInfo>>;
	InputMap_t inputMapTable_;

	InputMap_t tempMapTable_;

	InputMap_t defaultMapTable_;

	std::map<InputType, std::string> inputNameTable_;

	std::vector<bool> currentInput_;
	std::vector<bool> lastInput_;

	std::unordered_map<int, int> rewriteKeyInfo_;
};
