#include "InputState.h"
#include <DxLib.h>
#include <cassert>

#include<iostream>
#include <nlohmann/json.hpp>
#include <fstream>
#include <cassert>

using json = nlohmann::json;
using namespace std;

InputState::InputState()
{
	//ポーズ
	defaultMapTable_[InputType::pause] =		{ {InputCategory::keybd,KEY_INPUT_TAB},
												{InputCategory::pad,PAD_INPUT_8} };
	//上
	defaultMapTable_[InputType::up] =			{ {InputCategory::keybd,KEY_INPUT_W},
												{InputCategory::pad,PAD_INPUT_UP} };
	//下
	defaultMapTable_[InputType::down] =			{ {InputCategory::keybd,KEY_INPUT_S},
												{InputCategory::pad,PAD_INPUT_DOWN} };
	//左
	defaultMapTable_[InputType::left] =			{ {InputCategory::keybd,KEY_INPUT_A},
												{InputCategory::pad,PAD_INPUT_LEFT} };
	//右
	defaultMapTable_[InputType::right] =		{ {InputCategory::keybd,KEY_INPUT_D},
												{InputCategory::pad,PAD_INPUT_RIGHT} };
	//上矢印
	defaultMapTable_[InputType::upArrow] =		{ {InputCategory::keybd,KEY_INPUT_UP},
												/*{InputCategory::pad,PAD_INPUT_UP}*/ };
	//下矢印
	defaultMapTable_[InputType::downArrow] =	{ {InputCategory::keybd,KEY_INPUT_DOWN},
												/*{InputCategory::pad,PAD_INPUT_DOWN}*/ };
	//左矢印
	defaultMapTable_[InputType::leftArrow] =	{ {InputCategory::keybd,KEY_INPUT_LEFT},
												/*{InputCategory::pad,PAD_INPUT_LEFT}*/ };
	//右矢印
	defaultMapTable_[InputType::rightArrow] =	{ {InputCategory::keybd,KEY_INPUT_RIGHT},
												 /*{InputCategory::pad,PAD_INPUT_RIGHT}*/ };
	//Space
	defaultMapTable_[InputType::space] =		{ {InputCategory::keybd,KEY_INPUT_SPACE},
												{InputCategory::pad,PAD_INPUT_1} };
	//死亡
	defaultMapTable_[InputType::death] =		{ {InputCategory::keybd,KEY_INPUT_Z},
												{InputCategory::pad,PAD_INPUT_4} };
	//shift
	defaultMapTable_[InputType::shift] =		{ {InputCategory::keybd,KEY_INPUT_LSHIFT},
												{InputCategory::pad,PAD_INPUT_3} };
	//activate
	defaultMapTable_[InputType::activate] =		{ {InputCategory::keybd,KEY_INPUT_F},
												{InputCategory::pad,PAD_INPUT_2} };

	defaultMapTable_[InputType::creative] =		{ {InputCategory::keybd,KEY_INPUT_Q},
												{InputCategory::pad,PAD_INPUT_5} };

	inputMapTable_ = defaultMapTable_;

	//loadKeyInfo();
	LoadKeyInfo2("keyInfo");

	//一時マップテーブルにコピー
	tempMapTable_ = inputMapTable_;

	//入力タイプの名前のテーブルを作る
	inputNameTable_[InputType::pause] = "ポーズ";
	inputNameTable_[InputType::up] = "上へ移動";
	inputNameTable_[InputType::down] = "下へ移動";
	inputNameTable_[InputType::left] = "左へ移動";
	inputNameTable_[InputType::right] = "右へ移動";
	inputNameTable_[InputType::upArrow] = "上を見る";
	inputNameTable_[InputType::downArrow] = "下を見る";
	inputNameTable_[InputType::leftArrow] = "左を見る";
	inputNameTable_[InputType::rightArrow] = "右を見る";
	inputNameTable_[InputType::space] = "決定/ジャンプ";
	inputNameTable_[InputType::death] = "死亡";
	inputNameTable_[InputType::shift] = "走る";
	inputNameTable_[InputType::activate] = "アクション";


	currentInput_.resize(static_cast<int>(InputType::max));
	lastInput_.resize(static_cast<int>(InputType::max));

}

