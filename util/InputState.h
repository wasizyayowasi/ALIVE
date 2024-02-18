#pragma once
#include "KeyData.h"
#include <vector>
#include <map>
#include <unordered_map>
#include <string>

enum class InputType
{
	Pause,			// ポーズボタン
	Up,				//上
	Down,			//下
	Left,			//左
	Right,			//右
	UpArrow,		//上
	DownArrow,		//下
	LeftArrow,		//左
	RightArrow,		//右
	Space,			//space
	Death,			//死亡入力
	Dush,			//走る
	Activate,		//運ぶ
	Creative,		//デバッグ用
	max,			//最大入力インデックス
};

enum class InputCategory {
	keybd,		//キーボード
	pad,		//ゲームパッド
};

struct InputInfo {
	InputCategory cat;		//メモリに保存されない(コンパイル時展開)
	int id;					//4byte
};

class Tutorial;
class KeyConfigScene;
class KeyConfigSceneForSceneTitle;

class InputState
{
	friend Tutorial;
	friend KeyConfigScene;
	friend KeyConfigSceneForSceneTitle;
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

	/// <summary>
	/// 変更したキーを戻す
	/// </summary>
	/// <param name="type">キーのタイプ</param>
	/// <param name="cat">入力装置のカテゴリ</param>
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

	/// <summary>
	/// 入力装置の番号を取得する
	/// </summary>
	/// <param name="type">ボタンのタイプ</param>
	/// <param name="cat">入力装置のカテゴリー</param>
	/// <returns>入力装置の番号</returns>
	int GetInputNum(int num,InputCategory cat);

	/// <summary>
	/// key画像の描画
	/// </summary>
	/// <param name="type">描画したいkey　castでint型にした値</param>
	/// <param name="posX">描画座標X</param>
	/// <param name="posY">描画座標Y</param>
	/// <param name="size">描画サイズY</param>
	void DrawKeyGraph(int type,float posX, float posY, float size);

	/// <summary>
	/// padのボタンの描画
	/// </summary>
	/// <param name="type">描画したいボタン castでint型にした値</param>
	/// <param name="posX">描画座標X</param>
	/// <param name="posY">描画座標Y</param>
	/// <param name="scale">描画サイズY</param>
	void DrawPadGraph(int type, float posX, float posY,float scale);

	/// <summary>
	/// 名前の描画
	/// </summary>
	/// <param name="type">描画したいkey</param>
	/// <param name="posX">描画座標X</param>
	/// <param name="posY">描画座標Y</param>
	/// <param name="color">色</param>
	/// <param name="fontHandle">フォント</param>
	
	/// <summary>
	/// 名前の描画
	/// </summary>
	/// <param name="type">描画したいkey　castでint型にした値</param>
	/// <param name="posX">描画座標X</param>
	/// <param name="posY">描画座標Y</param>
	/// <param name="color">色</param>
	/// <param name="fontHandle">フォント</param>
	/// <param name="editName">名前を編集するか</param>
	/// <param name="before">前か後か　true : 前　false：後ろ</param>
	/// <param name="sign">記号</param>
	void DrawName(int type, float posX, float posY, int color, int fontHandle,bool editName,bool before,std::string sign = "");
private:

	InputState();

	InputState(const InputState&) = delete;
	void operator = (const InputState&) = delete;


	bool currentInputDevice_ = false;			//true:キーボード　false:パッド

	using InputMap_t = std::map<InputType, std::vector<InputInfo>>;
	InputMap_t inputMapTable_;

	InputMap_t tempMapTable_;

	InputMap_t defaultMapTable_;

	std::map<int, Key> keyNum_;							//キー番号
	std::map<InputType, std::string> inputNameTable_;

	std::vector<bool> currentInput_;
	std::vector<bool> lastInput_;

	std::unordered_map<int, int> rewriteKeyInfo_;
};
