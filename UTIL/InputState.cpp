#include "InputState.h"
#include "InputState.h"
#include "DxLib.h"
#include <cassert>

InputState::InputState()
{
	//次へ
	defaultMapTable[InputType::next] = { {InputCategory::keybd,KEY_INPUT_RETURN},
										{InputCategory::pad,PAD_INPUT_1}};
	//戻る
	defaultMapTable[InputType::prev] = { {InputCategory::keybd,KEY_INPUT_BACK},
										  {InputCategory::pad,PAD_INPUT_2} };
	//ポーズ
	defaultMapTable[InputType::pause] = { {InputCategory::keybd,KEY_INPUT_TAB},
										 {InputCategory::pad,PAD_INPUT_8} };
	//上
	defaultMapTable[InputType::up] = { {InputCategory::keybd,KEY_INPUT_W},
									  {InputCategory::pad,PAD_INPUT_UP} };
	//下
	defaultMapTable[InputType::down] = { {InputCategory::keybd,KEY_INPUT_S},
										{InputCategory::pad,PAD_INPUT_DOWN} };
	//左
	defaultMapTable[InputType::left] = { {InputCategory::keybd,KEY_INPUT_A},
										{InputCategory::pad,PAD_INPUT_LEFT} };
	//右
	defaultMapTable[InputType::right] = { {InputCategory::keybd,KEY_INPUT_D},
									  {InputCategory::pad,PAD_INPUT_RIGHT} };
	//Space
	defaultMapTable[InputType::space] = { {InputCategory::keybd,KEY_INPUT_SPACE},
									  {InputCategory::pad,PAD_INPUT_1} };
	//死亡
	defaultMapTable[InputType::death] = { {InputCategory::keybd,KEY_INPUT_Z},
									  {InputCategory::pad,PAD_INPUT_2} };
	//shift
	defaultMapTable[InputType::shift] = { {InputCategory::keybd,KEY_INPUT_LSHIFT},
									  {InputCategory::pad,PAD_INPUT_3} };
	//ctrl
	defaultMapTable[InputType::ctrl] = { {InputCategory::keybd,KEY_INPUT_LCONTROL},
									  {InputCategory::pad,PAD_INPUT_4} };

	inputMapTable = defaultMapTable;

	//loadKeyInfo();

	//一時マップテーブルにコピー
	tempMapTable = inputMapTable;

	//入力タイプの名前のテーブルを作る
	inputNameTable[InputType::next] = "決定";
	inputNameTable[InputType::prev] = "戻る";
	inputNameTable[InputType::pause] = "ポーズ";
	inputNameTable[InputType::up] = "上";
	inputNameTable[InputType::down] = "下";
	inputNameTable[InputType::left] = "左";
	inputNameTable[InputType::right] = "右";
	inputNameTable[InputType::space] = "space";
	inputNameTable[InputType::death] = "死亡";
	inputNameTable[InputType::shift] = "shift";
	inputNameTable[InputType::ctrl] = "ctrl";


	currentInput.resize(static_cast<int>(InputType::max));
	lastInput.resize(static_cast<int>(InputType::max));
}

bool InputState::isTriggered(InputType type) const
{
	return isPressed(type) && !lastInput[static_cast<int>(type)];
}

bool InputState::isPressed(InputType type) const
{
	return currentInput[static_cast<int>(type)];
}

void InputState::update()
{
	lastInput = currentInput;//直前の入力情報を記憶しておく

	char keystate[256];
	GetHitKeyStateAll(keystate);//全キー情報取得

	int padState = GetJoypadInputState(DX_INPUT_PAD1);//パッド1コンの情報を取得する
	int mouseState = GetMouseInput();

	for (const auto& keymap : inputMapTable) {			//マップの全情報をループする
		for (const auto& input : keymap.second) {		//入力情報配列をループする
			//この中身は、keybd,KEY_INPUT_RETURNなどのセット(InputInfo)が入ってる
			//keymap.secondには、この入力情報セットInputInfoが入っている。
			//keymap.firstには、対応するゲーム入力名の"InputType"などが入っている
			if (input.cat == InputCategory::keybd) {
				currentInput[static_cast<int>(keymap.first)] = keystate[input.id];
			}
			else if (input.cat == InputCategory::pad) {
				currentInput[static_cast<int>(keymap.first)] = padState & input.id;
			}
			//2つの入力のうちどれかがtrueだったらもう「入力されてる」
			//とみなして、breakする。
			if (currentInput[static_cast<int>(keymap.first)]) {
				break;
			}
		}
	}
}

void InputState::rewriteInputInfo(InputType type, InputCategory cat, int id)
{
	//入力種別(割り当て先)がなければ、無効なので無視します。
	if (tempMapTable.count(type) == 0) {
		return;
	}

	bool isRewrite = false;

	for (auto& InputInfo : tempMapTable[type]) {
		if (InputInfo.cat == cat)//カテゴリがヒットしたら
		{
			InputInfo.id = id;//IDを上書きする
			isRewrite = true;
			break;
		}
	}
	if (!isRewrite) {//もしカテゴリが存在しなかったら、ここで追加しておく
		tempMapTable[type].push_back({ cat,id });
	}
}

//変更を許諾する
void InputState::commitChangedInputInfo()
{
	inputMapTable = tempMapTable;
}

//前の状態に戻す
void InputState::rollbackChangedInputInfo()
{
	tempMapTable = inputMapTable;
}

//リセット
void InputState::resetInputInfo()
{
	inputMapTable = defaultMapTable;
	tempMapTable = defaultMapTable;
}

//書き出し
void InputState::savekeyInfo() const
{
	FILE* fp = nullptr;

	auto err = fopen_s(&fp, "key.info", "wb");
	assert(fp != nullptr);

	//仮想キータイプの数を書き込む
	int keyTypeNum = inputMapTable.size();
	fwrite(&keyTypeNum, sizeof(keyTypeNum), 1, fp);
	//仮想キータイプ(next、prevなど)のループ
	for (const auto& key : inputMapTable) {
		int keytype = static_cast<int>(key.first);
		//仮想キー番号
		fwrite(&keytype, sizeof(keytype), 1, fp);
		int dataSize = key.second.size();
		//いくつ入力データがあるのか
		fwrite(&dataSize, sizeof(dataSize), 1, fp);

		//Vector型のdata()は先頭のアドレスを返す
		//一気にその入力データを書き込む
		fwrite(key.second.data(), dataSize * sizeof(InputInfo), 1, fp);
	}

	fclose(fp);
}
//読み込み
void InputState::loadKeyInfo()
{
	int handle = FileRead_open("key.info");
	assert(handle != -1);

	int keyTypeNum = 0;
	FileRead_read(&keyTypeNum, sizeof(keyTypeNum), handle);

	//初期化した中身を消す
	inputMapTable.clear();

	//外部データの読み込み
	for (int i = 0; i < keyTypeNum; i++) {
		int  inputType;
		FileRead_read(&inputType, sizeof(inputType), handle);

		int dataSize = 0;
		FileRead_read(&dataSize, sizeof(dataSize), handle);
		std::vector<InputInfo>inputInfoes(dataSize);
		FileRead_read(inputInfoes.data(), sizeof(InputInfo) * dataSize, handle);
		inputMapTable[static_cast<InputType>(inputType)] = inputInfoes;
	}
	tempMapTable = inputMapTable;
	FileRead_close(handle);
}