bool InputState::IsTriggered(InputType type) const
{
	return IsPressed(type) && !lastInput_[static_cast<int>(type)];
}

bool InputState::IsPressed(InputType type) const
{
	
	return currentInput_[static_cast<int>(type)];
}

void InputState::Update()
{

	lastInput_ = currentInput_;//直前の入力情報を記憶しておく

	char keystate[256];
	GetHitKeyStateAll(keystate);//全キー情報取得

	int padState = GetJoypadInputState(DX_INPUT_PAD1);//パッド1コンの情報を取得する

	for (const auto& keymap : inputMapTable_) {			//マップの全情報をループする
		for (const auto& input : keymap.second) {		//入力情報配列をループする
			//この中身は、keybd,KEY_INPUT_RETURNなどのセット(InputInfo)が入ってる
			//keymap.secondには、この入力情報セットInputInfoが入っている。
			//keymap.firstには、対応するゲーム入力名の"InputType"などが入っている

			if (input.cat == InputCategory::keybd) {
				currentInput_[static_cast<int>(keymap.first)] = keystate[input.id];
				if (currentInput_[static_cast<int>(keymap.first)]) {
					currentInputDevice_ = true;
				}
			}
			else if (input.cat == InputCategory::pad) {
				currentInput_[static_cast<int>(keymap.first)] = padState & input.id;
				if (currentInput_[static_cast<int>(keymap.first)]) {
					currentInputDevice_ = false;
				}
			}
			//2つの入力のうちどれかがtrueだったらもう「入力されてる」
			//とみなして、breakする。
			if (currentInput_[static_cast<int>(keymap.first)]) {
				break;
			}
		}
	}
}

void InputState::RewriteInputInfo(InputType type, InputCategory cat, int id)
{
	//入力種別(割り当て先)がなければ、無効なので無視します。
	if (tempMapTable_.count(type) == 0) {
		return;
	}

	bool isRewrite = false;

	for (auto& info : tempMapTable_[type]) {
		if (info.cat == cat)//カテゴリがヒットしたら
		{
			info.id = id;//IDを上書きする
			isRewrite = true;
			break;
		}
	}
	if (!isRewrite) {//もしカテゴリが存在しなかったら、ここで追加しておく
		tempMapTable_[type].push_back({ cat,id });
	}
}

//変更を許諾する
void InputState::CommitChangedInputInfo()
{
	inputMapTable_ = tempMapTable_;
}

//前の状態に戻す
void InputState::RollbackChangedInputInfo()
{
	tempMapTable_ = inputMapTable_;
}

//リセット
void InputState::ResetInputInfo()
{
	inputMapTable_ = defaultMapTable_;
	tempMapTable_ = defaultMapTable_;
}

void InputState::UndoSelectKey(InputType type, InputCategory cat)
{
	for (auto& info : tempMapTable_[type]) {
		if (info.cat == cat) {
			info.id = inputMapTable_[type].begin()->id;
			break;
		}
	}
}

//TODO：消す
//書き出し
//もう使わないと思う
void InputState::SavekeyInfo() const
{
	FILE* fp = nullptr;

	auto err = fopen_s(&fp, "key.info", "wb");
	assert(fp != nullptr);

	//仮想キータイプの数を書き込む
	int keyTypeNum = inputMapTable_.size();
	fwrite(&keyTypeNum, sizeof(keyTypeNum), 1, fp);
	//仮想キータイプ(next、prevなど)のループ
	for (const auto& key : inputMapTable_) {
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

//TODO：消す
//読み込み
//もう使わないと思う
void InputState::LoadKeyInfo()
{
	int handle = FileRead_open("key.info");
	assert(handle != -1);

	int keyTypeNum = 0;
	FileRead_read(&keyTypeNum, sizeof(keyTypeNum), handle);

	//初期化した中身を消す
	inputMapTable_.clear();

	//外部データの読み込み
	for (int i = 0; i < keyTypeNum; i++) {
		int  inputType;
		FileRead_read(&inputType, sizeof(inputType), handle);

		int dataSize = 0;
		FileRead_read(&dataSize, sizeof(dataSize), handle);
		std::vector<InputInfo>inputInfoes(dataSize);
		FileRead_read(inputInfoes.data(), sizeof(InputInfo) * dataSize, handle);
		inputMapTable_[static_cast<InputType>(inputType)] = inputInfoes;
	}
	tempMapTable_ = inputMapTable_;
	FileRead_close(handle);
}

void InputState::SavekeyInfo2() const
{
	//決め打ちしか出来ないのがネック
	json keyInfo_[static_cast<int>(InputType::max)];
	//配列番号を進めるための変数(出来るなら書きたくない)
	int i = 0;
	for (auto& input : inputMapTable_) {
		//inputMapTable_(map型)のキーをもとに値を取得する
		//vectorのサイズを値のサイズ分用意する
		std::vector<InputInfo> rewrite(input.second.size());
		rewrite = input.second;

		//※※※他の入力装置が増えた場合は手動でプログラムを増やさないと行けない※※※
		//各入力装置名と対応するidを取得
		//キーボード
		std::unordered_map<int, int> keybord;
		keybord[static_cast<int>(rewrite.front().cat)] = static_cast<int>(rewrite.front().id);
		//パッド
		std::unordered_map<int, int> pad;
		pad[static_cast<int>(rewrite.back().cat)] = static_cast<int>(rewrite.back().id);

		//各入力装置とid、サイズ、inputTypeをデータにまとめる
		keyInfo_[i] = {
			{"inputType",static_cast<int>(input.first)},
			{"inputSize",static_cast<int>(input.second.size())},
			{"keybord",keybord},
			{"pad",pad},
		};
		i++;
	}

	//出力したデータをまとめる
	json keySize = {
		{"name","keyInfo"},
		{"keyTypeNum",inputMapTable_.size()},
		{"temp",keyInfo_}
	};

	//書き出すファイル名を"name"にする(別に無くてもいい)
	string filename = keySize["name"];
	string extension = ".json";
	filename += extension;
	filename = "data/jsonFile/" + filename;

	//出力
	ofstream writeingFile;
	//ファイルを開く
	writeingFile.open(filename, ios::out);
	//まとめたデータを一列にまとめる
	writeingFile << keySize.dump() << std::endl;
	//閉じる
	writeingFile.close();

}

void InputState::LoadKeyInfo2(const char* filename)
{

	//初期化した中身を消す
	inputMapTable_.clear();

	//読み込むファイルのパスを生成
	string path = "data/jsonFile/";
	path += filename;
	path += ".json";

	//読み込むこと
	ifstream ifs(path.c_str());
	assert(ifs);

	//ビットをシフトする？
	//よくわかっていない
	//習ったけど覚えてない
	json json_;
	ifs >> json_;

	//名前を取得している(正直いらないけど残してる(笑))
	string name = json_["name"];
	//inputStateの総数
	int keyTypeNum = json_["keyTypeNum"];

	//決め打ちしか出来ないのがネック(知識、理解不足)
	json inputInfo[static_cast<int>(InputType::max)];
	//↓嫌だこれ嫌い
	int i = 0;

	//各入力装置、idを取得
	for (auto& aiu : json_["temp"]) {
		inputInfo[i] = aiu;
		i++;
	}

	//inputMapTable_に読み込んだデータを代入
	for (auto& const info : inputInfo) {
		//入力装置番号とキーidをそれぞれ保持するためのやつ(ひとまとめにできるなら死体)
		std::unordered_map<int, int> keybord;
		std::unordered_map<int, int> pad;

		//読み込んだ情報を一時保管するための変数
		InputInfo tempInfo;

		//データを取得
		int type = info["inputType"];
		int inputSize = info["inputSize"];
		keybord = info["keybord"];
		pad = info["pad"];

		//各入力装置とキーidをひとまとめに持つための変数
		std::vector<InputInfo> inputInfo(inputSize);

		//各入力装置とキーidを取得
		tempInfo.cat = static_cast<InputCategory>(keybord.begin()->first);
		tempInfo.id = keybord.begin()->second;
		inputInfo.front() = tempInfo;
		tempInfo.cat = static_cast<InputCategory>(pad.begin()->first);
		tempInfo.id = pad.begin()->second;
		inputInfo.back() = tempInfo;

		//読み込んだ情報を代入する
		inputMapTable_[static_cast<InputType>(type)] = inputInfo;
	}

	//一応閉じる
	ifs.close();
}

bool InputState::LastInputDevice() const
{
	return currentInputDevice_;
}
